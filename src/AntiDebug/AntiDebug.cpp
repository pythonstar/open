#include "StdAfx.h"
#include "AntiDebug.h"
#include <Winternl.h>
#include <TlHelp32.h>
#include <process.h>


//////////////////////////////////////////////////////////////////////////
PVOID CAntiDebug::m_lpOldHandler=NULL;
DWORD CAntiDebug::m_dwSafeAddr=0;
//////////////////////////////////////////////////////////////////////////




CAntiDebug::CAntiDebug(void)
{
}

CAntiDebug::~CAntiDebug(void)
{
}


BOOL CAntiDebug::CheckDbgPort(void)
{
	typedef BOOL (WINAPI *FuncCheckRemoteDebuggerPresent)(HANDLE, DWORD*);
	typedef NTSTATUS (WINAPI *FuncNtQueryInformationProcess)(
		IN HANDLE ProcessHandle,
		IN PROCESSINFOCLASS ProcessInformationClass,
		OUT PVOID ProcessInformation,
		IN ULONG ProcessInformationLength,
		OUT PULONG ReturnLength OPTIONAL
		);

	DWORD bDebuggerPresent=FALSE;
	FuncCheckRemoteDebuggerPresent pCheckRemoteDebuggerPresent=
		(FuncCheckRemoteDebuggerPresent)GetProcAddress( GetModuleHandle(_T("kernel32.dll")),"CheckRemoteDebuggerPresent" );

	if ( pCheckRemoteDebuggerPresent ){
		pCheckRemoteDebuggerPresent(GetCurrentProcess(),&bDebuggerPresent);
	}

	bDebuggerPresent=IsDebuggerPresent();
	DWORD dwReturnLen;
	FuncNtQueryInformationProcess pNtQueryInformationProcess= 
		(FuncNtQueryInformationProcess)GetProcAddress(GetModuleHandle(_T("ntdll.dll")), "NtQueryInformationProcess");
	if ( pNtQueryInformationProcess ){
		pNtQueryInformationProcess(GetCurrentProcess(),(PROCESSINFOCLASS)7,&bDebuggerPresent,sizeof(bDebuggerPresent),&dwReturnLen);
	}
	
	return bDebuggerPresent;
}

//PEB!NtGlobalFlags
BOOL CAntiDebug::CheckDebugger_Method2()
{
	// 只能检测自己的进程是否在调试中
	__asm {
		mov eax, fs:[30h]
		mov eax, [eax+0x68]
		and	eax, 0x70
	}
}

BOOL CAntiDebug::CheckHeapFlags()
{
	__asm{
		mov eax,fs:[30h]
		mov eax,[eax+18h]	;process heap
		mov eax,[eax+10h]	;heap flags
	}
}

BOOL CAntiDebug::CheckDebugger_Method3(HANDLE hProcess)
{
	typedef long NTSTATUS; 
#define STATUS_SUCCESS    ((NTSTATUS)0L) 

	typedef struct _SYSTEM_KERNEL_DEBUGGER_INFORMATION { 
		BOOLEAN DebuggerEnabled; 
		BOOLEAN DebuggerNotPresent; 
	} SYSTEM_KERNEL_DEBUGGER_INFORMATION, *PSYSTEM_KERNEL_DEBUGGER_INFORMATION; 

	typedef struct _PROCESS_DEBUG_PORT_INFO { 
		HANDLE DebugPort; 
	} PROCESS_DEBUG_PORT_INFO; 

	enum SYSTEM_INFORMATION_CLASS { SystemKernelDebuggerInformation = 35 }; 
	enum THREAD_INFO_CLASS        { ThreadHideFromDebugger          = 17 }; 
	enum PROCESS_INFO_CLASS       { ProcessDebugPort                = 7  }; 

	typedef NTSTATUS  (__stdcall *ZW_QUERY_SYSTEM_INFORMATION)(IN SYSTEM_INFORMATION_CLASS SystemInformationClass, IN OUT PVOID SystemInformation, IN ULONG SystemInformationLength, OUT PULONG ReturnLength); 
	typedef NTSTATUS  (__stdcall *ZW_SET_INFORMATION_THREAD)(IN HANDLE ThreadHandle, IN THREAD_INFO_CLASS ThreadInformationClass, IN PVOID ThreadInformation, IN ULONG ThreadInformationLength); 
	typedef NTSTATUS  (__stdcall *ZW_QUERY_INFORMATION_PROCESS)(IN HANDLE ProcessHandle, IN PROCESS_INFO_CLASS ProcessInformationClass, OUT PVOID ProcessInformation, IN ULONG ProcessInformationLength, OUT PULONG ReturnLength); 

	BOOL bFound=FALSE;

	// 开始检测
	ZW_QUERY_SYSTEM_INFORMATION ZwQuerySystemInformation;
	ZW_QUERY_INFORMATION_PROCESS ZwQueryInformationProcess;
	SYSTEM_KERNEL_DEBUGGER_INFORMATION Info;
	PROCESS_DEBUG_PORT_INFO ProcessInfo;

	HMODULE hModule = GetModuleHandle(_T("ntdll.dll"));
	if ( hModule!=NULL ) {
		ZwQuerySystemInformation = (ZW_QUERY_SYSTEM_INFORMATION)GetProcAddress(hModule, "ZwQuerySystemInformation");
		ZwQueryInformationProcess = (ZW_QUERY_INFORMATION_PROCESS)GetProcAddress(hModule, "ZwQueryInformationProcess");

		// 检测全局调试器
		if ( ZwQuerySystemInformation ) {
			if ( STATUS_SUCCESS==ZwQuerySystemInformation(SystemKernelDebuggerInformation, &Info, sizeof(Info), NULL) ) {
				if ( Info.DebuggerEnabled && !Info.DebuggerNotPresent ) {
					bFound=TRUE;
				}
			}
		}

		// 检测局部调试器
		if ( ZwQueryInformationProcess ) {
			if ( STATUS_SUCCESS==ZwQueryInformationProcess(hProcess, ProcessDebugPort, &ProcessInfo, sizeof(ProcessInfo), NULL) ) {
				if ( ProcessInfo.DebugPort ) {
					bFound=TRUE;
				}
			}
		}
	}
	

	return bFound;
}


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

BOOL GetPIDPath(DWORD dwPID, LPTSTR exe_path, int max_path)
{
	BOOL success = FALSE;
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	MODULEENTRY32 me32;

	// Take a snapshot of all modules in the specified process.
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
	if( hModuleSnap == INVALID_HANDLE_VALUE )
	{
		return( FALSE );
	}

	// Set the size of the structure before using it.
	me32.dwSize = sizeof( MODULEENTRY32 );

	// Retrieve information about the first module, and exit if unsuccessful
	if( !Module32First( hModuleSnap, &me32 ) )
	{
		CloseHandle( hModuleSnap );    // Must clean up the
		return( FALSE );
	}

	do   
	{
		if (me32.th32ProcessID == dwPID)   
		{   
			_tcsncpy(exe_path, me32.szExePath, max_path-1);
			success = TRUE;
			break;
		}   
	} while(Module32Next(hModuleSnap, &me32));   

	CloseHandle(hModuleSnap);     
	return success;
}   


//检测父进程路径不在系统目录下并且不和本进程相同路径，则认为是调试器
BOOL CAntiDebug::CheckDebugger_Method4(DWORD processid)
{
	BOOL has_debugger = FALSE;
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	PROCESSENTRY32 me32;

	// Take a snapshot of all modules in the specified process.
	hModuleSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	if( hModuleSnap == INVALID_HANDLE_VALUE )
	{
		return( FALSE );
	}

	// Set the size of the structure before using it.
	me32.dwSize = sizeof( PROCESSENTRY32 );

	// Retrieve information about the first module, and exit if unsuccessful
	if( !Process32First( hModuleSnap, &me32 ) )
	{
		CloseHandle( hModuleSnap );    // Must clean up the
		return( FALSE );
	}

	// Now walk the module list of the process, and display information about each module
	do
	{
		if (me32.th32ProcessID == processid) 
		{
			TCHAR full_path[MAX_PATH] = {0};
			if (GetPIDPath(me32.th32ParentProcessID, full_path, MAX_PATH))
			{
				// 如果是系统目录的话，
				TCHAR system_path[MAX_PATH] = {0};
				GetSystemWindowsDirectory(system_path, MAX_PATH);
				if (!_tcsstr(full_path, system_path))
				{
					TCHAR cur_path[MAX_PATH] = {0};
					::GetModuleFileName(NULL, cur_path, MAX_PATH);
					TCHAR* p2 = _tcsrchr(cur_path, _T('\\'));
					if (p2)
					{
						*p2 = 0;
						if (!_tcsstr(full_path, cur_path))
						{
							has_debugger = TRUE;
						}
					}
				}
			}
			break;
		}
	} while( Process32Next ( hModuleSnap, &me32 ) );

	CloseHandle( hModuleSnap );
	return has_debugger;
}


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

//如果给CloseHandle()函数一个无效句柄作为输入参数，
//在无调试器时，将会返回一个错误代码，而有调试器存在时，将会触发一个EXCEPTION_INVALID_HANDLE (0xc0000008)的异常。
BOOL CAntiDebug::CheckDebugger_Method5()
{
	//	APIs making user of the ZwClose syscall (such as CloseHandle, indirectly) 
	//can be used to detect a debugger. When a process is debugged, calling ZwClose 
	//with an invalid handle will generate a STATUS_INVALID_HANDLE (0xC0000008) exception.
	//	As with all anti-debugs that rely on information made directly available 
	//from the kernel (therefore involving a syscall), the only proper way to bypass 
	//the "CloseHandle" anti-debug is to either modify the syscall data from ring3, 
	//before it is called, or set up a kernel hook.
	//	This anti-debug, though extremely powerful, does not seem to be widely used 
	//by malicious programs.

	BOOL bFound=FALSE;

	__try{
		CloseHandle(HANDLE(0x00001234));
	}__except(1){
		bFound=TRUE;
	}

#ifdef _DEBUG
	if ( bFound ){
		::MessageBox(0,_T("CheckDebugger_Method5 find debugger"),0,0);
	}
#endif

	return bFound;
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

//当一个进程获得SeDebugPrivilege，它就获得了对CSRSS.EXE的完全控制，这种特权也会被子进程继承，
//也就是说一个被调试的程序如果获得了CSRSS.EXE的进程ID,它就可以使用openprocess操作CSRSS.EXE
BOOL CAntiDebug::CheckDebugger_Method6()
{
	DWORD csrss_id = -1;
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	PROCESSENTRY32 me32;

	// Take a snapshot of all modules in the specified process.
	hModuleSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	if( hModuleSnap == INVALID_HANDLE_VALUE )
	{
		return( FALSE );
	}

	// Set the size of the structure before using it.
	me32.dwSize = sizeof( PROCESSENTRY32 );

	// Retrieve information about the first module, and exit if unsuccessful
	if( !Process32First( hModuleSnap, &me32 ) )
	{
		CloseHandle( hModuleSnap );    // Must clean up the
		return( FALSE );
	}

	// Now walk the module list of the process, and display information about each module
	do
	{
		if (!_tcsicmp(me32.szExeFile, _T("csrss.exe")))
		{
			csrss_id = me32.th32ProcessID;
			break;
		}

	} while( Process32Next ( hModuleSnap, &me32 ) );

	CloseHandle( hModuleSnap );

	// 如果没有找到，直接返回
	if (csrss_id==-1) 
	{
		return FALSE;
	}

	// 只能检测本进程
	HANDLE handle = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, csrss_id);  
	if (handle)
	{
		// 如果能打开，表示就有问题
		CloseHandle(handle);
		return TRUE;
	}
	return FALSE;
}


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

//查找调试器窗口
BOOL CAntiDebug::CheckDebugger_Method7()
{
	BOOL bFound=FALSE;
	TCHAR title[260];

	HWND hwnd = ::GetForegroundWindow();
	if (hwnd)
	{
		::GetWindowText(hwnd, title, 260);
		_tcslwr(title);
		if (_tcsstr(title, _T("lly"))) 
		{
			bFound=TRUE;
		}
		else if (_tcsstr(title, _T("debug")))
		{
			bFound=TRUE;
		}
		else if (_tcsstr(title, _T("dbg")))
		{
			bFound=TRUE;
		}
		else if (_tcsstr(title, _T("ida")))
		{
			bFound=TRUE;
		}
		else if (_tcsstr(title, _T("asm")))
		{
			bFound=TRUE;
		}
	}

	if (FindWindow(NULL, _T("ollydbg"))) {
		bFound=TRUE;
	}

	if (FindWindow(NULL, _T("TIdaWindow"))) {
		bFound=TRUE;
	}

#ifdef _DEBUG
	if ( bFound ){
		::MessageBox(0,title,_T("发现调试器窗口"),0);
	}
#endif


	return bFound;
}


//  DebugObject: NtQueryObject()
BOOL CAntiDebug::CheckDebugObject()
{
#ifndef STATUS_INFO_LENGTH_MISMATCH 
#define STATUS_INFO_LENGTH_MISMATCH ((UINT32)0xC0000004L) 
#endif 

	typedef enum _POOL_TYPE { 
		NonPagedPool, 
		PagedPool, 
		NonPagedPoolMustSucceed, 
		DontUseThisType, 
		NonPagedPoolCacheAligned, 
		PagedPoolCacheAligned, 
		NonPagedPoolCacheAlignedMustS 
	} POOL_TYPE; 

	typedef struct _UNICODE_STRING { 
		USHORT Length; 
		USHORT MaximumLength; 
		PWSTR Buffer; 
	} UNICODE_STRING; 
	typedef UNICODE_STRING *PUNICODE_STRING; 
	typedef const UNICODE_STRING *PCUNICODE_STRING; 

	typedef enum _OBJECT_INFORMATION_CLASS 
	{ 
		ObjectBasicInformation,   // Result is OBJECT_BASIC_INFORMATION structure 
		ObjectNameInformation,   // Result is OBJECT_NAME_INFORMATION structure 
		ObjectTypeInformation,   // Result is OBJECT_TYPE_INFORMATION structure 
		ObjectAllTypesInformation,   // Result is OBJECT_ALL_INFORMATION structure 
		ObjectDataInformation  // Result is OBJECT_DATA_INFORMATION structure 

	} OBJECT_INFORMATION_CLASS, *POBJECT_INFORMATION_CLASS; 

	typedef struct _OBJECT_TYPE_INFORMATION { 
		UNICODE_STRING TypeName;  
		ULONG TotalNumberOfHandles;  
		ULONG TotalNumberOfObjects;  
		WCHAR Unused1[8];  
		ULONG HighWaterNumberOfHandles;  
		ULONG HighWaterNumberOfObjects;  
		WCHAR Unused2[8];  
		ACCESS_MASK InvalidAttributes;  
		GENERIC_MAPPING GenericMapping;  
		ACCESS_MASK ValidAttributes;  
		BOOLEAN SecurityRequired;  
		BOOLEAN MaintainHandleCount;  
		USHORT MaintainTypeList;  
		POOL_TYPE PoolType;  
		ULONG DefaultPagedPoolCharge;  
		ULONG DefaultNonPagedPoolCharge; 
	} OBJECT_TYPE_INFORMATION, *POBJECT_TYPE_INFORMATION; 

	typedef struct _OBJECT_ALL_INFORMATION { 
		ULONG NumberOfObjectsTypes;  
		OBJECT_TYPE_INFORMATION ObjectTypeInformation[1]; 
	} OBJECT_ALL_INFORMATION, *POBJECT_ALL_INFORMATION; 

	typedef struct _OBJECT_ALL_TYPES_INFORMATION { 
		ULONG NumberOfTypes; 
		OBJECT_TYPE_INFORMATION TypeInformation[1]; 
	} OBJECT_ALL_TYPES_INFORMATION, *POBJECT_ALL_TYPES_INFORMATION; 

	typedef UINT32 (__stdcall  *ZwQueryObject_t) (       
		IN HANDLE ObjectHandle,  
		IN OBJECT_INFORMATION_CLASS ObjectInformationClass,  
		OUT PVOID ObjectInformation,  
		IN ULONG Length,  
		OUT PULONG ResultLength ); 

	//////////////////////////////////////////////////////////////////////////
	BOOL bFound=FALSE;

	//  调试器必须正在调试才能检测到，仅打开 OD 是检测不到的 
	HMODULE hNtDLL; 
	DWORD dwSize; 
	UINT i; 
	UCHAR  KeyType=0; 
	OBJECT_ALL_TYPES_INFORMATION *Types; 
	OBJECT_TYPE_INFORMATION *t; 
	ZwQueryObject_t ZwQueryObject; 

	hNtDLL = GetModuleHandle(_T("ntdll.dll")); 
	if(hNtDLL){ 
		ZwQueryObject = (ZwQueryObject_t)GetProcAddress(hNtDLL, "ZwQueryObject"); 
		UINT32 iResult = ZwQueryObject(NULL, ObjectAllTypesInformation, NULL, NULL, &dwSize); 
		if(iResult==STATUS_INFO_LENGTH_MISMATCH) 
		{ 
			Types=(OBJECT_ALL_TYPES_INFORMATION*)VirtualAlloc(NULL,dwSize,MEM_COMMIT,PAGE_READWRITE); 
			if (Types == NULL)  
				return bFound; 

			if (iResult=ZwQueryObject(NULL,ObjectAllTypesInformation, Types, dwSize, &dwSize)) 
				return bFound; 

			for (t=Types->TypeInformation,i=0;i<Types->NumberOfTypes;i++) 
			{    
				if ( !_wcsicmp(t->TypeName.Buffer,L"DebugObject")) //比较两个是否相等，这个 L很特殊，本地的意思 
				{    
					if(t->TotalNumberOfHandles > 0 || t->TotalNumberOfObjects > 0) 
					{ 
						MessageBox(0,_T("发现 OD"),0,0); 
						VirtualFree (Types,0,MEM_RELEASE); 
						return bFound; 
					} 
					break; // Found Anyways 
				} 
				t=(OBJECT_TYPE_INFORMATION  *)((char*)t->TypeName.Buffer+((t->TypeName.MaximumLength+3)&~3)); 
			} 
		} 

		//没有发现调试器
		VirtualFree (Types,0,MEM_RELEASE); 
	} 


	return bFound; 
}


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

BOOL CAntiDebug::CheckDeleteFib()
{
	BOOL bFound=FALSE;

	char fib[1024] = {0};	
	DeleteFiber(fib); 

	if(GetLastError() == 0x00000057){
		//MessageBoxA(NULL,"This process is NOT debugged","Info",MB_OK);
	}else{
		bFound=TRUE;
		//MessageBoxA(NULL,"This process IS debugged","Info",MB_OK);
	}

#ifdef _DEBUG
	if ( bFound ){
		::MessageBox(0,_T("CheckDeleteFib find debugger"),0,0);
	}
#endif

	return bFound;
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

BOOL CAntiDebug::TimeChecker()
{
	DWORD time_begin;
	DWORD time_end;
	int time_low, time_high;

	JUNK_CODE_FOUR;
	JUNK_CODE_THREE;
	time_begin = ::GetTickCount();
	JUNK_CODE_TWO;

	JUNK_CODE_THREE;
	time_low = ::GetTickCount();
	JUNK_CODE_ONE;
	JUNK_CODE_ONE;

	JUNK_CODE_ONE;
	time_high = ::GetTickCount();
	JUNK_CODE_FIVE;

	for (int i=0;i<200;++i) {
		time_low += time_high;
		time_low -= i;
	}

	JUNK_CODE_ONE;
	__asm
	{
		rdtsc
		mov time_low,eax
		mov time_high,edx
	}

	for (int i=0;i<200;++i) {
		time_low -= time_high;
		JUNK_CODE_ONE;
		time_low += i;
	}
	JUNK_CODE_ONE;
	JUNK_CODE_ONE;
	time_end = GetTickCount();

	__asm
	{
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
	}

	__try{
		__asm { 
			int 2dh
			inc eax				;any opcode of singlebyte.
			JUNK_CODE_FIVE
		}
	}__except(1){
		JUNK_CODE_THREE;
		JUNK_CODE_ONE;

		// 阈值设为13秒吧，防止机器突然卡死等问题
		if (time_end>=time_begin&&(time_end-time_begin < 13000)) {
			return FALSE;
		}
	}

	return TRUE;
}

//禁用CR4控制寄存器的Time Stamp Disable位,ring3执行rdtsc会引发异常(特权指令)
// 接管0D中断:http://bbs.pediy.com/showthread.php?t=14837	9楼
BOOL CAntiDebug::CR4TSD()
{
	//_asm int 3

	__asm
	{
		_emit 0x0F    // mov eax,cr4,Set TSD
		_emit 0x20
		_emit 0xE0

		or eax,0x4

		_emit 0x0F    // mov cr4,eax
		_emit 0x22
		_emit 0xE0


		rdtsc
	}

}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

BOOL CAntiDebug::FTPushSSPopSS()
{
	__asm
	{
		push ebp
			mov ebp, esp

			push ss 
			pop ss 

			pushf
			pop eax 
			and eax, 0x100 
			or eax, eax 
			jnz _debugged

			xor eax, eax
			jmp _end

_debugged:
		mov eax, 1
_end:
		mov esp,ebp
		pop ebp
	}
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
// The IsDbgPresentPrefixCheck works in at least two debuggers
// OllyDBG and VS 2008, by utilizing the way the debuggers handle
// prefixes we can determine their presence. Specifically if this code
// is ran under a debugger it will simply be stepped over;
// however, if there is no debugger SEH will fire :D

//004110C8 >  F3:64:          prefix rep:     	; (initial cpu selection)
//004110CA    F1              int1
BOOL CAntiDebug::IsDbgPresentPrefixCheck()
{
	__try
	{
		__asm __emit 0xF3 // 0xF3 0x64 disassembles as PREFIX REP:
		__asm __emit 0x64
		__asm __emit 0xF1 // One byte INT 1
	}
	__except(1)
	{
		return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
// 直接停止调试:ZwSetInformationThread 可以用来将线程隐藏，从而使调试器接收不到信息
BOOL CAntiDebug::DisableDebugEvent()
{
	// 关闭调试端口
	typedef DWORD (WINAPI *ZW_SET_INFORMATION_THREAD)(HANDLE, DWORD, PVOID, ULONG);
	static const DWORD ThreadHideFromDebugger = 17;

	HMODULE module=::GetModuleHandle(_T("ntdll.dll"));
	if ( module!=NULL ){
		ZW_SET_INFORMATION_THREAD ZwSetInformationThread_;
		ZwSetInformationThread_ = (ZW_SET_INFORMATION_THREAD)GetProcAddress(module, "ZwSetInformationThread");
		if (ZwSetInformationThread_){
			(*ZwSetInformationThread_)(GetCurrentThread(), ThreadHideFromDebugger, 0, 0);
		}
	}
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

typedef LPTOP_LEVEL_EXCEPTION_FILTER (_stdcall*pSetUnhandledExceptionFilter)(LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter); 
pSetUnhandledExceptionFilter lpSetUnhandledExceptionFilter; 

//////////////////////////////////////////////////////////////////////////

LONG WINAPI CAntiDebug::TopUnhandledExceptionFilter(_EXCEPTION_POINTERS *ExceptionInfo) 
{ 
	_asm pushad 
	//AfxMessageBox("回调函数"); 
	lpSetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER )m_lpOldHandler);
	ExceptionInfo->ContextRecord->Eip=m_dwSafeAddr;//转移到安全位置 
	_asm popad 
	return EXCEPTION_CONTINUE_EXECUTION; 
}

BOOL CAntiDebug::SetUnhandledExceptionFilterMethod()
{
	bool isDebugged=0; 
	lpSetUnhandledExceptionFilter= 
		(pSetUnhandledExceptionFilter)GetProcAddress( GetModuleHandle(_T("kernel32.dll")),"SetUnhandledExceptionFilter" );  
	m_lpOldHandler=lpSetUnhandledExceptionFilter(TopUnhandledExceptionFilter); 
	_asm{   
		mov m_dwSafeAddr,offset __safe //获取这个安全地址
		//int 3  //触发异常 
		mov ecx,offset __safe
		mov eax,[ecx+4]
		mov eax,[eax]
	}  
	isDebugged=1; 
	_asm{ 
__safe:  
	} 
	
#ifdef _DEBUG
	if ( isDebugged ){
		MessageBox(0,_T("SetUnhandledExceptionFilterMethod find debugger"),0,0);
	}
#endif

}


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

#ifdef _ANTIDEBUG

VOID NTAPI test_PIMAGE_TLS_CALLBACK(PVOID DllHandle, DWORD Reason, PVOID Reserved)
{
	printf("test_PIMAGE_TLS_CALLBACK\n");
#if 1
	IMAGE_DOS_HEADER *dos_head=(IMAGE_DOS_HEADER *)GetModuleHandle(NULL);
	PIMAGE_NT_HEADERS32 nt_head=(PIMAGE_NT_HEADERS32)((DWORD)dos_head+(DWORD)dos_head->e_lfanew);
	BYTE*OEP=(BYTE*)(nt_head->OptionalHeader.AddressOfEntryPoint+(DWORD)dos_head);

	//下面的代码则通过扫描程序入口点的20字节，判断其中有无调试断点，如有，则退出进程。
	for(unsigned long index=0;index<20;index++)
	{
		if (OEP[index]==0xcc) {
#ifndef _DEBUG
			CAntiDebug::DisableDebugEvent();
#endif
			break;
		}
	}
#endif

	return;
}

#pragma comment(linker, "/INCLUDE:__tls_used")
#pragma data_seg(".CRT$XLB")
PIMAGE_TLS_CALLBACK TlsCallBackArray[] = {
	test_PIMAGE_TLS_CALLBACK,
	NULL
};
#pragma data_seg()

#endif

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////


// xp 以上
bool CAntiDebug::InstallAntiAttach()
{
	HMODULE ntdll;                   // ntdll handle
	void* pDbgUiRemoteBreakin;       // function handle
	DWORD dwOldProtect;              // just for fun
	DWORD dwCodeSize;                // Size of code to copy

	// Get ntdll.dll handle
	ntdll = GetModuleHandle( _T("ntdll.dll") );
	if(ntdll)
	{
		// Get target function addr
		pDbgUiRemoteBreakin = GetProcAddress(ntdll, "DbgUiRemoteBreakin");
		if(pDbgUiRemoteBreakin)
		{
			__asm
			{
				// Get code size
				lea eax, __CodeToCopyStart
				lea ecx, __CodeToCopyEnd
				sub ecx, eax
				mov dwCodeSize, ecx
			}
			// Make sure that we have write rights ...
			if(VirtualProtect(pDbgUiRemoteBreakin, dwCodeSize, PAGE_EXECUTE_READWRITE, &dwOldProtect))
			{
				__asm
				{
					// Copy code between __CodeToCopyStart and __CodeToCopyEnd
					mov edi, pDbgUiRemoteBreakin
						lea esi, __CodeToCopyStart
						mov ecx, dwCodeSize
						rep movsb
						// Skip code
						jmp __CodeEnd

__CodeToCopyStart:
					lea eax, __CodeToCopyEnd
						jmp eax
__CodeToCopyEnd:
				}

				// ***CODE*HERE***
				__asm
				{
					// Clear registers
					xor eax, eax
					pushfd
					mov [esp], eax
					popfd
					xor ebx, ebx
					xor ecx, ecx
					xor edx, edx
					xor esi, esi
					xor edi, edi
					xor esp, esp
					xor ebp, ebp
					// Jump to address 0
					jmp eax
				}
				// ***************

__CodeEnd:;
				return true;
			}
		}
	}
	return false;
}


// xp 以上
bool CAntiDebug::InstallAntiAttach2()
{
	HMODULE ntdll;                   // ntdll handle
	void* pDbgUiRemoteBreakin;       // function handle
	DWORD dwOldProtect;              // just for fun
	DWORD dwCodeSize;                // Size of code to copy

	// Get ntdll.dll handle
	ntdll = GetModuleHandle( _T("ntdll.dll") );
	if(ntdll)
	{
		// Get target function addr
		pDbgUiRemoteBreakin = GetProcAddress(ntdll, "NtContinue");
		if(pDbgUiRemoteBreakin)
		{
			__asm
			{
				// Get code size
				lea eax, __CodeToCopyStart
					lea ecx, __CodeToCopyEnd
					sub ecx, eax
					mov dwCodeSize, ecx
			}
			// Make sure that we have write rights ...
			if(VirtualProtect(pDbgUiRemoteBreakin, dwCodeSize, PAGE_EXECUTE_READWRITE, &dwOldProtect))
			{
				__asm
				{
					// Copy code between __CodeToCopyStart and __CodeToCopyEnd
					mov edi, pDbgUiRemoteBreakin
						lea esi, __CodeToCopyStart
						mov ecx, dwCodeSize
						rep movsb
						// Skip code
						jmp __CodeEnd

__CodeToCopyStart:
					lea eax, __CodeToCopyEnd
						jmp eax
__CodeToCopyEnd:
				}

				// ***CODE*HERE***
				__asm
				{
					// Clear registers
					xor eax, eax
					pushfd
					mov [esp], eax
					popfd
					xor ebx, ebx
					xor ecx, ecx
					xor edx, edx
					xor esi, esi
					xor edi, edi
					xor esp, esp
					xor ebp, ebp
					// Jump to address 0
					jmp eax
				}
				// ***************

__CodeEnd:;
				return true;
			}
		}
	}
	return false;
}



// xp 以上
bool CAntiDebug::InstallAntiAttach3()
{
	HMODULE ntdll;                   // ntdll handle
	void* pDbgUiRemoteBreakin;       // function handle
	DWORD dwOldProtect;              // just for fun
	DWORD dwCodeSize;                // Size of code to copy

	// Get ntdll.dll handle
	ntdll = GetModuleHandle( _T("ntdll.dll") );
	if(ntdll)
	{
		// Get target function addr
		pDbgUiRemoteBreakin = GetProcAddress(ntdll, "DbgBreakPoint");
		if(pDbgUiRemoteBreakin)
		{
			__asm
			{
				// Get code size
				lea eax, __CodeToCopyStart
					lea ecx, __CodeToCopyEnd
					sub ecx, eax
					mov dwCodeSize, ecx
			}
			// Make sure that we have write rights ...
			if(VirtualProtect(pDbgUiRemoteBreakin, dwCodeSize, PAGE_EXECUTE_READWRITE, &dwOldProtect))
			{
				__asm
				{
					// Copy code between __CodeToCopyStart and __CodeToCopyEnd
					mov edi, pDbgUiRemoteBreakin
						lea esi, __CodeToCopyStart
						mov ecx, dwCodeSize
						rep movsb
						// Skip code
						jmp __CodeEnd

__CodeToCopyStart:
					lea eax, __CodeToCopyEnd
						jmp eax
__CodeToCopyEnd:
				}

				// ***CODE*HERE***
				__asm
				{
					// Clear registers
					xor eax, eax
						pushfd
						mov [esp], eax
						popfd
						xor ebx, ebx
						xor ecx, ecx
						xor edx, edx
						xor esi, esi
						xor edi, edi
						xor esp, esp
						xor ebp, ebp
						// Jump to address 0
						jmp eax
				}
				// ***************

__CodeEnd:;
				return true;
			}
		}
	}
	return false;
}



/* 线程人口 */
UINT CAntiDebug::execute()
{
	while ( m_bRunning ){
		CheckDbgPort();
		CheckDebugger_Method2();
		CheckHeapFlags();
		CheckDebugger_Method3(GetCurrentProcess());
		CheckDebugger_Method4(GetCurrentProcessId());
		CheckDebugger_Method5();
		CheckDebugger_Method6();
		CheckDebugger_Method7();
		CheckDeleteFib();
		TimeChecker();
		FTPushSSPopSS();
		IsDbgPresentPrefixCheck();
		DisableDebugEvent();
		
		::Sleep(10000);
	}

	return 0;
}

//开始反调试检测
BOOL CAntiDebug::Start(void)
{
	m_hThread = (HANDLE)_beginthreadex(NULL,0,ThreadProc,(void*)this,0,&m_nThreadId);
	if (m_hThread==INVALID_HANDLE_VALUE)
		return FALSE;

	m_bRunning=TRUE;
	return TRUE;
}

//停止反调试检测
void CAntiDebug::Stop(void)
{
	if ( m_hThread!=INVALID_HANDLE_VALUE ){
		m_bRunning=FALSE;
		WaitForSingleObject(m_hThread,INFINITE);
		m_hThread=INVALID_HANDLE_VALUE;
	}
}

//等待检测结束
void CAntiDebug::Wait(void)
{
	WaitForSingleObject(m_hThread,INFINITE);
}
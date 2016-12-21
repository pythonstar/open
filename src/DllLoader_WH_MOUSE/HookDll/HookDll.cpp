// HookDll.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "HookDll.h"
#include <Shlwapi.h>
#include <Winsock2.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//


//////////////////////////////////////////////////////////////////////////
HHOOK		hhk;
HANDLE		hMapFile;
PSHAREMEM	pShareMem;
int			nSize;
//////////////////////////////////////////////////////////////////////////
UINT		nTimerId;


// CHookDllApp

BEGIN_MESSAGE_MAP(CHookDllApp, CWinApp)
END_MESSAGE_MAP()



//////////////////////////////////////////////////////////////////////////
LRESULT MouseProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	return CallNextHookEx(hhk,nCode,wParam,lParam);
}

__declspec(dllexport) void __stdcall StartHook(HANDLE hMainWnd,DWORD dwThreadId)
{
	pShareMem->hMainWnd=hMainWnd;
	hhk=SetWindowsHookEx(WH_MOUSE,(HOOKPROC)MouseProc,AfxGetInstanceHandle(),dwThreadId);
}
__declspec(dllexport) void __stdcall StopHook()
{
	if (hhk)
	{
		UnhookWindowsHookEx(hhk);
		hhk=NULL;
	}
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
int __stdcall NewRecv(DWORD s,char* buf,int len,int flags)
{
	int Result;
	typedef  int (__stdcall*TRecv)(DWORD s,char* buf,int len,int flags);
	__asm
	{
		pushad
	}
	theApp.Hook.Restore();
	__asm
	{
		popad
	}
	Result=TRecv(theApp.Hook.OldFunction)(s,buf,len,flags);
	::MessageBox(0,"O(∩_∩)O哈哈~","Fucked",0);
	theApp.Hook.Change();
	return Result;
}
//////////////////////////////////////////////////////////////////////////
int __stdcall NewWSARecv(SOCKET s,LPWSABUF lpBuffers,DWORD dwBufferCount,LPDWORD lpNumberOfBytesRecvd,
			LPDWORD lpFlags,LPWSAOVERLAPPED lpOverlapped,LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	int Result;
	typedef  int (__stdcall*TWSARecv)(SOCKET s,LPWSABUF lpBuffers,DWORD dwBufferCount,LPDWORD lpNumberOfBytesRecvd,
		LPDWORD lpFlags,LPWSAOVERLAPPED lpOverlapped,LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
	__asm
	{
		pushad
	}
	theApp.Hook2.Restore();
	__asm
	{
		popad
	}
	Result=TWSARecv(theApp.Hook2.OldFunction)(s,lpBuffers,dwBufferCount,lpNumberOfBytesRecvd,
		lpFlags,lpOverlapped,lpCompletionRoutine);

	unsigned char data[352] = {
		0x48, 0x54, 0x54, 0x50, 0x2F, 0x31, 0x2E, 0x31, 0x20, 0x32, 0x30, 0x30, 0x20, 0x4F, 0x4B, 0x0D, 
		0x0A, 0x43, 0x61, 0x63, 0x68, 0x65, 0x2D, 0x43, 0x6F, 0x6E, 0x74, 0x72, 0x6F, 0x6C, 0x3A, 0x20, 
		0x70, 0x72, 0x69, 0x76, 0x61, 0x74, 0x65, 0x0D, 0x0A, 0x44, 0x61, 0x74, 0x65, 0x3A, 0x20, 0x57, 
		0x65, 0x64, 0x2C, 0x20, 0x30, 0x38, 0x20, 0x4A, 0x75, 0x6E, 0x20, 0x32, 0x30, 0x31, 0x31, 0x20, 
		0x30, 0x34, 0x3A, 0x35, 0x37, 0x3A, 0x30, 0x34, 0x20, 0x47, 0x4D, 0x54, 0x0D, 0x0A, 0x43, 0x6F, 
		0x6E, 0x74, 0x65, 0x6E, 0x74, 0x2D, 0x4C, 0x65, 0x6E, 0x67, 0x74, 0x68, 0x3A, 0x20, 0x31, 0x30, 
		0x39, 0x0D, 0x0A, 0x43, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x2D, 0x54, 0x79, 0x70, 0x65, 0x3A, 
		0x20, 0x74, 0x65, 0x78, 0x74, 0x2F, 0x68, 0x74, 0x6D, 0x6C, 0x0D, 0x0A, 0x53, 0x65, 0x72, 0x76, 
		0x65, 0x72, 0x3A, 0x20, 0x4D, 0x69, 0x63, 0x72, 0x6F, 0x73, 0x6F, 0x66, 0x74, 0x2D, 0x49, 0x49, 
		0x53, 0x2F, 0x36, 0x2E, 0x30, 0x0D, 0x0A, 0x58, 0x2D, 0x50, 0x6F, 0x77, 0x65, 0x72, 0x65, 0x64, 
		0x2D, 0x42, 0x79, 0x3A, 0x20, 0x41, 0x53, 0x50, 0x2E, 0x4E, 0x45, 0x54, 0x0D, 0x0A, 0x53, 0x65, 
		0x74, 0x2D, 0x43, 0x6F, 0x6F, 0x6B, 0x69, 0x65, 0x3A, 0x20, 0x41, 0x53, 0x50, 0x53, 0x45, 0x53, 
		0x53, 0x49, 0x4F, 0x4E, 0x49, 0x44, 0x51, 0x53, 0x54, 0x51, 0x44, 0x42, 0x53, 0x54, 0x3D, 0x50, 
		0x42, 0x4E, 0x48, 0x44, 0x46, 0x48, 0x44, 0x4A, 0x43, 0x4A, 0x43, 0x4E, 0x4D, 0x4B, 0x47, 0x48, 
		0x48, 0x47, 0x4A, 0x4D, 0x48, 0x4B, 0x41, 0x3B, 0x20, 0x70, 0x61, 0x74, 0x68, 0x3D, 0x2F, 0x0D, 
		0x0A, 0x0D, 0x0A, 0x20, 0x20, 0x20, 0x31, 0x4B, 0x46, 0x34, 0x33, 0x35, 0x32, 0x44, 0x35, 0x46, 
		0x39, 0x43, 0x45, 0x30, 0x30, 0x38, 0x34, 0x37, 0x34, 0x45, 0x31, 0x43, 0x38, 0x46, 0x32, 0x33, 
		0x32, 0x43, 0x44, 0x44, 0x42, 0x45, 0x41, 0x30, 0x44, 0x32, 0x34, 0x36, 0x41, 0x33, 0x37, 0x42, 
		0x41, 0x31, 0x34, 0x34, 0x42, 0x37, 0x38, 0x43, 0x37, 0x44, 0x36, 0x31, 0x46, 0x42, 0x43, 0x43, 
		0x33, 0x38, 0x42, 0x33, 0x41, 0x34, 0x37, 0x36, 0x42, 0x42, 0x31, 0x42, 0x33, 0x45, 0x37, 0x36, 
		0x44, 0x30, 0x30, 0x36, 0x32, 0x44, 0x33, 0x36, 0x39, 0x35, 0x41, 0x32, 0x36, 0x42, 0x32, 0x44, 
		0x43, 0x39, 0x33, 0x38, 0x32, 0x44, 0x34, 0x30, 0x61, 0x73, 0x6D, 0x63, 0x76, 0x63, 0x30, 0x32
	};

	//修改返回的数据
	memcpy(lpBuffers->buf,data,sizeof(data));

	CString strText;
	CString strTemp;
	for ( int i=0; i<dwBufferCount; ++i ){
		strText.Empty();
		for ( int j=0; j<lpBuffers->len; ++j ){
			strTemp.Format(_T("%02X"),lpBuffers->buf[j]);
			strText+=strTemp;
		}

		strTemp.Format(_T("第%d行：%s\n"),i,strText);
		TRACE(strTemp);
	}


	//这里不再hook
	//theApp.Hook2.Change();
	return Result;
}


BOOL __stdcall newCreateProcess(
						 LPCTSTR lpApplicationName,
						 LPTSTR lpCommandLine,
						 LPSECURITY_ATTRIBUTES lpProcessAttributes,
						 LPSECURITY_ATTRIBUTES lpThreadAttributes,
						 BOOL bInheritHandles,
						 DWORD dwCreationFlags,
						 LPVOID lpEnvironment,
						 LPCTSTR lpCurrentDirectory,
						 LPSTARTUPINFO lpStartupInfo,
						 LPPROCESS_INFORMATION lpProcessInformation
						)
{
	int Result;
	typedef  int (__stdcall*TCreateProcess)(
		LPCTSTR lpApplicationName,
		LPTSTR lpCommandLine,
		LPSECURITY_ATTRIBUTES lpProcessAttributes,
		LPSECURITY_ATTRIBUTES lpThreadAttributes,
		BOOL bInheritHandles,
		DWORD dwCreationFlags,
		LPVOID lpEnvironment,
		LPCTSTR lpCurrentDirectory,
		LPSTARTUPINFO lpStartupInfo,
		LPPROCESS_INFORMATION lpProcessInformation
		);
	__asm
	{
		pushad
	}
	MessageBox(NULL,lpCommandLine,lpApplicationName,MB_OK);
	theApp.m_hkCreateProcess.Restore();
	__asm
	{
		popad
	}
	Result=TCreateProcess(theApp.m_hkCreateProcess.OldFunction)(lpApplicationName,lpCommandLine,lpProcessAttributes,lpThreadAttributes,
		bInheritHandles,dwCreationFlags,lpEnvironment,lpCurrentDirectory,lpStartupInfo,lpProcessInformation);

	theApp.m_hkCreateProcess.Change();
	return Result;
}


// CHookDllApp construction

CHookDllApp::CHookDllApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CHookDllApp object

CHookDllApp theApp;


// CHookDllApp initialization

BOOL CHookDllApp::InitInstance()
{
	CWinApp::InitInstance();

	//////////////////////////////////////////////////////////////////////////

	hMapFile=OpenFileMapping(FILE_MAP_WRITE,FALSE,MappingFileName);
	if (hMapFile==NULL){
		hMapFile=CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,sizeof(SHAREMEM),MappingFileName);
	}else{
		//可以打开映像说明这一次是注射到了目标程序中
		TCHAR szBuff[MAX_PATH*2];
		GetModuleFileName(this->m_hInstance,szBuff,sizeof(szBuff));
		StrRChrI(szBuff,NULL,'\\')[1]=0;

		///////////////////////////////////////////////
		//指定进程才注入
		//TCHAR szProcessName[MAX_PATH]={0};
		//GetModuleFileName(NULL,szProcessName,_countof(szProcessName));
		//if ( StrStrI(szProcessName,_T("HProtect.exe"))==NULL ){
		//	return FALSE;
		//}
		///////////////////////////////////////////////

		TRACE0("成功注入到目标进程!\n");
		//Hook.Create(recv,NewRecv);
		//Hook.Change();
		//Hook2.Create(WSARecv,NewWSARecv);
		//Hook2.Change();
		//m_hkCreateProcess.Create(CreateProcess,newCreateProcess);
		//m_hkCreateProcess.Change();

		m_pdlgMain=new CMainDlg;
		m_pdlgMain->Create(CMainDlg::IDD,AfxGetMainWnd());
		m_pdlgMain->ShowWindow(SW_SHOW);
	}

	if (hMapFile==NULL){
		::MessageBox(NULL,_T("不能建立共享内存!"),NULL,MB_OK|MB_ICONERROR);
	}

	pShareMem=(PSHAREMEM)MapViewOfFile(hMapFile,FILE_MAP_WRITE|FILE_MAP_READ,0,0,0);
	if (pShareMem==NULL){
		CloseHandle(hMapFile);
		::MessageBox(NULL,_T("不能建立共享内存!"),NULL,MB_OK|MB_ICONERROR);
	}

	//////////////////////////////////////////////////////////////////////////

	return TRUE;
}


int CHookDllApp::ExitInstance()
{
	::SendMessage(HWND(pShareMem->hMainWnd),WM_HOSTEXIT,NULL,NULL);
	if (m_pdlgMain){
		delete m_pdlgMain;
		m_pdlgMain=NULL;
	}

	return CWinApp::ExitInstance();
}


// AntiTestDlg.cpp : implementation file
//
#include   <winable.h> 
#include "stdafx.h"
#include "AntiTest.h"
#include "AntiTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAntiTestDlg dialog

CAntiTestDlg::CAntiTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAntiTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAntiTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAntiTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAntiTestDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAntiTestDlg, CDialog)
	//{{AFX_MSG_MAP(CAntiTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_CheckRemote, OnCheckRemote)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_HardwareBreak, OnHardwareBreak)
	ON_BN_CLICKED(IDC_Title, OnTitle)
	ON_BN_CLICKED(IDC_DEBUGEREVENT, OnDebugerevent)
	ON_BN_CLICKED(IDC_ZwQueryInformationProcess, OnZwQueryInformationProcess)
	ON_BN_CLICKED(IDC_STEP, OnStep)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAntiTestDlg message handlers

BOOL CAntiTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAntiTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAntiTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAntiTestDlg::OnQueryDragIcon()
{

	return (HCURSOR) m_hIcon;
}









///////////////////////////////////////////////////////////////////////
//tls 开始
typedef struct _IMAGE_TLS_DIRECTORY32_ {
	DWORD   StartAddressOfRawData;
	DWORD   EndAddressOfRawData;
	DWORD   AddressOfIndex;
	DWORD   AddressOfCallBacks;
	DWORD   SizeOfZeroFill;
	DWORD   Characteristics;
} IMAGE_TLS_DIRECTORY32_, * PIMAGE_TLS_DIRECTORY32_;
// 添加tls回调函数
VOID NTAPI on_tls_callback(PVOID handle, DWORD reason, PVOID resv)
{
	// Run code here
	switch (reason )
	{
		case  DLL_PROCESS_ATTACH:
		
			{
				
				MessageBox(NULL, TEXT("Thread attach!"), TEXT("TLS Callback"), 0);
				break;
			}

		case DLL_PROCESS_DETACH:
			{
				
				MessageBox(NULL, TEXT("Thread detach!"), TEXT("TLS Callback"), 0);
				break;
			}
		default:
		break;
	};
}


#pragma comment(linker, "/INCLUDE:__tls_used")

extern "C" {
	
#pragma data_seg(".tls")
	__declspec(allocate(".tls"))
		DWORD __tls_start = 0x0;
	
#pragma data_seg(".tls")
	__declspec(allocate(".tls"))
		DWORD __tls_end = 0x0;
	
#pragma data_seg(".tls")
	__declspec(allocate(".tls"))
		DWORD __tls_index = 0x0;
#pragma data_seg(".tls")
	__declspec(allocate(".tls"))
		DWORD __tls_func = (DWORD) on_tls_callback;
#pragma data_seg(".tls")
	__declspec(allocate(".tls"))
		DWORD __tls_size = 0x0;
#pragma data_seg(".tls")
	__declspec(allocate(".tls"))
		DWORD __tls_flag = 0x0;	
}



#pragma data_seg(".rdata$T")
__declspec(allocate(".rdata$T"))
extern "C" const _IMAGE_TLS_DIRECTORY32_ _tls_used =
{
	(DWORD) &__tls_start,
		(DWORD) &__tls_end,
		(DWORD) &__tls_index,
		(DWORD ) &__tls_func,
		(DWORD ) 0,
		(DWORD ) 0
};

//tls.end
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//Exception.start
//在OD里面会把call addr这里出现的异常当成一次中断  所以如果运行了异常处理就说明没有OD
#pragma data_seg(".excep")
BOOL Exceptioni = FALSE;
#pragma data_seg()
#pragma comment(linker, "/section:.excep,RWS") 
LONG WINAPI Exceptionfun(struct _EXCEPTION_POINTERS *ExceptionInfo)
{
		Exceptioni = TRUE;
		return EXCEPTION_CONTINUE_EXECUTION;
}
BOOL IsException()
{
	// TODO: Add your control notification handler code here
	DWORD pw;
	LPTOP_LEVEL_EXCEPTION_FILTER lpsetun;
	lpsetun = ::SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)Exceptionfun);
 	LPVOID addr= ::VirtualAlloc(NULL,0x1000,MEM_COMMIT,PAGE_READWRITE);
	*((PWORD)addr) = 0xc3;
	::VirtualProtect(addr,0x1000,PAGE_EXECUTE_READ|PAGE_GUARD,&pw);
	_asm{
		call addr;
	}
	
	::SetUnhandledExceptionFilter(lpsetun);
	return Exceptioni;
	}
void CAntiTestDlg::OnButton1() 
{
	if (IsException())
	{
		MessageBox("没有发现OD");
	}
	else
	{
		MessageBox("发现OD");
	}
	
}
//Exception.end
//////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//CheckRemoteDebuggerPresent检测方法.start
typedef BOOL (WINAPI *CHECK_REMOTE_DEBUGGER_PRESENT)(HANDLE, PBOOL); //定义函数指针
BOOL Is_CheckRemoteDebuger()
{
	HANDLE      hProcess;
    HINSTANCE   hModule;
	BOOL        bDebuggerPresent = FALSE;
	CHECK_REMOTE_DEBUGGER_PRESENT CheckRemoteDebuggerPresent;
	hModule = GetModuleHandleA("Kernel32");
	CheckRemoteDebuggerPresent = (CHECK_REMOTE_DEBUGGER_PRESENT)GetProcAddress(hModule, "CheckRemoteDebuggerPresent"); 
	hProcess = GetCurrentProcess();
	CheckRemoteDebuggerPresent(hProcess,&bDebuggerPresent);  //调用
	return bDebuggerPresent;
}
void CAntiTestDlg::OnCheckRemote() 
{
	// TODO: Add your control notification handler code here
		if (Is_CheckRemoteDebuger())
		{
			MessageBox("发现OD");
		}
		else
		{
			MessageBox("没有OD");
		}
}


//CheckRemoteDebuggerPresent检测方法.end
///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
//startupinfo结构.start
//原理:Windows操作系统中的explorer.exe创建进程的时候会把STARTUPINFO结构中的值设为0,
//而非explorer.exe创建进程的时候会忽略这个结构中的值，也就是结构中的值不为0，所以可以利用这个来判断OD是否在调试程序.
BOOL Is_startupinfo()
{
	STARTUPINFOA startinfo;
	GetStartupInfo(&startinfo);
	if(startinfo.dwX!=0 || startinfo.dwY!=0 || startinfo.dwXCountChars!=0 || startinfo.dwYCountChars!=0
		|| startinfo.dwFillAttribute!=0 || startinfo.dwXSize!=0 || startinfo.dwYSize!=0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}
void CAntiTestDlg::OnButton3() 
{
	// TODO: Add your control notification handler code here
	if (Is_startupinfo())
	{
		MessageBox("有OD");
	}
	else
	{
		MessageBox("没有OD");
	}
}
//startupinfo结构.end
//////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//检测硬件执行断点.start
#pragma data_seg(".Hardware")
BOOL Hardwarei = FALSE;
DWORD addr;
#pragma data_seg()
#pragma comment(linker, "/section:.Hardware,RWS") 
LONG WINAPI HardwareFun(struct _EXCEPTION_POINTERS *ExceptionInfo)
{
	if (0 != ExceptionInfo->ContextRecord->Dr0 || 0 != ExceptionInfo->ContextRecord->Dr1 || 0 != ExceptionInfo->ContextRecord->Dr2 || 0 != ExceptionInfo->ContextRecord->Dr3)
	{
		Hardwarei = TRUE;
		ExceptionInfo->ContextRecord->Dr0 = 0;
		ExceptionInfo->ContextRecord->Dr1 = 0;
		ExceptionInfo->ContextRecord->Dr2 = 0;
		ExceptionInfo->ContextRecord->Dr3 = 0;
	}
	ExceptionInfo->ContextRecord->Eip = addr; // 设置新的eip 让程序调转到safe执行
	return EXCEPTION_CONTINUE_EXECUTION;
}
BOOL IsHardware()
{
	// TODO: Add your control notification handler code here
	LPTOP_LEVEL_EXCEPTION_FILTER lpsetun;
	lpsetun = ::SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)HardwareFun);
	_asm{
		mov addr,offset safe;
		int 3;
	}
safe:
	::SetUnhandledExceptionFilter(lpsetun);
	return Hardwarei;
}

void CAntiTestDlg::OnHardwareBreak() 
{
	char *p = "aaaaaaaaaaaaaaddddddddddddddbbbbbbbbbbbb";
	// TODO: Add your control notification handler code here
	if (IsHardware())
	{
		MessageBox("检测到硬件执行断点");
	}
	else
	{
		MessageBox("没有检测到硬件执行断点");
	}
}
//检测硬件执行断点.start
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//into指令导致异常 使程序正常执行 OD会当成断点来处理
//这个功能在OD下执行 会导致程序死掉
//into溢出中断 当OF位 是1 的时候 会使用异常处理函数 不是1 的时候 不触发异常处理 在OD中 始终不触发异常处理
#pragma data_seg(".Intobreak")
DWORD addr_into;
#pragma data_seg()
#pragma comment(linker, "/section:.Intobreak,RWS")

LONG WINAPI Intobreak(struct _EXCEPTION_POINTERS *ExceptionInfo)
{
	ExceptionInfo->ContextRecord->Eip = addr_into; // 设置新的eip 让程序调转到safe执行
	return EXCEPTION_CONTINUE_EXECUTION;
}
void Is_IntoBreak()
{
	LPTOP_LEVEL_EXCEPTION_FILTER lpsetun;
	lpsetun = ::SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)Intobreak);
	_asm{
		mov addr_into,offset Intosafe;存放异常处理之后的偏移地址
		mov ecx,1
	}
here:
	_asm{
		rol ecx,1
		into
		jmp here
	}
Intosafe:
	::SetUnhandledExceptionFilter(lpsetun);
}
void CAntiTestDlg::OnTitle() 
{
	Is_IntoBreak();
	// TODO: Add your control notification handler code here
	
}
//into指令异常.end  
/////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
//禁止程序产生调试事件.start
typedef enum _THREADINFOCLASS {
	ThreadBasicInformation, // 0 Y N
		ThreadTimes, // 1 Y N
		ThreadPriority, // 2 N Y
		ThreadBasePriority, // 3 N Y
		ThreadAffinityMask, // 4 N Y
		ThreadImpersonationToken, // 5 N Y
		ThreadDescriptorTableEntry, // 6 Y N
		ThreadEnableAlignmentFaultFixup, // 7 N Y
		ThreadEventPair, // 8 N Y
		ThreadQuerySetWin32StartAddress, // 9 Y Y
		ThreadZeroTlsCell, // 10 N Y
		ThreadPerformanceCount, // 11 Y N
		ThreadAmILastThread, // 12 Y N
		ThreadIdealProcessor, // 13 N Y
		ThreadPriorityBoost, // 14 Y Y
		ThreadSetTlsArrayAddress, // 15 N Y
		ThreadIsIoPending, // 16 Y N
		ThreadHideFromDebugger // 17 N Y
} THREAD_INFO_CLASS;

typedef LPARAM (NTAPI *ZwSetInformationThread)(
												 IN  HANDLE 						ThreadHandle,
												 IN  THREAD_INFO_CLASS			ThreadInformaitonClass,
												 IN  PVOID 						ThreadInformation,
												 IN  ULONG 						ThreadInformationLength
												 );

void Is_stopDebug()
{
	HANDLE hwnd;
	HMODULE hModule;
	hwnd=GetCurrentThread();
	hModule=LoadLibrary("ntdll.dll");
	ZwSetInformationThread myFunc;
	myFunc=(ZwSetInformationThread)GetProcAddress(hModule,"ZwSetInformationThread");
	myFunc(hwnd,ThreadHideFromDebugger,NULL,NULL);//只有单步跟踪的时候才测试出这个的效果 如果不单步跟踪 不会有效果 很费解
}

void CAntiTestDlg::OnDebugerevent() 
{
		// TODO: Add your control notification handler code here
	
	Is_stopDebug();
}

//.end
///////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//ZwQueryInformationProcess.start
// 为了检测调试器的存在，需要将ProcessInformationclass参数设为ProcessDebugPort(7)。
// NtQueryInformationProcess()检索内核结构EPROCESS5的DebugPort成员，这个成员是系统用来与调试器通信的端口句柄。
//非0的DebugPort成员意味着进程正在被用户模式的调试器调试。如果是这样的话，ProcessInformation 将被置为0xFFFFFFFF ，
//否则ProcessInformation 将被置为0。

typedef enum _PROCESSINFOCLASS {
	ProcessBasicInformation, // 0 Y N
		ProcessQuotaLimits, // 1 Y Y
		ProcessIoCounters, // 2 Y N
		ProcessVmCounters, // 3 Y N
		ProcessTimes, // 4 Y N
		ProcessBasePriority, // 5 N Y
		ProcessRaisePriority, // 6 N Y
		ProcessDebugPort, // 7 Y Y
		ProcessExceptionPort, // 8 N Y
		ProcessAccessToken, // 9 N Y
		ProcessLdtInformation, // 10 Y Y
		ProcessLdtSize, // 11 N Y
		ProcessDefaultHardErrorMode, // 12 Y Y
		ProcessIoPortHandlers, // 13 N Y
		ProcessPooledUsageAndLimits, // 14 Y N
		ProcessWorkingSetWatch, // 15 Y Y
		ProcessUserModeIOPL, // 16 N Y
		ProcessEnableAlignmentFaultFixup, // 17 N Y
		ProcessPriorityClass, // 18 N Y
		ProcessWx86Information, // 19 Y N
		ProcessHandleCount, // 20 Y N
		ProcessAffinityMask, // 21 N Y
		ProcessPriorityBoost, // 22 Y Y
		ProcessDeviceMap,// 23 Y Y
		ProcessSessionInformation, // 24 Y Y
		ProcessForegroundInformation, // 25 N Y
		ProcessWow64Information // 26 Y N
} PROCESSINFOCLASS;

ULONG ZwQueryInformationProcess(
						  IN HANDLE ProcessHandle,
						  IN PROCESSINFOCLASS ProcessInformationClass,
						  OUT PVOID ProcessInformation,
						  IN ULONG ProcessInformationLength,
						  OUT PULONG ReturnLength OPTIONAL
						  );
//********************************************************
typedef LPARAM (_stdcall *ZW_QUERY_INFORMATION_PROCESS)(
														  HANDLE ProcessHandle,
														  PROCESSINFOCLASS ProcessInformationClass, 
														  PVOID ProcessInformation,
														  ULONG ProcessInformationLength,
														  PULONG ReturnLength
														  ); //定义函数指针
BOOL Is_ZW_QUERY_INFORMATION()
{
	HANDLE      hProcess;
    HINSTANCE   hModule;
    DWORD       dwResult;
    ZW_QUERY_INFORMATION_PROCESS MyFunc;
    hModule = GetModuleHandle("ntdll.dll");
    MyFunc=(ZW_QUERY_INFORMATION_PROCESS)GetProcAddress(hModule,"ZwQueryInformationProcess");
    hProcess = GetCurrentProcess();
    MyFunc(hProcess,ProcessDebugPort,&dwResult,4,NULL);
	if(dwResult!=0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}


void CAntiTestDlg::OnZwQueryInformationProcess() 
{
	// TODO: Add your control notification handler code here
	if (Is_ZW_QUERY_INFORMATION())
	{
		MessageBox("发现OD");
	}
	else
	{
		MessageBox("没有检测到OD");
	}
	
}
//ZwQueryInformationProcess.end
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//单步异常检测.start
//这种方法需要下面的汇编代码在单步跟中的时候才有效 ， 不是很实用的 检测方式 看用在什么地方了
void CAntiTestDlg::OnStep() 
{
	// TODO: Add your control notification handler code here
	try{
		_asm{					
				pushfd					 //触发单步异常
				or      dword ptr [esp],100h   ;TF=1
				popfd
		}
		AfxMessageBox("检测到OD");
	}catch(...){
		AfxMessageBox("没有OD");	
	}
}
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//

//DEL void CAntiTestDlg::OnBlock() 
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL 	BlockInput(TRUE);
//DEL }



//make by kkmylove QQ:417438592

// HookTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HookTest.h"
#include "HookTestDlg.h"
#include "Tlhelp32.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHookTestDlg 对话框




CHookTestDlg::CHookTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHookTestDlg::IDD, pParent)
	, m_szClassName(_T(""))
	, m_nHandle(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	HMODULE hModule=LoadLibrary("Hook.dll");

	m_bIsDrag		= FALSE;
	m_bMouseDown	= FALSE;
	m_hIconOrig		= NULL;
	m_hIconEmpty	= NULL;
	m_hWndLastFocus	= NULL;
}

void CHookTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FINDER, m_stcFinder);
	DDX_Text(pDX, IDC_EDIT_CLASS_NAME, m_szClassName);
	DDX_Text(pDX, IDC_EDIT_HANDLE, m_nHandle);
	DDX_Control(pDX, IDC_EDIT_DLLPATH, m_edtDllPath);
}

BEGIN_MESSAGE_MAP(CHookTestDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_HOOK, &CHookTestDlg::OnBnClickedButtonHook)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


void EnableDebugPriv( void )
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;

	OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,&tkp.Privileges[0].Luid);
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,(PTOKEN_PRIVILEGES)NULL, 0);
	CloseHandle( hToken );
}

// CHookTestDlg 消息处理程序

BOOL CHookTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	/************************************************************************/
	m_hIconOrig  = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ORIG));
	m_hIconEmpty = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_EMPTY));
	m_stcFinder.GetWindowRect(&m_rcFinder);
	ScreenToClient(&m_rcFinder);

	EnableDebugPriv();
	/************************************************************************/
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CHookTestDlg::OnPaint()
{
	CDialog::OnPaint();
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CHookTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//由进程ID获取相应的主线程ID
DWORD GetThreadIdFromPID(DWORD dwProcessId)
{
	HANDLE ThreadHandle;
	THREADENTRY32 ThreadStruct;

	ThreadHandle=CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,dwProcessId);
	ThreadStruct.dwSize=sizeof(ThreadStruct);
	if(Thread32First(ThreadHandle,&ThreadStruct))
	{
		do 
		{
			if (ThreadStruct.th32OwnerProcessID==dwProcessId)
			{
				CloseHandle(ThreadHandle);
				return ThreadStruct.th32ThreadID;
			}
		} while (Thread32Next(ThreadHandle,&ThreadStruct));
	}
	CloseHandle(ThreadHandle);
	return 0;
}

/************************************************************************/
typedef enum _THREADINFOCLASS{  
	ThreadBasicInformation,  
	ThreadTimes,  
	ThreadPriority,  
	ThreadBasePriority,  
	ThreadAffinityMask,  
	ThreadImpersonationToken,  
	ThreadDescriptorTableEntry,  
	ThreadEnableAlignmentFaultFixup,  
	ThreadEventPair_Reusable,  
	ThreadQuerySetWin32StartAddress,  
	ThreadZeroTlsCell,  
	ThreadPerformanceCount,  
	ThreadAmILastThread,  
	ThreadIdealProcessor,  
	ThreadPriorityBoost,  
	ThreadSetTlsArrayAddress,  
	ThreadIsIoPending,  
	ThreadHideFromDebugger,  
	ThreadBreakOnTermination,  
	MaxThreadInfoClass  
}THREADINFOCLASS;  

typedef struct _CLIENT_ID{  
	HANDLE   UniqueProcess;  
	HANDLE   UniqueThread;  
}CLIENT_ID,*PCLIENT_ID;  

typedef struct _THREAD_BASIC_INFORMATION{   //   Information   Class   0  
	LONG		ExitStatus;  
	PVOID		TebBaseAddress;  
	CLIENT_ID   ClientId;  
	LONG		AffinityMask;  
	LONG		Priority;  
	LONG		BasePriority;  
}THREAD_BASIC_INFORMATION,*PTHREAD_BASIC_INFORMATION;  

typedef LONG (__stdcall*_pfnZwQueryInformationThread)(
	IN  HANDLE			ThreadHandle,  
	IN  THREADINFOCLASS	ThreadInformationClass,  
	OUT PVOID			ThreadInformation,  
	IN  ULONG			ThreadInformationLength,  
	OUT PULONG			ReturnLength OPTIONAL  
	); 
typedef LONG (__stdcall*_pfnRtlNtStatusToDosError)(IN ULONG status);
/************************************************************************/
typedef enum _MEMORY_INFORMATION_CLASS{
	MemoryBasicInformation,
	MemoryWorkingSetInformation,
	MemoryMappedFileNameInformation,
	MemoryRegionInformation,
	MemoryWorkingSetExInformation
}MEMORY_INFORMATION_CLASS,*PMEMORY_INFORMATION_CLASS;

typedef ULONG(WINAPI*_pfnZwQueryVirtualMemory)(
	HANDLE hProcess,PVOID BaseAddress,
	MEMORY_INFORMATION_CLASS MemoryInformationClass,
	PVOID MemoryInformation,SIZE_T MemoryInformationLength,
	PSIZE_T ReturnLength
	);
/************************************************************************/

//F:\VS工程\DllDlg\debug\DllDlg.dll
/************************************************************************/
int InjectDll( DWORD dwProcessID,char*szDllPath)
{
	HANDLE hProcess=OpenProcess(PROCESS_ALL_ACCESS,false,dwProcessID);
	DWORD dwThreadId=GetThreadIdFromPID(dwProcessID);
	HANDLE hThread=OpenThread(THREAD_ALL_ACCESS,FALSE,dwThreadId);

	HINSTANCE		hNTDLL;

	hNTDLL=GetModuleHandle("ntdll");  
	_pfnZwQueryInformationThread ZwQueryInformationThread=(_pfnZwQueryInformationThread) 
		GetProcAddress(hNTDLL,"ZwQueryInformationThread");  

	PVOID startaddr; 
	LONG status=ZwQueryInformationThread(hThread,
		ThreadQuerySetWin32StartAddress,&startaddr,sizeof(startaddr),NULL);
	SuspendThread(hThread);

	CHAR szLibPath[MAX_PATH]={0};
	CHAR szOrgBin[MAX_PATH];
	DWORD dwReadWritten;
	lstrcpy(szLibPath,szDllPath);

	::ReadProcessMemory(hProcess,startaddr,szOrgBin,sizeof(szOrgBin),&dwReadWritten);
	::WriteProcessMemory(hProcess, startaddr, (void*)szLibPath,sizeof(szLibPath),&dwReadWritten);
	HANDLE hRemoteThread = ::CreateRemoteThread( hProcess, NULL, 0,	
		(LPTHREAD_START_ROUTINE)LoadLibraryA, //::GetProcAddress(hKernel32,"LoadLibraryA")
		startaddr, 0, NULL );
	CloseHandle(hRemoteThread);
	
	::WriteProcessMemory(hProcess, startaddr, (void*)szOrgBin,sizeof(szOrgBin),&dwReadWritten);
	
	ResumeThread(hThread);
	
	return 0;
}
/************************************************************************/

void CHookTestDlg::OnBnClickedButtonHook()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_nHandle)
	{
		DWORD cpid;

		GetWindowThreadProcessId((HWND)m_nHandle,&cpid);
		m_hProcess=OpenProcess(PROCESS_ALL_ACCESS,false,cpid);
		m_dwThreadId=GetThreadIdFromPID(cpid);
		if (m_dwThreadId == 0)
		{
			::MessageBox(NULL,"没有选定目标！",NULL,MB_OK|MB_ICONERROR);
		}
		else
		{
			CString strText;
			m_edtDllPath.GetWindowText(strText);
			if (GetFileAttributes(strText)!=-1)
			{
				InjectDll(cpid,(char*)(LPCTSTR)strText);
			}
		}
	}
}

void CHookTestDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_rcFinder.PtInRect(point))
	{
		m_bMouseDown = TRUE;	
		SetCapture();
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CHookTestDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_bIsDrag)
	{
		ReleaseCapture();
		m_bIsDrag = FALSE;
		m_stcFinder.SetIcon(m_hIconOrig);
		if(m_hWndLastFocus)
		{
			::InvalidateRect(NULL,NULL,TRUE);
			m_hWndLastFocus = NULL;
		}
	}
	m_bMouseDown = FALSE;

	CDialog::OnLButtonUp(nFlags, point);
}

void CHookTestDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_bMouseDown)
	{
		::SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_DRAG)));
		m_stcFinder.SetIcon(m_hIconEmpty);
		m_bIsDrag = TRUE;

		//get the window
		CPoint ptCursor(point);
		ClientToScreen(&ptCursor);
		HiliTheWindow(ptCursor);
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CHookTestDlg::HiliTheWindow(CPoint point)
{
	HWND hWnd = ::WindowFromPoint(point);
	if(!hWnd) return;
	DWORD dwProcess = 0;
	GetWindowThreadProcessId(hWnd,&dwProcess);
	if(dwProcess == GetCurrentProcessId()) 
		return;

	GetClassName(hWnd,m_szClassName.GetBuffer(128),128);
	m_szClassName.ReleaseBuffer();
	m_nHandle=(int)hWnd;

	UpdateData(FALSE);

	HDC hdc = ::GetWindowDC(hWnd);
	if(hdc)
	{
		if(m_hWndLastFocus && m_hWndLastFocus != hWnd)
			::InvalidateRect(m_hWndLastFocus,NULL,TRUE); //::RedrawWindow(m_hWndLastFocus,NULL,NULL,RDW_FRAME|RDW_ERASE|RDW_UPDATENOW);
		m_hWndLastFocus = hWnd;

		CRect rcWnd;
		::GetWindowRect(hWnd,rcWnd);
		::MapWindowPoints(NULL,hWnd,(LPPOINT)&rcWnd,2);
		rcWnd.OffsetRect(-rcWnd.left,-rcWnd.top);
		//TRACE2("left %d,top %d,\n",rcWnd.left,rcWnd.top);

		::SelectObject(hdc,::GetStockObject(NULL_BRUSH));
		HPEN hPen = ::CreatePen(PS_SOLID,3,RGB(0,0,0));

		HPEN hPenOld = (HPEN)::SelectObject(hdc,hPen);
		::Rectangle(hdc,rcWnd.left,rcWnd.top,rcWnd.Width(),rcWnd.Height());

		::SelectObject(hdc,hPenOld);
		::DeleteObject(hPenOld);
		::ReleaseDC(hWnd,hdc);
	}
}
// win7adminDlg.cpp : 实现文件
//


#include "stdafx.h"
#include "win7admin.h"
#include "win7adminDlg.h"
#include <Psapi.h>

#pragma comment(lib,"Psapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// Cwin7adminDlg 对话框




Cwin7adminDlg::Cwin7adminDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Cwin7adminDlg::IDD, pParent)
	, m_nPID(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cwin7adminDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_nPID);
}

BEGIN_MESSAGE_MAP(Cwin7adminDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &Cwin7adminDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// Cwin7adminDlg 消息处理程序

BOOL Cwin7adminDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Cwin7adminDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Cwin7adminDlg::OnPaint()
{
	CDialog::OnPaint();
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR Cwin7adminDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*------------------------------------------------------------------------
说明: 提高本进程权限,以取得系统进程的信息
------------------------------------------------------------------------*/
BOOL WINAPI EnableDebugPrivilege(BOOL bEnable) 
{
	// 附给本进程特权，用以取得系统进程的信息
	BOOL bOk = FALSE;    // Assume function fails
	HANDLE hToken;

	// 打开一个进程的访问令牌
	if (OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, 
		&hToken)) 
	{
		TOKEN_PRIVILEGES tp;
		tp.PrivilegeCount = 1;
		LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);
		tp.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;
		AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
		bOk = (GetLastError() == ERROR_SUCCESS);
		::CloseHandle(hToken);
	}
	return(bOk);
}

void ReadMemory(DWORD dwProcessId)
{
	CString strText;
	TCHAR szFilePath[MAX_PATH]={0};
	DWORD dwBuffer=123456;
	DWORD dwRead=0;
	HANDLE hProcess=OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwProcessId);
	if ( hProcess!=NULL ){
		//ReadProcessMemory(hProcess,(PBYTE)NULL+0x0047C9D4,&dwBuffer,sizeof(dwBuffer), &dwRead);
		//strText.Format("%d",dwBuffer);
		AfxMessageBox("只读方式打开进程成功");
		if ( GetModuleFileNameEx(hProcess,NULL,szFilePath,MAX_PATH)!=0 ){
			AfxMessageBox(szFilePath);
		}else{
			AfxMessageBox("GetModuleFileNameEx获取进程路径失败,使用GetProcessImageFileName获取");
			CloseHandle(hProcess);
			hProcess=OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwProcessId);
			if ( GetProcessImageFileName(hProcess,szFilePath,MAX_PATH) ){
				AfxMessageBox(szFilePath);
			}else{
				AfxMessageBox("GetProcessImageFileName获取进程路径失败");
			}
		}
	}else{
		AfxMessageBox("只读方式打开进程失败");
	}
}

//http://blog.csdn.net/abcpanpeng/archive/2009/09/15/4556374.aspx
void Cwin7adminDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	ReadMemory(m_nPID);
	EnableDebugPrivilege(TRUE);
	ReadMemory(m_nPID);

}

// LoaderDlg.cpp : 实现文件
//

#include "stdafx.h"
#include <Psapi.h>
#include "Loader.h"
#include "LoaderDlg.h"
#include "../../../lib/StarLib/Common/common.h"


#pragma comment(lib,"Psapi.lib")
#pragma comment(linker, "/SECTION:.text,REW" ) //设PE节：.text,可读可执行

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLoaderDlg 对话框




CLoaderDlg::CLoaderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoaderDlg::IDD, pParent)
	, m_strDllFile(_T(""))
	, m_dwPID(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLoaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_strDllFile);
	DDX_Text(pDX, IDC_EDIT1, m_dwPID);
}

BEGIN_MESSAGE_MAP(CLoaderDlg, CDialog)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CLoaderDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CLoaderDlg 消息处理程序

BOOL CLoaderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_strDllFile=Star::Common::GetStartPath()+_T("inject.dll");
	UpdateData(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


void CLoaderDlg::OnBnClickedOk()
{
	UpdateData();

	TCHAR szDllFile[MAX_PATH]={0};
	DWORD dwSize=m_strDllFile.GetLength()+1;
	lstrcpy(szDllFile,m_strDllFile);

	HANDLE hRemoteProcess=OpenProcess(PROCESS_CREATE_THREAD|PROCESS_VM_OPERATION|PROCESS_VM_WRITE,FALSE,m_dwPID);
	LPVOID lpszRemoteDllName=(PWSTR)VirtualAllocEx(hRemoteProcess,NULL,dwSize,MEM_COMMIT,PAGE_READWRITE);
	BOOL iReturnCode=WriteProcessMemory(hRemoteProcess,lpszRemoteDllName,(PVOID)szDllFile,dwSize,NULL);

	HANDLE hRemoteThread=CreateRemoteThread(hRemoteProcess,NULL,0,(LPTHREAD_START_ROUTINE)LoadLibraryA,lpszRemoteDllName,0,NULL);

	//等待远程线程退出     
	WaitForSingleObject(hRemoteThread,INFINITE);     

	if( lpszRemoteDllName!=NULL){     
		VirtualFreeEx(hRemoteProcess, lpszRemoteDllName, 0, MEM_RELEASE);
	}

	if( hRemoteThread!=NULL ){
		CloseHandle(hRemoteThread);   
	}
	if( hRemoteProcess!=NULL ){
		CloseHandle(hRemoteProcess);    
	}

	//AfxMessageBox("dll注入成功，我也可以退出了");
	//OnOK();
}
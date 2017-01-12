// LoadDllDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LoadDll.h"
#include "LoadDllDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLoadDllDlg 对话框




CLoadDllDlg::CLoadDllDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoadDllDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLoadDllDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLoadDllDlg, CDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CLoadDllDlg 消息处理程序

BOOL CLoadDllDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	////////////////////////////////////////////////////////
	LPCWSTR lpCmdLine=(LPCWSTR)GetCommandLineW();

	int nArgs;
	LPWSTR *cmdLineArr=CommandLineToArgvW(lpCmdLine,&nArgs);
	if ( nArgs>=2 ){
		LoadLibraryW(cmdLineArr[1]);
	}
	////////////////////////////////////////////////////////
	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

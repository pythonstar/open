// MainDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HookDll.h"
#include "MainDlg.h"


// CMainDlg 对话框

IMPLEMENT_DYNAMIC(CMainDlg, CDialog)

CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
{

}

CMainDlg::~CMainDlg()
{
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CMainDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CMainDlg 消息处理程序

void CMainDlg::OnBnClickedOk()
{
	//char szCode[]={"\0xB8\0x00\0x04\0x00\0x00\0x89\0x45\0xF8\0x90\0x90\0x90\0x90\0x90\0x90\0x90\0x90"};
	//memcpy( PBYTE(NULL)+0x0047278C,szCode,sizeof(szCode) );

	//char szCode[]={"\0xB8\0x40\0x01\0x00\0x00"};
	//memcpy( PBYTE(NULL)+0x00472787,szCode,sizeof(szCode) );
	::MessageBox(0,_T("patched OK"),0,0);
	//AfxBeginThread(TheadDetectPlug,NULL);
}



UINT CMainDlg::TheadDetectPlug(LPVOID lParam)
{
	//TRACE0("\n");

	return 0;
}
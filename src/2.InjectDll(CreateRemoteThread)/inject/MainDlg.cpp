// MainDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "inject.h"
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
END_MESSAGE_MAP()


// CMainDlg 消息处理程序
BOOL CMainDlg::OnInitDialog()
{
	__super::OnInitDialog();
	

	return TRUE;
}
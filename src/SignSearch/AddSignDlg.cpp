// AddSignDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SignSearch.h"
#include "AddSignDlg.h"


// CAddSignDlg 对话框

IMPLEMENT_DYNAMIC(CAddSignDlg, CDialog)

CAddSignDlg::CAddSignDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddSignDlg::IDD, pParent)
	, m_strSigns(_T(""))
{

}

CAddSignDlg::~CAddSignDlg()
{
}

void CAddSignDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SIGNS, m_strSigns);
}


BEGIN_MESSAGE_MAP(CAddSignDlg, CDialog)
END_MESSAGE_MAP()


// CAddSignDlg 消息处理程序

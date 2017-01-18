// FindTypeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DuplicateFileFinder.h"
#include "FindTypeDlg.h"


// CFindTypeDlg 对话框

IMPLEMENT_DYNAMIC(CFindTypeDlg, CDialog)

CFindTypeDlg::CFindTypeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFindTypeDlg::IDD, pParent)
	, m_strFindTypeDesc(_T(""))
	, m_strFindTypeFilter(_T(""))
{

}

CFindTypeDlg::~CFindTypeDlg()
{
}

void CFindTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strFindTypeDesc);
	DDX_Text(pDX, IDC_EDIT2, m_strFindTypeFilter);
}


BEGIN_MESSAGE_MAP(CFindTypeDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CFindTypeDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CFindTypeDlg 消息处理程序

void CFindTypeDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	if ( m_strFindTypeDesc.Find('|')!=-1 ){
		AfxMessageBox("“查找方案名称”中不要包含字符|");
		return;
	}

	OnOK();
}

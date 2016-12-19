// UpdateSignDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SignSearch.h"
#include "UpdateSignDlg.h"


// CUpdateSignDlg 对话框

IMPLEMENT_DYNAMIC(CUpdateSignDlg, CDialog)

CUpdateSignDlg::CUpdateSignDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUpdateSignDlg::IDD, pParent)
	, m_strSearchBase(_T(""))
	, m_strStartRva(_T(""))
	, m_strEndRva(_T(""))
	, m_strSign(_T(""))
{

}

CUpdateSignDlg::~CUpdateSignDlg()
{
}

void CUpdateSignDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SEARCHBASE, m_strSearchBase);
	DDX_Text(pDX, IDC_EDIT_STARTRVA, m_strStartRva);
	DDX_Text(pDX, IDC_EDIT_ENDRVA, m_strEndRva);
	DDX_Text(pDX, IDC_EDIT_SIGN, m_strSign);
}


BEGIN_MESSAGE_MAP(CUpdateSignDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CUpdateSignDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CUpdateSignDlg 消息处理程序

void CUpdateSignDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	m_strSearchBase.Trim();
	m_strStartRva.Trim();
	m_strEndRva.Trim();
	m_strSign.Trim();

	UpdateData(FALSE);
	
	OnOK();
}

// AddSignDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SignSearch.h"
#include "AddSignDlg.h"


// CAddSignDlg �Ի���

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


// CAddSignDlg ��Ϣ�������

// MainDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "inject.h"
#include "MainDlg.h"


// CMainDlg �Ի���

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


// CMainDlg ��Ϣ�������
BOOL CMainDlg::OnInitDialog()
{
	__super::OnInitDialog();
	

	return TRUE;
}
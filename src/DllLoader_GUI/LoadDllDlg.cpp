// LoadDllDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LoadDll.h"
#include "LoadDllDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLoadDllDlg �Ի���




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


// CLoadDllDlg ��Ϣ�������

BOOL CLoadDllDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	////////////////////////////////////////////////////////
	LPCWSTR lpCmdLine=(LPCWSTR)GetCommandLineW();

	int nArgs;
	LPWSTR *cmdLineArr=CommandLineToArgvW(lpCmdLine,&nArgs);
	if ( nArgs>=2 ){
		LoadLibraryW(cmdLineArr[1]);
	}
	////////////////////////////////////////////////////////
	

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

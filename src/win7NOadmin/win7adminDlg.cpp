// win7adminDlg.cpp : ʵ���ļ�
//


#include "stdafx.h"
#include "win7admin.h"
#include "win7adminDlg.h"
#include <Psapi.h>

#pragma comment(lib,"Psapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// Cwin7adminDlg �Ի���




Cwin7adminDlg::Cwin7adminDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Cwin7adminDlg::IDD, pParent)
	, m_nPID(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cwin7adminDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_nPID);
}

BEGIN_MESSAGE_MAP(Cwin7adminDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &Cwin7adminDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// Cwin7adminDlg ��Ϣ�������

BOOL Cwin7adminDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void Cwin7adminDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Cwin7adminDlg::OnPaint()
{
	CDialog::OnPaint();
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR Cwin7adminDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*------------------------------------------------------------------------
˵��: ��߱�����Ȩ��,��ȡ��ϵͳ���̵���Ϣ
------------------------------------------------------------------------*/
BOOL WINAPI EnableDebugPrivilege(BOOL bEnable) 
{
	// ������������Ȩ������ȡ��ϵͳ���̵���Ϣ
	BOOL bOk = FALSE;    // Assume function fails
	HANDLE hToken;

	// ��һ�����̵ķ�������
	if (OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, 
		&hToken)) 
	{
		TOKEN_PRIVILEGES tp;
		tp.PrivilegeCount = 1;
		LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);
		tp.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;
		AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
		bOk = (GetLastError() == ERROR_SUCCESS);
		::CloseHandle(hToken);
	}
	return(bOk);
}

void ReadMemory(DWORD dwProcessId)
{
	CString strText;
	TCHAR szFilePath[MAX_PATH]={0};
	DWORD dwBuffer=123456;
	DWORD dwRead=0;
	HANDLE hProcess=OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwProcessId);
	if ( hProcess!=NULL ){
		//ReadProcessMemory(hProcess,(PBYTE)NULL+0x0047C9D4,&dwBuffer,sizeof(dwBuffer), &dwRead);
		//strText.Format("%d",dwBuffer);
		AfxMessageBox("ֻ����ʽ�򿪽��̳ɹ�");
		if ( GetModuleFileNameEx(hProcess,NULL,szFilePath,MAX_PATH)!=0 ){
			AfxMessageBox(szFilePath);
		}else{
			AfxMessageBox("GetModuleFileNameEx��ȡ����·��ʧ��,ʹ��GetProcessImageFileName��ȡ");
			CloseHandle(hProcess);
			hProcess=OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwProcessId);
			if ( GetProcessImageFileName(hProcess,szFilePath,MAX_PATH) ){
				AfxMessageBox(szFilePath);
			}else{
				AfxMessageBox("GetProcessImageFileName��ȡ����·��ʧ��");
			}
		}
	}else{
		AfxMessageBox("ֻ����ʽ�򿪽���ʧ��");
	}
}

//http://blog.csdn.net/abcpanpeng/archive/2009/09/15/4556374.aspx
void Cwin7adminDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	ReadMemory(m_nPID);
	EnableDebugPrivilege(TRUE);
	ReadMemory(m_nPID);

}

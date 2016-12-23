// LoaderDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include <Psapi.h>
#include "Loader.h"
#include "LoaderDlg.h"
#include "../../../lib/StarLib/Common/common.h"


#pragma comment(lib,"Psapi.lib")
#pragma comment(linker, "/SECTION:.text,REW" ) //��PE�ڣ�.text,�ɶ���ִ��

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLoaderDlg �Ի���




CLoaderDlg::CLoaderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoaderDlg::IDD, pParent)
	, m_strDllFile(_T(""))
	, m_dwPID(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLoaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_strDllFile);
	DDX_Text(pDX, IDC_EDIT1, m_dwPID);
}

BEGIN_MESSAGE_MAP(CLoaderDlg, CDialog)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CLoaderDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CLoaderDlg ��Ϣ�������

BOOL CLoaderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_strDllFile=Star::Common::GetStartPath()+_T("inject.dll");
	UpdateData(FALSE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}


void CLoaderDlg::OnBnClickedOk()
{
	UpdateData();

	TCHAR szDllFile[MAX_PATH]={0};
	DWORD dwSize=m_strDllFile.GetLength()+1;
	lstrcpy(szDllFile,m_strDllFile);

	HANDLE hRemoteProcess=OpenProcess(PROCESS_CREATE_THREAD|PROCESS_VM_OPERATION|PROCESS_VM_WRITE,FALSE,m_dwPID);
	LPVOID lpszRemoteDllName=(PWSTR)VirtualAllocEx(hRemoteProcess,NULL,dwSize,MEM_COMMIT,PAGE_READWRITE);
	BOOL iReturnCode=WriteProcessMemory(hRemoteProcess,lpszRemoteDllName,(PVOID)szDllFile,dwSize,NULL);

	HANDLE hRemoteThread=CreateRemoteThread(hRemoteProcess,NULL,0,(LPTHREAD_START_ROUTINE)LoadLibraryA,lpszRemoteDllName,0,NULL);

	//�ȴ�Զ���߳��˳�     
	WaitForSingleObject(hRemoteThread,INFINITE);     

	if( lpszRemoteDllName!=NULL){     
		VirtualFreeEx(hRemoteProcess, lpszRemoteDllName, 0, MEM_RELEASE);
	}

	if( hRemoteThread!=NULL ){
		CloseHandle(hRemoteThread);   
	}
	if( hRemoteProcess!=NULL ){
		CloseHandle(hRemoteProcess);    
	}

	//AfxMessageBox("dllע��ɹ�����Ҳ�����˳���");
	//OnOK();
}
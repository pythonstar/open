// inject.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "inject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

__declspec(dllexport) void __stdcall start()
{
}


// CinjectApp

BEGIN_MESSAGE_MAP(CinjectApp, CWinApp)
END_MESSAGE_MAP()


// CinjectApp ����

CinjectApp::CinjectApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
	m_pdlgMain=NULL;
}


// Ψһ��һ�� CinjectApp ����

CinjectApp theApp;


// CinjectApp ��ʼ��

BOOL CinjectApp::InitInstance()
{
	CWinApp::InitInstance();

	//dllע��ɹ�������ͨ��CreateRemoteThread��ע���dll���ƺ����ܴ������ڣ�����Ƚϱ���
	m_pdlgMain=new CMainDlg;
	m_pdlgMain->Create(CMainDlg::IDD,AfxGetMainWnd());
	m_pdlgMain->ShowWindow(SW_SHOW);

	return TRUE;
}

int CinjectApp::ExitInstance()
{
	if (m_pdlgMain){
		delete m_pdlgMain;
		m_pdlgMain=NULL;
	}

	return CWinApp::ExitInstance();
}
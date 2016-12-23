// inject.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "inject.h"
#include "../ShareMemType.h"

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
	HANDLE hMapFile=OpenFileMapping(FILE_MAP_WRITE,FALSE,MappingFileName);
	if (hMapFile==NULL){
		AfxMessageBox("OpenFileMappingʧ��");
		return;
	}else{
		//���Դ�ӳ��˵����ע�䵽��Ŀ�������
		PSHAREMEM pShareMem=(PSHAREMEM)MapViewOfFile(hMapFile,FILE_MAP_WRITE|FILE_MAP_READ,0,0,0);
		if (pShareMem==NULL){
			CloseHandle(hMapFile);
			AfxMessageBox("MapViewOfFileʧ��");
			return;
		}else{
			
			//�޸�ҳ��������
			if ( !VirtualProtect((LPVOID)pShareMem->pCodeStart, pShareMem->nCodeLen, PAGE_EXECUTE_READWRITE, &pShareMem->dwOldProtect) ){
				AfxMessageBox("VirtualProtectʧ��");
				return;
			}

			//�ָ�ԭ������
			memcpy((LPVOID)pShareMem->pCodeStart,pShareMem->pOrigCode,pShareMem->nCodeLen);

			//�ָ�ҳ��������
			VirtualProtect((LPVOID)pShareMem->pCodeStart, pShareMem->nCodeLen, pShareMem->dwOldProtect, NULL);

			//֪ͨloader���سɹ��ˣ����Թرչ����ڴ���
			::SendMessage(pShareMem->hwndLoader,WM_INJECTSUCCESS,0,0);

			//�رչ����ڴ�
			UnmapViewOfFile(pShareMem);
			CloseHandle(hMapFile);
		}
	}
	
}


// CinjectApp

BEGIN_MESSAGE_MAP(CinjectApp, CWinApp)
END_MESSAGE_MAP()


// CinjectApp ����

CinjectApp::CinjectApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CinjectApp ����

CinjectApp theApp;


// CinjectApp ��ʼ��

BOOL CinjectApp::InitInstance()
{
	CWinApp::InitInstance();

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
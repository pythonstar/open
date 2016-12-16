// Hook.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "Hook.h"

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

HHOOK		hhk;
HANDLE		hMapFile;
PSHAREMEM	pShareMem;
int			nSize;


// CHookApp

BEGIN_MESSAGE_MAP(CHookApp, CWinApp)
END_MESSAGE_MAP()


LRESULT MouseProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	return CallNextHookEx(hhk,nCode,wParam,lParam);
}

__declspec(dllexport) void __stdcall StartHook(HANDLE hMainWnd,DWORD dwThreadId)
{
	pShareMem->hMainWnd=hMainWnd;
	hhk=SetWindowsHookEx(WH_MOUSE,(HOOKPROC)MouseProc,AfxGetInstanceHandle(),dwThreadId);
}
__declspec(dllexport) void __stdcall StopHook()
{
	if (hhk)
	{
		UnhookWindowsHookEx(hhk);
		hhk=NULL;
	}
}

/************************************************************************/
int __stdcall NewMessageBox(HWND hWnd,LPCTSTR lpText,LPCTSTR lpCaption,UINT uType)
{
	int Result;
	typedef  int (__stdcall*TMessageBox)(HWND hWnd,LPCTSTR lpText,LPCTSTR lpCaption,UINT uType);
	__asm
	{
		pushad
	}
	theApp.Hook.Restore();
	__asm
	{
		popad
	}
	Result=TMessageBox(theApp.Hook.OldFunction)(hWnd,lpText,lpCaption,uType);
	MessageBox(0,"O(��_��)O����~","Fucked",0);
	theApp.Hook.Change();
	return Result;
}

DWORD __cdecl NewSendOrder(char*szGame,DWORD dwOrder,char*szMessage)
{
	DWORD dwResult;
	
	__asm
	{
		pushad
	}
	theApp.Hook.Restore();
	__asm
	{
		popad
	}
	dwResult=TSendOrder(theApp.Hook.OldFunction)(szGame,dwOrder,szMessage);
	__asm
	{
		pushad
	}
	CString strText;
	strText.Format("%s\t%d\t%s",szGame,dwOrder,szMessage);
	theApp.m_pdlgMain->AppendText(strText);
	theApp.Hook.Change();
	__asm
	{
		popad
	}
	return dwResult;
}
/************************************************************************/

// CHookApp ����

CHookApp::CHookApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CHookApp ����

CHookApp theApp;

// CHookApp ��ʼ��

BOOL CHookApp::InitInstance()
{
	CWinApp::InitInstance();
	
	hMapFile=OpenFileMapping(FILE_MAP_WRITE,FALSE,MappingFileName);
	if (hMapFile==NULL)
	{
		hMapFile=CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,sizeof(SHAREMEM),MappingFileName);
	}
	else
	{
		//���Դ�ӳ��˵����һ����ע�䵽��Ŀ�������
		//Hook.Create(MessageBox,NewMessageBox);
		Hook.Create((PVOID)ADDR_SEND,NewSendOrder);		
		//Hook.Change();

		char szBuff[MAX_PATH*2];
		GetModuleFileName(this->m_hInstance,szBuff,sizeof(szBuff));
		strrchr(szBuff,'\\')[1]=0;

		m_strDllPath=szBuff;
		//MessageBox(0,m_strDllPath,NULL,0);
		
		m_pdlgMain=new CDlgMain;
		m_pdlgMain->Create(CDlgMain::IDD,AfxGetMainWnd());
		m_pdlgMain->ShowWindow(SW_SHOW);
	}
	if (hMapFile==NULL)
	{
		::MessageBox(NULL,"���ܽ��������ڴ�!",NULL,MB_OK|MB_ICONERROR);
	}
	pShareMem=(PSHAREMEM)MapViewOfFile(hMapFile,FILE_MAP_WRITE|FILE_MAP_READ,0,0,0);
	if (pShareMem==NULL)
	{
		CloseHandle(hMapFile);
		::MessageBox(NULL,"���ܽ��������ڴ�!",NULL,MB_OK|MB_ICONERROR);
	}
	return TRUE;
}

int CHookApp::ExitInstance()
{
	::SendMessage(HWND(pShareMem->hMainWnd),WM_HOSTEXIT,NULL,NULL);
	if (m_pdlgMain)
	{
		delete m_pdlgMain;
		m_pdlgMain=NULL;
	}
	return CWinApp::ExitInstance();
}

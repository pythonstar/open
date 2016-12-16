// Hook.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "Hook.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
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
	MessageBox(0,"O(∩_∩)O哈哈~","Fucked",0);
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

// CHookApp 构造

CHookApp::CHookApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CHookApp 对象

CHookApp theApp;

// CHookApp 初始化

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
		//可以打开映像说明这一次是注射到了目标程序中
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
		::MessageBox(NULL,"不能建立共享内存!",NULL,MB_OK|MB_ICONERROR);
	}
	pShareMem=(PSHAREMEM)MapViewOfFile(hMapFile,FILE_MAP_WRITE|FILE_MAP_READ,0,0,0);
	if (pShareMem==NULL)
	{
		CloseHandle(hMapFile);
		::MessageBox(NULL,"不能建立共享内存!",NULL,MB_OK|MB_ICONERROR);
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

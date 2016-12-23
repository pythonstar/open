// inject.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "inject.h"
#include "../ShareMemType.h"

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

__declspec(dllexport) void __stdcall start()
{
	HANDLE hMapFile=OpenFileMapping(FILE_MAP_WRITE,FALSE,MappingFileName);
	if (hMapFile==NULL){
		AfxMessageBox("OpenFileMapping失败");
		return;
	}else{
		//可以打开映像说明是注射到了目标程序中
		PSHAREMEM pShareMem=(PSHAREMEM)MapViewOfFile(hMapFile,FILE_MAP_WRITE|FILE_MAP_READ,0,0,0);
		if (pShareMem==NULL){
			CloseHandle(hMapFile);
			AfxMessageBox("MapViewOfFile失败");
			return;
		}else{
			
			//修改页保护属性
			if ( !VirtualProtect((LPVOID)pShareMem->pCodeStart, pShareMem->nCodeLen, PAGE_EXECUTE_READWRITE, &pShareMem->dwOldProtect) ){
				AfxMessageBox("VirtualProtect失败");
				return;
			}

			//恢复原来代码
			memcpy((LPVOID)pShareMem->pCodeStart,pShareMem->pOrigCode,pShareMem->nCodeLen);

			//恢复页保护属性
			VirtualProtect((LPVOID)pShareMem->pCodeStart, pShareMem->nCodeLen, pShareMem->dwOldProtect, NULL);

			//通知loader加载成功了，可以关闭共享内存了
			::SendMessage(pShareMem->hwndLoader,WM_INJECTSUCCESS,0,0);

			//关闭共享内存
			UnmapViewOfFile(pShareMem);
			CloseHandle(hMapFile);
		}
	}
	
}


// CinjectApp

BEGIN_MESSAGE_MAP(CinjectApp, CWinApp)
END_MESSAGE_MAP()


// CinjectApp 构造

CinjectApp::CinjectApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CinjectApp 对象

CinjectApp theApp;


// CinjectApp 初始化

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
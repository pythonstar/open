// Hook.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "Hook.h"
#include "../../StarLib/Star.h"

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


// CHookApp

BEGIN_MESSAGE_MAP(CHookApp, CWinApp)
END_MESSAGE_MAP()

HHOOK		hhk;
HANDLE		hMapFile;
PSHAREMEM	g_pShareMem;
int			nSize;
DWORD		g_dwEnterRoomSubAddr;


LRESULT MouseProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	return CallNextHookEx(hhk,nCode,wParam,lParam);
}

__declspec(dllexport) void __stdcall StartHook(HWND hControlWnd,HWND hDummyWnd,DWORD dwThreadId)
{
	g_pShareMem->hControlWnd=hControlWnd;
	g_pShareMem->hDummyWnd=hDummyWnd;
	hhk=SetWindowsHookEx(WH_MOUSE,(HOOKPROC)MouseProc,AfxGetInstanceHandle(),dwThreadId);
}
__declspec(dllexport) void __stdcall StopHook()
{
	if (hhk!=NULL){
		UnhookWindowsHookEx(hhk);
		hhk=NULL;
	}
}

// CHookApp 构造

CHookApp::CHookApp()
{
	m_bCanInstalled=FALSE;
	g_dwEnterRoomSubAddr=0;
	g_pShareMem=NULL;
}


// 唯一的一个 CHookApp 对象

CHookApp theApp;


// CHookApp 初始化

BOOL CHookApp::InitInstance()
{
	CWinApp::InitInstance();

	/************************************************************************/

	hMapFile=OpenFileMapping(FILE_MAP_WRITE,FALSE,MappingFileName);
	if (hMapFile==NULL){
		hMapFile=CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,sizeof(SHAREMEM),MappingFileName);
	}
	else{
		//可以打开映像说明这一次是注射到了目标程序中
		m_bCanInstalled=TRUE;
	}

	if (hMapFile==NULL){
		::MessageBox(NULL,"不能建立共享内存!",NULL,MB_OK|MB_ICONERROR);
		return FALSE;
	}
	g_pShareMem=(PSHAREMEM)MapViewOfFile(hMapFile,FILE_MAP_WRITE|FILE_MAP_READ,0,0,0);
	if (g_pShareMem==NULL){
		CloseHandle(hMapFile);
		::MessageBox(NULL,"不能映射共享内存!",NULL,MB_OK|MB_ICONERROR);
		return FALSE;
	}

	/************************************************************************/

	if ( m_bCanInstalled==TRUE && InitHook()==TRUE){//可以安装钩子并且钩子安装成功
		//::SendMessage(g_pShareMem->hControlWnd,WM_);
		m_pdlgMain=new CDlgHookMain;
		m_pdlgMain->Create(CDlgHookMain::IDD,NULL);
		m_pdlgMain->ShowWindow(SW_SHOW);
		g_pShareMem->hDllWnd=m_pdlgMain->m_hWnd;
	}

	/************************************************************************/

	return TRUE;
}

int CHookApp::ExitInstance()
{
	/************************************************************************/

	::SendMessage(HWND(g_pShareMem->hControlWnd),WM_HOSTEXIT,NULL,NULL);
	if (m_pdlgMain)
	{
		delete m_pdlgMain;
		m_pdlgMain=NULL;
	}
	UnmapViewOfFile(g_pShareMem);
	CloseHandle(hMapFile);

	/************************************************************************/
	return CWinApp::ExitInstance();
}

DWORD		g_dwOrignAddr1;
__declspec(naked) void GetTreeThis()
{
	__asm{
		pushad
		mov g_dwOrignAddr1,edi
	}

	g_pShareMem->dwTreeThis=g_dwOrignAddr1;
	
	__asm{
		mov eax, theApp.m_Hook1.OldFunction
		add eax, 6
		mov g_dwOrignAddr1,eax
	}

	//theApp.m_Hook1.Restore();

	__asm{
		popad
		/***********************************/
		mov     eax, dword ptr [edi+0x70]
		lea     esi, dword ptr [edi+0x70] 
		/***********************************/
		jmp g_dwOrignAddr1
	}

}

DWORD		g_dwOrignAddr2;
__declspec(naked) void GetSkinListThis()
{
	__asm{
		pushad
		mov g_dwOrignAddr2,ecx
	}

	g_pShareMem->dwSkinListThis=g_dwOrignAddr2;
	
	__asm{
		popad
		/***********************************/
		mov     eax, dword ptr [eax+edx*4]  
		ret 4
		/***********************************/
	}
}

BOOL CHookApp::InitHook()
{
	MODULEENTRY32 module;
	PBYTE pAddr=NULL;

	//必须放在g_pShareMem被初始化之后
	g_pShareMem->dwTreeThis=0;
	g_pShareMem->dwSkinListThis=0;

	/************************************************************************
	0147F23F| 74 3D                 | je      short 0147F27E         
	0147F241| 8B47 70               | mov     eax, dword ptr [edi+70] 
	0147F244| 8D77 70               | lea     esi, dword ptr [edi+70]  
	0147F247| 8BCE                  | mov     ecx, esi                  
	0147F249| FF90 88000000         | call    dword ptr [eax+88]      
	0147F24F| 8BD8                  | mov     ebx, eax                       
	0147F251| 8BCF                  | mov     ecx, edi            
	0147F253| 53                    | push    ebx         
	0147F254| E8 8B0E0000           | call    014800E4          
	************************************************************************/

	if( Star::Process::GetModuleEntry(GetCurrentProcessId(),"CommonUI.dll",&module)==TRUE ){
		CSignMatch oneSign;
		oneSign.SetOrigSign("8B47708D77708BCEFF90880000008BD88BCF53E8",0x1000,0x55000);//相对基址的范围
		pAddr=oneSign.SearchSign((PBYTE)0x01440000);	//基址
		if (pAddr!=NULL){
			pAddr-=2;
			HANDLE hProcess=OpenProcess(PROCESS_ALL_ACCESS,FALSE,GetCurrentProcessId());
			DWORD dwOldProtect;
			VirtualProtectEx(hProcess,pAddr,2,PAGE_READWRITE,&dwOldProtect);
			*pAddr=0x90;
			*(pAddr+1)=0x90;
// 			DWORD dwAddr=(DWORD)pAddr + oneSign.m_nSignLen;
// 			g_dwEnterRoomSubAddr = (*(PDWORD)(dwAddr)) + dwAddr + 5 - 1;
// 			m_Hook1.Create(pAddr,GetTreeThis);
// 			m_Hook1.Change();
		}else{
			::MessageBox(0,"第一组特征没有匹配",0,0);
		}
	}


	/************************************************************************
	02E9634A| 8B0490                | mov     eax, dword ptr [eax+edx*4]              
	02E9634D| EB 02                 | jmp     short 02E96351                         
	02E9634F| 33C0                  | xor     eax, eax                 
	02E96351| C2 0400               | ret     4                        
	************************************************************************/

	if( Star::Process::GetModuleEntry(GetCurrentProcessId(),"MGRoom.dll",&module)==TRUE ){
		CSignMatch sign2;
		sign2.SetOrigSign("8B0490EB0233C0C20400",0x1000,0xB9000);//相对基址的范围
		pAddr=sign2.SearchSign((PBYTE)module.modBaseAddr);	//基址
		if (pAddr!=NULL){
			m_Hook2.Create(pAddr,GetSkinListThis);
			m_Hook2.Change();
		}else{
			::MessageBox(0,"第二组特征没有匹配",0,0);
		}
	}


	return TRUE;
}
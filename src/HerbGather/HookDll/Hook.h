// Hook.h : Hook DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "HookClass.h"
#include "ShareMemType.h"
#include "DlgMain.h"


#define ADDR_SEND			0x006982D0	//发送数据的函数地址
#define ADDR_MAPOBJ_THIS	0x026A9FFC	//存放小地图this指针的内存地址
#define ADDR_WALKTO			0x0076FD10	//定点移动的函数
#define ADDR_MAINOBJ_THIS	0x015B14B8	//最重要的一个this指针的内存地址
#define	ADDR_HERB_ARR		0x014D59AC  //存放草药数组的地址

//注意:一定要加上stdcall
extern "C" __declspec(dllexport) void __stdcall StartHook(HANDLE hMainWnd,DWORD dwThreadId);
extern "C" __declspec(dllexport) void __stdcall StopHook();

typedef DWORD(__cdecl*TSendOrder)(char*szGame,DWORD dwOrder,char*szMessage);
typedef DWORD(__stdcall*TWalkTo)(int x,int y,DWORD nReserved1,DWORD nReserved2);

// CHookApp
// 有关此类实现的信息，请参阅 Hook.cpp
//

class CHookApp : public CWinApp
{
public:
	CHookApp();
public:
	CHookClass	Hook;
	CDlgMain*m_pdlgMain;
	CString m_strDllPath;
// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

extern CHookApp theApp;
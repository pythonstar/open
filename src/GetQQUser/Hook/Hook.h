// Hook.h : Hook DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "DlgHookMain.h"
#include "../ShareMemType.h"
#include "../../StarLib/Star.h"

// CHookApp
// 有关此类实现的信息，请参阅 Hook.cpp
//

class CHookApp : public CWinApp
{
public:
	CHookApp();
public:
	CHookClass	m_Hook1,m_Hook2;
	CDlgHookMain*m_pdlgMain;
	CString m_strDllPath;
	BOOL	m_bCanInstalled;
private:
	BOOL InitHook();

// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

extern CHookApp		theApp;
extern PSHAREMEM	g_pShareMem;
extern DWORD		g_dwEnterRoomSubAddr;
extern DWORD		g_dwOrignAddr1;
extern DWORD		g_dwOrignAddr2;
// inject.h : inject DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "MainDlg.h"
#include "resource.h"		// 主符号


//注意:一定要加上stdcall
extern "C" __declspec(dllexport) void __stdcall start();

// CinjectApp
// 有关此类实现的信息，请参阅 inject.cpp
//

class CinjectApp : public CWinApp
{
public:
	CinjectApp();
	CMainDlg*m_pdlgMain;

// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

// inject.h : inject DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "MainDlg.h"
#include "resource.h"		// ������


//ע��:һ��Ҫ����stdcall
extern "C" __declspec(dllexport) void __stdcall start();

// CinjectApp
// �йش���ʵ�ֵ���Ϣ������� inject.cpp
//

class CinjectApp : public CWinApp
{
public:
	CinjectApp();
	CMainDlg*m_pdlgMain;

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

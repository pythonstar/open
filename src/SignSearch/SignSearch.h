// SignSearch.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSignSearchApp:
// �йش����ʵ�֣������ SignSearch.cpp
//

class CSignSearchApp : public CWinApp
{
public:
	CSignSearchApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSignSearchApp theApp;
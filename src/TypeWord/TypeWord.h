// TypeWord.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTypeWordApp:
// �йش����ʵ�֣������ TypeWord.cpp
//

class CTypeWordApp : public CWinApp
{
public:
	CTypeWordApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTypeWordApp theApp;
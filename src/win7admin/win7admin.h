// win7admin.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cwin7adminApp:
// �йش����ʵ�֣������ win7admin.cpp
//

class Cwin7adminApp : public CWinApp
{
public:
	Cwin7adminApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cwin7adminApp theApp;
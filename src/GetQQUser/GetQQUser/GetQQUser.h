// GetQQUser.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CGetQQUserApp:
// �йش����ʵ�֣������ GetQQUser.cpp
//

class CGetQQUserApp : public CWinApp
{
public:
	CGetQQUserApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CGetQQUserApp theApp;
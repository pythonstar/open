// DuplicateFileFinder.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "VersionNo.h"

// CDuplicateFileFinderApp:
// �йش����ʵ�֣������ DuplicateFileFinder.cpp
//

class CDuplicateFileFinderApp : public CWinApp
{
public:
	CDuplicateFileFinderApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDuplicateFileFinderApp theApp;
// Hook.h : Hook DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "DlgHookMain.h"
#include "../ShareMemType.h"
#include "../../StarLib/Star.h"

// CHookApp
// �йش���ʵ�ֵ���Ϣ������� Hook.cpp
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

// ��д
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
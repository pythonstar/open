// Hook.h : Hook DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "HookClass.h"
#include "ShareMemType.h"
#include "DlgMain.h"


#define ADDR_SEND			0x006982D0	//�������ݵĺ�����ַ
#define ADDR_MAPOBJ_THIS	0x026A9FFC	//���С��ͼthisָ����ڴ��ַ
#define ADDR_WALKTO			0x0076FD10	//�����ƶ��ĺ���
#define ADDR_MAINOBJ_THIS	0x015B14B8	//����Ҫ��һ��thisָ����ڴ��ַ
#define	ADDR_HERB_ARR		0x014D59AC  //��Ų�ҩ����ĵ�ַ

//ע��:һ��Ҫ����stdcall
extern "C" __declspec(dllexport) void __stdcall StartHook(HANDLE hMainWnd,DWORD dwThreadId);
extern "C" __declspec(dllexport) void __stdcall StopHook();

typedef DWORD(__cdecl*TSendOrder)(char*szGame,DWORD dwOrder,char*szMessage);
typedef DWORD(__stdcall*TWalkTo)(int x,int y,DWORD nReserved1,DWORD nReserved2);

// CHookApp
// �йش���ʵ�ֵ���Ϣ������� Hook.cpp
//

class CHookApp : public CWinApp
{
public:
	CHookApp();
public:
	CHookClass	Hook;
	CDlgMain*m_pdlgMain;
	CString m_strDllPath;
// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

extern CHookApp theApp;
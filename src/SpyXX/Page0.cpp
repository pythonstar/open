// Page0.cpp : implementation file
//

#include "stdafx.h"
#include "SpyXX.h"
#include "Page0.h"

#include <stdio.h>
#include <tlhelp32.h>



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage0 dialog

//extern HWND g_hWnd;
HWND g_hPage0;
CPage0::CPage0(CWnd* pParent /*=NULL*/)
	: CDialog(CPage0::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPage0)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPage0::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage0)
	DDX_Control(pDX, IDC_EDITWNDID, m_editWNDID);
	DDX_Control(pDX, IDC_EDITPATH, m_editPATH);
	DDX_Control(pDX, IDC_EDITPROCESSID, m_editPID);
	DDX_Control(pDX, IDC_EDITRECT, m_editRECT);
	DDX_Control(pDX, IDC_EDITTITLE, m_editTITLE);
	DDX_Control(pDX, IDC_EDITCLASS, m_editCLASS);
	DDX_Control(pDX, IDC_EDITHWND, m_editHWND);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage0, CDialog)
	//{{AFX_MSG_MAP(CPage0)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage0 message handlers

void CPage0::OnButton1() 
{
	// TODO: Add your control notification handler code here
	CString str;
	str.Format("%d",(int)g_hWnd);
	MessageBox(str);
}

BOOL CPage0::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	g_hPage0=m_hWnd;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
CString getProcPath(int PID)
{

	
	HANDLE hProcess=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	HANDLE hModule;
	PROCESSENTRY32* pinfo=new PROCESSENTRY32;
	MODULEENTRY32* minfo=new MODULEENTRY32;
	pinfo->dwSize=sizeof(PROCESSENTRY32);
	minfo->dwSize=sizeof(MODULEENTRY32);
	//BOOL report;
	char shortpath[MAX_PATH] = "";
	
	//printf("ShowProcessPath with [ToolHelp API]\n\n");
	
	//report=Process32First(hProcess,pinfo);
	
	//while(report)
	
		hModule=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,PID);//pinfo->th32ProcessID);
		Module32First(hModule, minfo);
		
		//GetShortPathName(minfo->szExePath,shortpath,256);
	
		//printf("%s --- %s\n",pinfo->szExeFile,shortpath);
		CString str;
		str.Format("%s",minfo->szExePath);
		//report=Process32Next(hProcess, pinfo); 
	CloseHandle(hProcess);
	CloseHandle(hModule);
	if(pinfo) delete pinfo;
	if(minfo) delete minfo;
	
	//printf("\n");
	return str;
}

// Page3.cpp : implementation file
//

#include "stdafx.h"
#include "SpyXX.h"
#include "Page3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage3 dialog

HWND g_hPage3=NULL;
CPage3::CPage3(CWnd* pParent /*=NULL*/)
	: CDialog(CPage3::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPage3)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPage3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage3)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage3, CDialog)
	//{{AFX_MSG_MAP(CPage3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage3 message handlers

BOOL CPage3::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	g_hPage3=m_hWnd;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

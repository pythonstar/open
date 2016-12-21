// Page4.cpp : implementation file
//

#include "stdafx.h"
#include "SpyXX.h"
#include "Page4.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage4 dialog

HWND g_hPage4=NULL;
CPage4::CPage4(CWnd* pParent /*=NULL*/)
	: CDialog(CPage4::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPage4)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPage4::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage4)
	DDX_Control(pDX, IDC_LISTSTATUS, m_listStatus);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage4, CDialog)
	//{{AFX_MSG_MAP(CPage4)
	ON_LBN_SELCHANGE(IDC_LISTSTATUS, OnSelchangeListstatus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage4 message handlers

BOOL CPage4::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	g_hPage4=m_hWnd;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPage4::OnSelchangeListstatus() 
{
	
	// TODO: Add your control notification handler code here
	int n=m_listStatus.GetCurSel();
	switch(n)
	{
	case 0:
        if(m_listStatus.GetCheck(0)== 1 )
            ::ShowWindow(g_hWnd, SW_SHOW);
        else
            ::ShowWindow(g_hWnd, SW_HIDE);
		break;
        
    case 1:
        if(m_listStatus.GetCheck(1) == 1)
            ::EnableWindow(g_hWnd, TRUE);
        else
            ::EnableWindow(g_hWnd,FALSE);
        break;
    case 2:
        if(m_listStatus.GetCheck(2) == 1)
            ::SetWindowPos(g_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
        else
            ::SetWindowPos(g_hWnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
		break;
    case 3:
        if(m_listStatus.GetCheck(3) == 1)
            ::SendMessage(g_hWnd, EM_SETREADONLY, TRUE, 0);
        else
            ::SendMessage(g_hWnd, EM_SETREADONLY, FALSE, 0);
		break;
        
    case 4:
        if(m_listStatus.GetCheck(4) ==1)
		{
            ::ShowWindow(g_hWnd, SW_MAXIMIZE);
            m_listStatus.SetCheck(5,0);
        }
		else
            ::ShowWindow (g_hWnd, SW_RESTORE);
        break;
    case 5:

        if (m_listStatus.GetCheck(5) == 1)
		{
			::ShowWindow(g_hWnd, SW_MINIMIZE);
            m_listStatus.SetCheck(4,0);
		}
        else
            ::ShowWindow(g_hWnd, SW_RESTORE);
		break;
    case 6:
        if(m_listStatus.GetCheck(6) ==1)
		{       ::ShowWindow (g_hWnd, SW_RESTORE);
            m_listStatus.SetCheck(6,0);
            m_listStatus.SetCheck(5,0);
            m_listStatus.SetCheck(4,0);
		}
        break;
    case 7:
        if(m_listStatus.GetCheck(7) ==1)
		{
			::SendMessage (g_hWnd, WM_CLOSE, 0, 0);
            m_listStatus.SetCheck(7,0);
		}
		break;
    case 8:
        if(m_listStatus.GetCheck(8) ==1)
		{
            ::BringWindowToTop(g_hWnd);
            m_listStatus.SetCheck(8,0);
		}
		break;
	default:
		;
	}
	
}

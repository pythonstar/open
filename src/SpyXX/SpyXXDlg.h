// SpyXXDlg.h : header file
//

#if !defined(AFX_SpyXXDLG_H__AC01BB38_19AA_4711_A42A_E54047E1A33F__INCLUDED_)
#define AFX_SpyXXDLG_H__AC01BB38_19AA_4711_A42A_E54047E1A33F__INCLUDED_

#include "MyPic.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSpyXXDlg dialog

class CSpyXXDlg : public CDialog
{
// Construction
public:
	CSpyXXDlg(CWnd* pParent = NULL);	// standard constructor
	CPage0 m_page0;
	CPage1 m_page1;
	CPage2 m_page2;
	CPage3 m_page3;
	CPage4 m_page4;





// Dialog Data
	//{{AFX_DATA(CSpyXXDlg)
	enum { IDD = IDD_SpyXX_DIALOG };
	CTabCtrl	m_tab;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpyXXDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSpyXXDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnAbout();
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnChkhex();
	afx_msg void OnChktop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	 CMyPic m_pic;
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SpyXXDLG_H__AC01BB38_19AA_4711_A42A_E54047E1A33F__INCLUDED_)

// AntiTestDlg.h : header file
//

#if !defined(AFX_ANTITESTDLG_H__FCFAC47C_7FE7_4626_BEE9_E728D0CADDDB__INCLUDED_)
#define AFX_ANTITESTDLG_H__FCFAC47C_7FE7_4626_BEE9_E728D0CADDDB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CAntiTestDlg dialog

class CAntiTestDlg : public CDialog
{
// Construction
public:
	CAntiTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CAntiTestDlg)
	enum { IDD = IDD_ANTITEST_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAntiTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAntiTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnCheckRemote();
	afx_msg void OnButton3();
	afx_msg void OnHardwareBreak();
	afx_msg void OnTitle();
	afx_msg void OnDebugerevent();
	afx_msg void OnZwQueryInformationProcess();
	afx_msg void OnStep();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANTITESTDLG_H__FCFAC47C_7FE7_4626_BEE9_E728D0CADDDB__INCLUDED_)

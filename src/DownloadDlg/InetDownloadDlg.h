// InetDownloadDlg.h : header file
//

#if !defined(AFX_INETDOWNLOADDLG_H__A7534C07_96D9_11D3_9CDD_9C09860A044F__INCLUDED_)
#define AFX_INETDOWNLOADDLG_H__A7534C07_96D9_11D3_9CDD_9C09860A044F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CInetDownloadDlg dialog

class CInetDownloadDlg : public CDialog
{
// Construction
public:
	CInetDownloadDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CInetDownloadDlg)
	enum { IDD = IDD_INETDOWNLOAD_DIALOG };
	CString	m_sFile;
	CString	m_sPassword;
	CString	m_sURL;
	CString	m_sUserName;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInetDownloadDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CInetDownloadDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDownload();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INETDOWNLOADDLG_H__A7534C07_96D9_11D3_9CDD_9C09860A044F__INCLUDED_)

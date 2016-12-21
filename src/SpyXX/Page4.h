#if !defined(AFX_PAGE4_H__C6F8F776_2112_4070_91F8_BF00B5928252__INCLUDED_)
#define AFX_PAGE4_H__C6F8F776_2112_4070_91F8_BF00B5928252__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page4.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage4 dialog

class CPage4 : public CDialog
{
// Construction
public:
	CPage4(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPage4)
	enum { IDD = IDD_OLE_PROPPAGE_LARGE4 };
	CCheckListBox	m_listStatus;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPage4)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPage4)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListstatus();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE4_H__C6F8F776_2112_4070_91F8_BF00B5928252__INCLUDED_)

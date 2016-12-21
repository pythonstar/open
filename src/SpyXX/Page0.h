#if !defined(AFX_PAGE0_H__DD3468A3_75D7_4BBC_830D_5CB4CA0BE45F__INCLUDED_)
#define AFX_PAGE0_H__DD3468A3_75D7_4BBC_830D_5CB4CA0BE45F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page0.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage0 dialog

class CPage0 : public CDialog
{
// Construction
public:
	CPage0(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPage0)
	enum { IDD = IDD_OLE_PROPPAGE_LARGE };
	CEdit	m_editWNDID;
	CEdit	m_editPATH;
	CEdit	m_editPID;
	CEdit	m_editRECT;
	CEdit	m_editTITLE;
	CEdit	m_editCLASS;
	CEdit	m_editHWND;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPage0)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPage0)
	afx_msg void OnButton1();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE0_H__DD3468A3_75D7_4BBC_830D_5CB4CA0BE45F__INCLUDED_)

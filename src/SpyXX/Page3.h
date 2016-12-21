#if !defined(AFX_PAGE3_H__10BC149A_D1AA_44AE_945A_0F083401F30A__INCLUDED_)
#define AFX_PAGE3_H__10BC149A_D1AA_44AE_945A_0F083401F30A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page3.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage3 dialog

class CPage3 : public CDialog
{
// Construction
public:
	CPage3(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPage3)
	enum { IDD = IDD_OLE_PROPPAGE_LARGE3 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPage3)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPage3)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE3_H__10BC149A_D1AA_44AE_945A_0F083401F30A__INCLUDED_)

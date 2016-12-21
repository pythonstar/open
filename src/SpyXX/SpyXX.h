// SpyXX.h : main header file for the SpyXX application
//

#if !defined(AFX_SpyXX_H__C75D0123_E16C_4A93_90E1_5FE195E3EFD2__INCLUDED_)
#define AFX_SpyXX_H__C75D0123_E16C_4A93_90E1_5FE195E3EFD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "mypic.h"
#include "page0.h"
#include "page1.h"
#include "page2.h"
#include "page3.h"
#include "page4.h"

/////////////////////////////////////////////////////////////////////////////
// CSpyXXApp:
// See SpyXX.cpp for the implementation of this class
//
extern HWND g_hWnd;
extern HWND g_hMe;
extern HWND g_hPage0;
extern HWND g_hPage1;
extern HWND g_hPage2;
extern HWND g_hPage3;
extern HWND g_hPage4;

extern int g_nHex;
extern CString Display(int nVal);


extern CString getProcPath(int PID);
class CSpyXXApp : public CWinApp
{
public:
	CSpyXXApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpyXXApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSpyXXApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SpyXX_H__C75D0123_E16C_4A93_90E1_5FE195E3EFD2__INCLUDED_)

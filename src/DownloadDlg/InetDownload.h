// InetDownload.h : main header file for the INETDOWNLOAD application
//

#if !defined(AFX_INETDOWNLOAD_H__A7534C05_96D9_11D3_9CDD_9C09860A044F__INCLUDED_)
#define AFX_INETDOWNLOAD_H__A7534C05_96D9_11D3_9CDD_9C09860A044F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CInetDownloadApp:
// See InetDownload.cpp for the implementation of this class
//

class CInetDownloadApp : public CWinApp
{
public:
	CInetDownloadApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInetDownloadApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CInetDownloadApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INETDOWNLOAD_H__A7534C05_96D9_11D3_9CDD_9C09860A044F__INCLUDED_)

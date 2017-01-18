#pragma once

#include "afxcmn.h"
#include <controls/StaticLink/StaticLink.h>
#include <controls/HyperLink/HyperLink.h>
#include <controls/TabSheet/TabSheet.h>
#include "SettingDlg.h"
#include "afxwin.h"
#include "global.h"
#include <StarLib/SqliteDB/SqliteDB.h>

// CMainDlg 对话框

class CMainDlg : public CDialog
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMainDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_MAIN };
	enum { 
		WM_FINDONEDUPFILE = WM_USER + 10, WM_FINDSTART, WM_FINDEND
	};

private:
	CSettingDlg m_settingDlg;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG *pMsg);
	virtual BOOL OnInitDialog();

	afx_msg LRESULT OnFindOneDupFile(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnFindDupFileStart(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnFindDupFileEnd(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnNMClickListAlbum(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheckSelectAll();
	afx_msg void OnBnClickedSetting();
	afx_msg void OnBnClickedButtonSearchAll();
	afx_msg void OnBnClickedButtonSearchCustom();
	CStatic m_stcCurrentMsg;
	afx_msg void OnBnClickedButtonPause();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnMenuLocateFile(int nItem,int nSubItem);
	afx_msg void OnNMRclickListAlbum(NMHDR *pNMHDR, LRESULT *pResult);

public:
	CAnimateCtrl m_animate;
	CImageList m_imageList;

private:
	BOOL m_bInited;
	void InitAllControls();

	CButton m_chkSelectAll;
	CListCtrl m_lstFile;
	CString m_strApplicationPath;

	static UINT ThreadFindDupFile(LPVOID lpParam);
	static void FindDupFile(CString&strFolder);
	static CEvent m_eventStop;
	static int m_nRunningFlag;
	static int m_nFileTotal;
	static CSqliteDB m_sqlitedb;
	static void InitDbFile();
	static void FreeDbFile();
	static void AnalyzeFile(CString&strFileName);

	static CString m_strDupFileName;
	static CString m_strSrcFileName;

	static vector<CString>m_vtFileTypeFilter;
};

extern CMainDlg*g_pMainDlg;

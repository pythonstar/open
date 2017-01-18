#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CSettingDlg 对话框

class CSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CSettingDlg)

public:
	CSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSettingDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SETTINGS };

private:
	void InitAllControls();
	void InitFindType(CString strConfigFile);
	void LoadFindType(CString strConfigFile);
	void SaveFindType();

	void OnUpdateFindType(int nItem);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG *pMsg);
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cmbFindType;
	CListCtrl m_lstFindType;
	afx_msg void OnNMRclickListFileType(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnNMDblclkListFileType(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

extern CSettingDlg *g_settingDlg;
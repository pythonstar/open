// SignSearchDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CSignSearchDlg 对话框
class CSignSearchDlg : public CDialog
{
// 构造
public:
	CSignSearchDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SIGNSEARCH_DIALOG };
	enum { WM_SCANSIGNBEGIN=WM_USER+1,WM_SCANSIGNEND };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


	void InitAllControls();
	void AppendText(const CString&strText);
	int AddSigns(CString&strSigns);

	//从文本文件中加载上次保持的特征
	void LoadSigns();
	void SaveSigns();
	CString GetStartPath();
	CString m_strStartPath;
	CString m_strSignFile;


	BOOL m_bScanning;
	static UINT ThreadScanSign(LPVOID lParam);
	afx_msg LRESULT OnScanSignBegin(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnScanSignEnd(WPARAM wParam,LPARAM lParam);

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG *pMsg);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edtMsg;
	CListCtrl m_lstSign;
	afx_msg void OnNMRclickListSign(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListSign(NMHDR *pNMHDR, LRESULT *pResult);
	void OnUpdateSign(int nItem);
};

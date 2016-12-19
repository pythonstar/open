#pragma once


// CUpdateSignDlg 对话框

class CUpdateSignDlg : public CDialog
{
	DECLARE_DYNAMIC(CUpdateSignDlg)

public:
	CUpdateSignDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUpdateSignDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_UPDATESIGN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strSearchBase;
	CString m_strStartRva;
	CString m_strEndRva;
	CString m_strSign;
	afx_msg void OnBnClickedOk();
};

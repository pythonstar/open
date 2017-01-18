#pragma once


// CFindTypeDlg 对话框

class CFindTypeDlg : public CDialog
{
	DECLARE_DYNAMIC(CFindTypeDlg)

public:
	CFindTypeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFindTypeDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_FIND_TYPE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strFindTypeDesc;
	CString m_strFindTypeFilter;
	afx_msg void OnBnClickedOk();
};

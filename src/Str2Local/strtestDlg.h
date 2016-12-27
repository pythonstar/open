// strtestDlg.h : 头文件
//

#pragma once


// CstrtestDlg 对话框
class CstrtestDlg : public CDialog
{
// 构造
public:
	CstrtestDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_STRTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CString m_strIN;
	CString m_strOut;
	afx_msg void OnBnClickedOk();
};

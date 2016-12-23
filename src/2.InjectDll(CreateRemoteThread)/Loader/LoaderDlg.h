// LoaderDlg.h : 头文件
//

#pragma once


// CLoaderDlg 对话框
class CLoaderDlg : public CDialog
{
// 构造
public:
	CLoaderDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_Loader_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg LRESULT OnInjectSuccess(WPARAM wParam,LPARAM lParam);
	CString m_strDllFile;
	DWORD m_dwPID;
};


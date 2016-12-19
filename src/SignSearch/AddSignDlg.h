#pragma once


// CAddSignDlg 对话框

class CAddSignDlg : public CDialog
{
	DECLARE_DYNAMIC(CAddSignDlg)

public:
	CAddSignDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddSignDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_ADDSIGN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strSigns;
};

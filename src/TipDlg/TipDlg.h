#pragma once


// CTipDlg 对话框

class CTipDlg : public CDialog
{
	DECLARE_DYNAMIC(CTipDlg)
private:
	DWORD m_dwShowTime;
	POINT m_Points[3];
	CString m_strTip;
// 	CRect rcOutter;
// 	CRect rcInner;
public:
	void SetTipString(CString strTip);

public:
	CTipDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTipDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_TIP };

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnTimer( UINT );
 
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};

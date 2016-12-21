#pragma once
#include "afxwin.h"


// CDlgHookMain 对话框

class CDlgHookMain : public CDialog
{
	DECLARE_DYNAMIC(CDlgHookMain)

public:
	CDlgHookMain(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgHookMain();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };
private:
	void AppendMsg(CString strMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL DestroyWindow();
	virtual void OnOK();
	virtual void OnCancel();
	LRESULT OnOrderEnterRoom(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edtMsg;
};

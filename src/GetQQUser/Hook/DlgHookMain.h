#pragma once
#include "afxwin.h"


// CDlgHookMain �Ի���

class CDlgHookMain : public CDialog
{
	DECLARE_DYNAMIC(CDlgHookMain)

public:
	CDlgHookMain(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgHookMain();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };
private:
	void AppendMsg(CString strMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL DestroyWindow();
	virtual void OnOK();
	virtual void OnCancel();
	LRESULT OnOrderEnterRoom(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edtMsg;
};

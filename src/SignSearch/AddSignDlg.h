#pragma once


// CAddSignDlg �Ի���

class CAddSignDlg : public CDialog
{
	DECLARE_DYNAMIC(CAddSignDlg)

public:
	CAddSignDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddSignDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_ADDSIGN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strSigns;
};

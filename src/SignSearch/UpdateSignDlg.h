#pragma once


// CUpdateSignDlg �Ի���

class CUpdateSignDlg : public CDialog
{
	DECLARE_DYNAMIC(CUpdateSignDlg)

public:
	CUpdateSignDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUpdateSignDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_UPDATESIGN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strSearchBase;
	CString m_strStartRva;
	CString m_strEndRva;
	CString m_strSign;
	afx_msg void OnBnClickedOk();
};

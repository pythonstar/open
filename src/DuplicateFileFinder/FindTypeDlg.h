#pragma once


// CFindTypeDlg �Ի���

class CFindTypeDlg : public CDialog
{
	DECLARE_DYNAMIC(CFindTypeDlg)

public:
	CFindTypeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFindTypeDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_FIND_TYPE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strFindTypeDesc;
	CString m_strFindTypeFilter;
	afx_msg void OnBnClickedOk();
};

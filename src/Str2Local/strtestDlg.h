// strtestDlg.h : ͷ�ļ�
//

#pragma once


// CstrtestDlg �Ի���
class CstrtestDlg : public CDialog
{
// ����
public:
	CstrtestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_STRTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CString m_strIN;
	CString m_strOut;
	afx_msg void OnBnClickedOk();
};

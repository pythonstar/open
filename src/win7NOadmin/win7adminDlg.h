// win7adminDlg.h : ͷ�ļ�
//

#pragma once


// Cwin7adminDlg �Ի���
class Cwin7adminDlg : public CDialog
{
// ����
public:
	Cwin7adminDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_WIN7ADMIN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int m_nPID;
	afx_msg void OnBnClickedOk();
};

// GetListDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CGetListDlg �Ի���
class CGetListDlg : public CDialog
{
// ����
public:
	CGetListDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GETLIST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
public:
	CEdit m_edtUserList;
public:
	afx_msg void OnClose();
};

// GetUserListDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "MyButton.h"
#include "afxcmn.h"

// CGetUserListDlg �Ի���
class CGetUserListDlg : public CDialog
{
// ����
public:
	CGetUserListDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GETUSERLIST_DIALOG };

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
	CMyButton m_btnShoot;
public:
	afx_msg void OnBnClickedButton2();
public:
	CListCtrl m_lstCopyed;
};

// ���ݴ���Dlg.h : ͷ�ļ�
//

#pragma once

#include "TipDlg.h"

// C���ݴ���Dlg �Ի���
class C���ݴ���Dlg : public CDialog
{
private:
	CTipDlg *tipDlg;
// ����
public:
	C���ݴ���Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MY_DIALOG };

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
};

// LoadDllDlg.h : ͷ�ļ�
//

#pragma once


// CLoadDllDlg �Ի���
class CLoadDllDlg : public CDialog
{
// ����
public:
	CLoadDllDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_LOADDLL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};

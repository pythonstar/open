/************************************************************************
�ļ���PreviewDlg.h
���ߣ�������
���ڣ�2009-6-6
˵��������Ԥ������ͷ�ļ�����Ҫ����ʾ���������в�ҩ�ڵ�ͼ�еĴ��·ֲ������
	  Ԥ�����������󻯣��������ܲ鿴��ȫ��
************************************************************************/

#pragma once


// CPreviewDlg �Ի���

class CPreviewDlg : public CDialog
{
	DECLARE_DYNAMIC(CPreviewDlg)

public:
	CPreviewDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPreviewDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_PREVIEW };

public:
	CString m_strMethodFile;	//���淽�����ļ�����ȫ·����
	int		m_nPlaceCnt;		//�����е��������
	BOOL	m_bInited;
	POINT	m_ptArr[1000];		//�������������

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};

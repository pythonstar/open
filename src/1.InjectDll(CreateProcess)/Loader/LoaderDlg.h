// LoaderDlg.h : ͷ�ļ�
//

#pragma once
#include "../ShareMemType.h"


// CLoaderDlg �Ի���
class CLoaderDlg : public CDialog
{
// ����
public:
	CLoaderDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_Loader_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

private:
	HANDLE m_hMapFile;
	PSHAREMEM m_pShareMem;

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CString m_strExeFile;
	afx_msg void OnBnClickedOk();
	afx_msg LRESULT OnInjectSuccess(WPARAM wParam,LPARAM lParam);
	CString m_strDllFile;
	CString m_strDllFunc;
};


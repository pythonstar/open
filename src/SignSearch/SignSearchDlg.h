// SignSearchDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CSignSearchDlg �Ի���
class CSignSearchDlg : public CDialog
{
// ����
public:
	CSignSearchDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SIGNSEARCH_DIALOG };
	enum { WM_SCANSIGNBEGIN=WM_USER+1,WM_SCANSIGNEND };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


	void InitAllControls();
	void AppendText(const CString&strText);
	int AddSigns(CString&strSigns);

	//���ı��ļ��м����ϴα��ֵ�����
	void LoadSigns();
	void SaveSigns();
	CString GetStartPath();
	CString m_strStartPath;
	CString m_strSignFile;


	BOOL m_bScanning;
	static UINT ThreadScanSign(LPVOID lParam);
	afx_msg LRESULT OnScanSignBegin(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnScanSignEnd(WPARAM wParam,LPARAM lParam);

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG *pMsg);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edtMsg;
	CListCtrl m_lstSign;
	afx_msg void OnNMRclickListSign(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListSign(NMHDR *pNMHDR, LRESULT *pResult);
	void OnUpdateSign(int nItem);
};

#pragma once
#include "afxcmn.h"


// CCustomSearchAreaDlg �Ի���

class CCustomSearchAreaDlg : public CDialog
{
	DECLARE_DYNAMIC(CCustomSearchAreaDlg)

public:
	CCustomSearchAreaDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCustomSearchAreaDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_CUSTOM_AREA };

private:
	void InitAllControls();

public:
	CStringList *m_pFindAreaList;
	CImageList *m_imagelist;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();


	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_lstArea;
	afx_msg void OnNMClickListArea(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedButtonAddFolder();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonDelFolder();
	afx_msg void OnNMRclickListArea(NMHDR *pNMHDR, LRESULT *pResult);
};

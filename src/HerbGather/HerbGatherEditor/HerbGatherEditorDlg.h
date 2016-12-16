// HerbGatherEditorDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "shlwapi.h"
#include "afxwin.h"
#include "OfficeXPMenu.h"

#define HERB_GATHER_METHOD_PATH	"Herb"	
#define HERB_GATHER_METHOD_FILE "Method"

// CHerbGatherEditorDlg 对话框
class CHerbGatherEditorDlg : public CDialog
{

	CString m_strMethodPath;
	int		m_nMethodCnt;
private:
	void InitHerbGatherMethod(CString strPath);
	void InitHerbPlaceList();
	void SaveMethod();

// 构造
public:
	CHerbGatherEditorDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_HERBGATHEREDITOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_lstGatherMethod;
public:
	CListCtrl m_lstHerbPlace;
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton3();
public:
	CString m_strPosx;
public:
	CString m_strPosy;
public:
	afx_msg void OnBnClickedButton4();
public:
	CString m_strMethodName;
public:
	afx_msg void OnNMRclickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclickList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnAddPosition();
	afx_msg void OnAlterPosition();
	afx_msg void OnDeletePosition();
public:
	CButton m_btnPosCmd;
public:
	afx_msg void OnDeleteMethod();
	afx_msg void OnPreviewMethod();
};

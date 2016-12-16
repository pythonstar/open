/************************************************************************
文件：PreviewDlg.h
作者：朱星星
日期：2009-6-6
说明：方案预览窗口头文件，主要是显示各个方案中草药在地图中的大致分布情况。
	  预览窗口最好最大化，这样才能查看完全。
************************************************************************/

#pragma once


// CPreviewDlg 对话框

class CPreviewDlg : public CDialog
{
	DECLARE_DYNAMIC(CPreviewDlg)

public:
	CPreviewDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPreviewDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_PREVIEW };

public:
	CString m_strMethodFile;	//保存方案的文件名（全路径）
	int		m_nPlaceCnt;		//方案中的坐标个数
	BOOL	m_bInited;
	POINT	m_ptArr[1000];		//保存坐标的数组

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};

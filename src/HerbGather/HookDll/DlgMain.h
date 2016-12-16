#pragma once
#include "afxwin.h"
#include "Hook.h"
#include "shlwapi.h"

#define TIMER_GET_POS				1
#define TIMER_CHECK_HERB_GATHERED	2
#define TIMER_COUNT_HERB			3

typedef struct HERB
{
	UINT	ID;
	int		nPosx;
	int		nPosy;
	int		nOffset;
}*PHERB;

// CDlgMain 对话框

class CDlgMain : public CDialog
{
	DECLARE_DYNAMIC(CDlgMain)

public:
	CDlgMain(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMain();
	void AppendText(CString strText);
	void WalkTo(int x,int y);
	void Arrived();
	int  SearchHerb();
	PHERB GetNearestHerb(int nMinLen=65535);
	void GatherHerb(UINT ID);
	void ClearHerbArr();

public:
	int m_nCurPosX;
	int m_nCurPosY;
	int m_nDstPosX;
	int m_nDstPosY;
	CPtrArray m_arrHerb;
	UINT m_uGatheringHerbID;	//正在采集的草药的ID
	CString m_strCfgFile;

// 对话框数据
	enum { IDD = IDD_DIALOG_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow( );
	void OnOK();
	void OnCancel();

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edtMsg;
	BOOL m_bCheckSend;
	BOOL m_bCheckRecv;
public:
	afx_msg void OnBnClickedCheckDataSend();
public:
	CString m_strGame;
	CString m_strMessage;
public:
	afx_msg void OnBnClickedButtonSend();
public:
	CComboBox m_cmbOrder;
public:
	afx_msg void OnBnClickedButtonWalk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

public:
	afx_msg void OnEnChangeEditMsg();
public:
	int m_nGatherLevel;
public:
	afx_msg void OnBnClickedButtonGatherHerb();
public:
	CString m_strRoleName;
public:
	CComboBox m_cmbHerbs;
public:
	CButton m_btnGatherHerb;
public:
	afx_msg void OnCbnSelchangeComboHerbs();
public:
	afx_msg void OnCbnDropdownComboHerbs();
public:
	afx_msg void OnBnClickedButtonCountHerb();
public:
	afx_msg void OnBnClickedButton3();
public:
	afx_msg void OnBnClickedButton1();
};

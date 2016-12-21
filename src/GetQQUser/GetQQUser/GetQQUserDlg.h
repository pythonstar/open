// GetQQUserDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include <vector>
using namespace std;
#include "../ShareMemType.h"

typedef void (__stdcall*TStartHook)(HWND hControlWnd,HWND hDummyWnd,DWORD dwThreadId);
typedef void (__stdcall*TStopHook)();

typedef struct TREEITEMPARAM
{
	DWORD dwObj;
	DWORD dwUnknow1;
	DWORD dwUnknow2;
	DWORD lpszMsg1;
	DWORD dwUnknow3;
	DWORD dwUnknow4;
	DWORD dwUnknow5;
	DWORD lpszMsg2;
	DWORD dwUnknow6;
	DWORD dwUnknow7;
}*PTREEITEMPARAM;


struct GAME_INFO 
{
	DWORD dwGameID;
	CString strGameName;
};

struct PLAYER_INFO 
{
	CString strQQ;
	CString strNicky;
};

namespace GAME_TYPE
{
	enum { 
		SIGUOJUNQI=1, ZHAOCHA=2, SHENGJI=4, DOUDIZHU3=8, DOUDIZHU4=16, 
		LIANLIANKAN=32, PAOPAOLONG, DUIDUIPENG, QQLONGZHU,ZHONGGUOXIANGQI, 
		WUZIQI, FEIXINGQI,
		MAJIANG, TIAOQI, WAJINZI,
		D2ZHUOQIU, WEIQI
		};

};

static GAME_INFO gamelist[]=
{
	{GAME_TYPE::SIGUOJUNQI, "四国军棋"},
	{GAME_TYPE::ZHAOCHA, "找茬"},
	{GAME_TYPE::SHENGJI, "升级"},
	{GAME_TYPE::DOUDIZHU3,	"欢乐斗地主"},
	{GAME_TYPE::DOUDIZHU4,	"四人斗地主"},
	{GAME_TYPE::LIANLIANKAN, "连连看"}

};


// CGetQQUserDlg 对话框
class CGetQQUserDlg : public CDialog
{
// 构造
public:
	CGetQQUserDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_GETQQUSER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	DWORD		m_dwThreadId;
	HANDLE		m_hMapFile;
	PSHAREMEM	m_pShareMem;
	TStartHook	StartHook;
	TStopHook	StopHook;
private:
	HWND	m_hMainQQGame;
	HWND	m_hGameListTree;
	int		m_nTreeItemCount;
	HANDLE	m_hProcessQQGame;
	PBYTE	m_pItemBuff;
	PBYTE	m_pOutBuff;
	CSqliteDB m_sqlitedb;
	vector<PLAYER_INFO>m_vtPlayer;
	void AppendMsg(CString strMsg);
	void InitQQGameInfo();
	void FreeMemory();
	void VisitTree(CTreeCtrl&ctrlTree,HTREEITEM hItem);

	HTREEITEM GetSpecifiedItem(CTreeCtrl&ctrlTree,HTREEITEM hItem,LPCTSTR lpszDstText,BOOL bJustContain=TRUE);

	BOOL ItemHasChildren(HWND hWnd, HTREEITEM hItem);
	CString GetItemText(HWND hWnd,HTREEITEM hItem);
	void GetItemParam(HWND hWnd,HTREEITEM hItem);
	void GetItemRect(HWND hWnd,HTREEITEM hItem,LPRECT lpRect);
	vector<HTREEITEM>m_vtTreeItem;	//无孩子的节点
	int		m_nSelectedItem;

	HWND FindExitRoomButton();
	BOOL EnterRoom();
	void ExitRoom();
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	CString m_strAddr;
	CEdit m_edtMsg;
	CString m_strGameType;
	CComboBox m_cmbGameType;
public:
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonSavetodb();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};

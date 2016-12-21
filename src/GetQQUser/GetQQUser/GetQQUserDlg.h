// GetQQUserDlg.h : ͷ�ļ�
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
	{GAME_TYPE::SIGUOJUNQI, "�Ĺ�����"},
	{GAME_TYPE::ZHAOCHA, "�Ҳ�"},
	{GAME_TYPE::SHENGJI, "����"},
	{GAME_TYPE::DOUDIZHU3,	"���ֶ�����"},
	{GAME_TYPE::DOUDIZHU4,	"���˶�����"},
	{GAME_TYPE::LIANLIANKAN, "������"}

};


// CGetQQUserDlg �Ի���
class CGetQQUserDlg : public CDialog
{
// ����
public:
	CGetQQUserDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GETQQUSER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

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
	vector<HTREEITEM>m_vtTreeItem;	//�޺��ӵĽڵ�
	int		m_nSelectedItem;

	HWND FindExitRoomButton();
	BOOL EnterRoom();
	void ExitRoom();
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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

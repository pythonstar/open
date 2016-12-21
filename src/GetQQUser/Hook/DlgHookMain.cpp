// DlgHookMain.cpp : 实现文件
//

#include "stdafx.h"
#include "Hook.h"
#include "DlgHookMain.h"
#include "Hook.h"

// CDlgHookMain 对话框

IMPLEMENT_DYNAMIC(CDlgHookMain, CDialog)

CDlgHookMain::CDlgHookMain(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHookMain::IDD, pParent)
{

}

CDlgHookMain::~CDlgHookMain()
{
}

void CDlgHookMain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MSG, m_edtMsg);
}


BEGIN_MESSAGE_MAP(CDlgHookMain, CDialog)
	ON_MESSAGE(WM_ENTERROOM,OnOrderEnterRoom)
END_MESSAGE_MAP()


// CDlgHookMain 消息处理程序
void CDlgHookMain::AppendMsg(CString strMsg)
{
	LONG nLen=(LONG)m_edtMsg.SendMessage(WM_GETTEXTLENGTH);
	m_edtMsg.SetSel(nLen,-1);
	m_edtMsg.ReplaceSel(strMsg+"\r\n");
}

BOOL CDlgHookMain::DestroyWindow()
{
	return CDialog::DestroyWindow();
}

void CDlgHookMain::OnOK()
{
	DestroyWindow();
}

void CDlgHookMain::OnCancel()
{
	DestroyWindow();
}

LRESULT CDlgHookMain::OnOrderEnterRoom(WPARAM wParam,LPARAM lParam)
{
	if (g_pShareMem==NULL){
		AppendMsg("g_pShareMem==NULL");
	}else if (g_dwEnterRoomSubAddr==NULL){
		AppendMsg("g_dwEnterRoomSubAddr==NULL");
	}else{
		CString strText;
		strText.Format("进入房间：%08X(func:%08X,this:%08X)",
			lParam,g_dwEnterRoomSubAddr,g_pShareMem->dwTreeThis);
		AppendMsg(strText);

		DWORD dwThis=g_pShareMem->dwTreeThis;
		__asm{
			push lParam
			mov eax,g_dwEnterRoomSubAddr
			mov ecx,dwThis
			CALL eax
		}
	}
	return 0;
}
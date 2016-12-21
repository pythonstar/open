// GetListDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GetList.h"
#include "GetListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGetListDlg 对话框

CGetListDlg::CGetListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGetListDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGetListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edtUserList);
}

BEGIN_MESSAGE_MAP(CGetListDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CGetListDlg::OnBnClickedButton1)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CGetListDlg 消息处理程序

BOOL CGetListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGetListDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CGetListDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void AppendMsg(CEdit*edtMsg,CString strMsg)
{
	LONG nLen=(LONG)edtMsg->SendMessage(WM_GETTEXTLENGTH);
	edtMsg->SetSel(nLen,-1);
	edtMsg->ReplaceSel(strMsg);
}

CEdit* edtMsg=NULL;
BOOL bRunning=TRUE;

UINT ThreadCallBack(PVOID lparam)
{	
	char szText[280];
	CString strText;

	HWND hSkinList;
	HWND hTemp;
	HWND hParent;
	HWND hwnd;

	hParent=::FindWindow(NULL,"QQ游戏");

	hwnd=::FindWindowEx(hParent,0,"Afx:15a0000:8:10011:0:0","SkinWnd");
	hwnd=::FindWindowEx(hwnd,0,"Afx:15a0000:8:10011:0:0","SkinTab");
	hwnd=::FindWindowEx(hwnd,0,"Afx:15a0000:8:10011:0:0","SkinWnd");
	hwnd=::FindWindowEx(hwnd,0,"Afx:15a0000:8:10011:0:0","SkinWnd");
	hTemp=::FindWindowEx(hwnd,0,"Afx:15a0000:8:10011:0:0","SkinWnd");
	hSkinList=::FindWindowEx(hwnd,hTemp,"Afx:15a0000:8:10011:0:0","SkinWnd");
	hwnd=::FindWindowEx(hTemp,0,"Afx:22d0000:0","");
	hwnd=::FindWindowEx(hwnd,0,"Afx:15a0000:8:10011:0:0","玩家资料面板");
	hwnd=::FindWindowEx(hwnd,0,"Edit",NULL);

	hSkinList=::FindWindowEx(hSkinList,0,"Afx:15a0000:8:10011:0:0","SkinTab");
	hSkinList=::FindWindowEx(hSkinList,0,"Afx:15a0000:8:10011:0:0","SkinWnd");
	hSkinList=::FindWindowEx(hSkinList,0,"Afx:15a0000:8:10011:0:0","SkinList");

	int nCount=400;
	int nPosy=33;
	while (bRunning&&nCount--)
	{
		::SendMessage(hSkinList,WM_LBUTTONDOWN,MK_LBUTTON,MAKELPARAM(181,nPosy));
		nPosy+=19;
		int n=::SendMessage(hwnd,WM_GETTEXT,sizeof(szText),(LPARAM)szText);
		szText[n++]='\r';
		szText[n++]='\n';
		szText[n]=0;
		strText=szText;
		AppendMsg(edtMsg,szText);
		Sleep(200);
	}

	return 0;
}
void CGetListDlg::OnBnClickedButton1()
{
	edtMsg=&m_edtUserList;
	AfxBeginThread(ThreadCallBack,NULL);
}

void CGetListDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	bRunning=FALSE;
	CDialog::OnClose();
}

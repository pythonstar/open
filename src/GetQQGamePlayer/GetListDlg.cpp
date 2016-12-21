// GetListDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GetList.h"
#include "GetListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGetListDlg �Ի���

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


// CGetListDlg ��Ϣ�������

BOOL CGetListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CGetListDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
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

	hParent=::FindWindow(NULL,"QQ��Ϸ");

	hwnd=::FindWindowEx(hParent,0,"Afx:15a0000:8:10011:0:0","SkinWnd");
	hwnd=::FindWindowEx(hwnd,0,"Afx:15a0000:8:10011:0:0","SkinTab");
	hwnd=::FindWindowEx(hwnd,0,"Afx:15a0000:8:10011:0:0","SkinWnd");
	hwnd=::FindWindowEx(hwnd,0,"Afx:15a0000:8:10011:0:0","SkinWnd");
	hTemp=::FindWindowEx(hwnd,0,"Afx:15a0000:8:10011:0:0","SkinWnd");
	hSkinList=::FindWindowEx(hwnd,hTemp,"Afx:15a0000:8:10011:0:0","SkinWnd");
	hwnd=::FindWindowEx(hTemp,0,"Afx:22d0000:0","");
	hwnd=::FindWindowEx(hwnd,0,"Afx:15a0000:8:10011:0:0","����������");
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	bRunning=FALSE;
	CDialog::OnClose();
}

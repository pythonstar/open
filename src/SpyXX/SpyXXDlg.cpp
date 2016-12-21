// SpyXXDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpyXX.h"
#include "SpyXXDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
int n=0;
int g_nHex=1;   //��־��16���ơ��Ƿ�ѡ��
CString Display(int nVal)
{
	CString str;
	if(g_nHex==1)
	{
		str.Format("%x",nVal);
		str.MakeUpper();
	}
	else
		str.Format("%d",nVal);
	
	return str;
}
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnUrl();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_URL, OnUrl)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpyXXDlg dialog

CSpyXXDlg::CSpyXXDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSpyXXDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSpyXXDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSpyXXDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpyXXDlg)
	DDX_Control(pDX, IDC_TAB1, m_tab);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSpyXXDlg, CDialog)
	//{{AFX_MSG_MAP(CSpyXXDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ABOUT, OnAbout)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHKHEX, OnChkhex)
	ON_BN_CLICKED(IDC_ABOUT, OnAbout)
	ON_BN_CLICKED(IDC_CHKTOP, OnChktop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpyXXDlg message handlers

BOOL CSpyXXDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	//�ѡ�ʼ����ǰ���͡�16���ơ����Ϲ���Ĭ�ϣ�
	((CButton*)GetDlgItem(IDC_CHKTOP))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_CHKHEX))->SetCheck(1);
	g_nHex=1;

	//Ĭ��������ǰ
	if(!::SetWindowPos(m_hWnd, (HWND)-1, 0, 0, 0, 0, 3))
		TRACE("������ǰʧ��");



	m_pic.SubclassDlgItem(IDC_PIC,this);
	m_tab.InsertItem(0," ���� ");
	m_tab.InsertItem(1," ��ʽ ");
	m_tab.InsertItem(2,"  ��  ");
	m_tab.InsertItem(3," ���� ");
	m_tab.InsertItem(4," ��Ϣ ");
	//m_tab.InsertItem(5," ͼ�� ");


	m_page0.Create(IDD_OLE_PROPPAGE_LARGE,GetDlgItem(IDC_TAB1));
	m_page1.Create(IDD_OLE_PROPPAGE_LARGE1,GetDlgItem(IDC_TAB1));
	m_page2.Create(IDD_OLE_PROPPAGE_LARGE2,GetDlgItem(IDC_TAB1));
	m_page3.Create(IDD_OLE_PROPPAGE_LARGE3,GetDlgItem(IDC_TAB1));
	m_page4.Create(IDD_OLE_PROPPAGE_LARGE4,GetDlgItem(IDC_TAB1));
	CRect rs;
	m_tab.GetClientRect(rs);

	rs.top+=20;
	rs.bottom-=3;
	rs.left+=3;
	rs.right-=3;

	m_page0.MoveWindow(rs);
	m_page1.MoveWindow(rs);
	m_page2.MoveWindow(rs);
	m_page3.MoveWindow(rs);
	m_page4.MoveWindow(rs);

	m_page0.ShowWindow(SW_SHOW);
	
	m_tab.SetCurSel(0);
	//SetTimer(1,800,NULL);
	g_hMe=m_hWnd;

	//���IDC_LISTSTATUS�б��

	CCheckListBox* plistStatus=((CCheckListBox*)FromHandle(g_hPage4)->GetDlgItem(IDC_LISTSTATUS));
	plistStatus->AddString("���ڿɼ�");
	plistStatus->AddString("���ڿ���");
	plistStatus->AddString("������ǰ");
	plistStatus->AddString("����ֻ��");
	plistStatus->AddString("���");
	plistStatus->AddString("��С��");
	plistStatus->AddString("���ڻ�ԭ");
	plistStatus->AddString("�رմ���");
	plistStatus->AddString("�����");

	


	return TRUE;  // return TRUE  unless you set the focus to a control


}

void CSpyXXDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSpyXXDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSpyXXDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSpyXXDlg::OnOK() 
{
	// TODO: Add extra validation here
	//m_pic.SetIcon(LoadIcon(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDI_ICON2)));
	CDialog::OnOK();
}

void CSpyXXDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialog::OnCancel();
}

void CSpyXXDlg::OnAbout() 
{
	// TODO: Add your control notification handler code here
/*
	CString str("Spy Lite V2.2 ��Ȩ����(C) 2000-2006 2006.12.14\n����:��ɺ���� QQ:6019187 VBȺ:2634364 VCȺ:713035\n������ҳ:http://www.asanscape.com ���ȷ������ҳ��");
	int i=MessageBox(str,"About",MB_OKCANCEL | MB_ICONASTERISK);
	if(i==2)  //��ȡ��
		;
	else
		ShellExecute(NULL,NULL,"http://www.asanscape.com",NULL,NULL,1);
*/

	CAboutDlg dlg;
	dlg.DoModal();
}

void CSpyXXDlg::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int i=m_tab.GetCurSel();
	switch(i) {
	case 0:
		m_page0.ShowWindow(SW_SHOW);
		m_page1.ShowWindow(SW_HIDE);
		m_page2.ShowWindow(SW_HIDE);
		m_page3.ShowWindow(SW_HIDE);
		m_page4.ShowWindow(SW_HIDE);
		break;
	case 1:
		m_page0.ShowWindow(SW_HIDE);
		m_page1.ShowWindow(SW_SHOW);
		m_page2.ShowWindow(SW_HIDE);
		m_page3.ShowWindow(SW_HIDE);
		m_page4.ShowWindow(SW_HIDE);
		break;
	case 2:
		m_page0.ShowWindow(SW_HIDE);
		m_page1.ShowWindow(SW_HIDE);
		m_page2.ShowWindow(SW_SHOW);
		m_page3.ShowWindow(SW_HIDE);
		m_page4.ShowWindow(SW_HIDE);
		break;
	case 3:
		m_page0.ShowWindow(SW_HIDE);
		m_page1.ShowWindow(SW_HIDE);
		m_page2.ShowWindow(SW_HIDE);
		m_page3.ShowWindow(SW_SHOW);
		m_page4.ShowWindow(SW_HIDE);
		break;
	case 4:
		m_page0.ShowWindow(SW_HIDE);
		m_page1.ShowWindow(SW_HIDE);
		m_page2.ShowWindow(SW_HIDE);
		m_page3.ShowWindow(SW_HIDE);
		m_page4.ShowWindow(SW_SHOW);
		break;
	default:
		;
	}
	
	*pResult = 0;
}
HWND g_hWnd;
HWND g_hMe;
void CSpyXXDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
    POINT pnt;
	
	RECT rc;
	HWND DeskHwnd = ::GetDesktopWindow();    //ȡ��������
    HDC DeskDC = ::GetWindowDC(DeskHwnd);     //ȡ�������豸����
    
    int oldRop2 = SetROP2(DeskDC, R2_NOTXORPEN);
    ::GetCursorPos(&pnt);                //ȡ���������
    HWND UnHwnd = ::WindowFromPoint(pnt) ;    //ȡ�����ָ�봦���ھ��
	
	g_hWnd=UnHwnd;
    HWND grayHwnd = ::GetWindow(g_hWnd, GW_CHILD);
	RECT tempRc;
	BOOL bFind=FALSE;
    while (grayHwnd)
	{
        ::GetWindowRect(grayHwnd, &tempRc);
		if(::PtInRect(&tempRc,pnt))
		{
			bFind = TRUE;
			break;
		}
        else
            grayHwnd = ::GetWindow(grayHwnd, GW_HWNDNEXT);
        
	}//while
    if(bFind == TRUE)
    {
		bFind= FALSE;
        g_hWnd = grayHwnd;
    }
	else
        ;//Wnd=UnHwnd
	
	::GetWindowRect(g_hWnd, &rc);      //'��ô��ھ���
	
    if( rc.left < 0 ) rc.left = 0;
    if (rc.top < 0 ) rc.top = 0;
    //If() rc.Right > Screen.Width / 15 Then rc.Right = Screen.Width / 15
    //If rc.Bottom > Screen.Height / 15 Then rc.Bottom = Screen.Height / 15
    HPEN newPen = ::CreatePen(0, 3, RGB(125,0,125));    //�����»���,����DeskDC
    HGDIOBJ oldPen = ::SelectObject(DeskDC, newPen);
    ::Rectangle(DeskDC, rc.left, rc.top, rc.right, rc.bottom);  //��ָʾ������Χ��ʾ��˸����
    //::Beep(100,100);
	Sleep(400);    //������˸ʱ����
    ::Rectangle( DeskDC, rc.left, rc.top, rc.right, rc.bottom);
	
    ::SetROP2(DeskDC, oldRop2);
    ::SelectObject( DeskDC, oldPen);
    ::DeleteObject(newPen);
    ::ReleaseDC( DeskHwnd, DeskDC);
	DeskDC = NULL;
	
	
	
	CDialog::OnTimer(nIDEvent);
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	#define IDC_HAND            MAKEINTRESOURCE(32649)

	::SetClassLong(GetDlgItem(IDC_URL)->m_hWnd,GCL_HCURSOR,(LONG)LoadCursor(NULL,IDC_HAND));
	SetDlgItemText(IDC_EDITVC,"713035");
	SetDlgItemText(IDC_EDITVB,"2634364");
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSpyXXDlg::OnChkhex() 
{
	// TODO: Add your control notification handler code here
	g_nHex=((CButton*)GetDlgItem(IDC_CHKHEX))->GetCheck();

	
}

void CSpyXXDlg::OnChktop() 
{
	// TODO: Add your control notification handler code here
	int nTop=((CButton*)GetDlgItem(IDC_CHKTOP))->GetCheck();
	if(nTop==1)
		::SetWindowPos(m_hWnd,HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	else
        ::SetWindowPos(m_hWnd,HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	

	
}

HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if(pWnd->GetDlgCtrlID()==IDC_URL)
		pDC->SetTextColor(RGB(0,0,255));
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CAboutDlg::OnUrl() 
{
	// TODO: Add your control notification handler code here
	ShellExecute(NULL,NULL,"http://www.asanscape.com",NULL,NULL,0);
	
}

BOOL CAboutDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
/*
	if(pMsg->message==WM_MOUSEMOVE)
	{
		POINT pt=pMsg->pt;
		ScreenToClient(&pt);
		CWnd* pWnd=ChildWindowFromPoint(pt);
		UINT nID=pWnd->GetDlgCtrlID();
		switch(nID)
		{
		case IDC_URL:
			::SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR1)));
			break;
		default:
			break;
		}
	
	}
*/
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CAboutDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	::SetClassLong(GetDlgItem(IDC_URL)->m_hWnd,GCL_HCURSOR,(LONG)LoadCursor(NULL,IDC_ARROW));

	
}

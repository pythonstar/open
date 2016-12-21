#include "stdafx.h"
#include "InetDownload.h"
#include "InetDownloadDlg.h"
#include "HttpDownloadDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CAboutDlg)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CAboutDlg)
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
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CInetDownloadDlg::CInetDownloadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInetDownloadDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInetDownloadDlg)
	m_sFile = _T("");
	m_sPassword = _T("");
	m_sURL = _T("");
	m_sUserName = _T("");
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CInetDownloadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInetDownloadDlg)
	DDX_Text(pDX, IDC_FILE, m_sFile);
	DDX_Text(pDX, IDC_PASSWORD, m_sPassword);
	DDX_Text(pDX, IDC_URL, m_sURL);
	DDX_Text(pDX, IDC_USER, m_sUserName);
	//}}AFX_DATA_MAP

  if (pDX->m_bSaveAndValidate)
  {
    if (m_sURL.IsEmpty())
    {
      AfxMessageBox(_T("Please enter a non-empty URL to download"));
      pDX->PrepareEditCtrl(IDC_URL);
      pDX->Fail();
    }
    if (m_sFile.IsEmpty())
    {
      AfxMessageBox(_T("Please specify a file to download into"));
      pDX->PrepareEditCtrl(IDC_FILE);
      pDX->Fail();
    }
  }
}

BEGIN_MESSAGE_MAP(CInetDownloadDlg, CDialog)
	//{{AFX_MSG_MAP(CInetDownloadDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_DOWNLOAD, OnDownload)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CInetDownloadDlg::OnInitDialog()
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

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CInetDownloadDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CInetDownloadDlg::OnPaint() 
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

HCURSOR CInetDownloadDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CInetDownloadDlg::OnDownload() 
{
  if (UpdateData())
  {
    CHttpDownloadDlg dlg;
    dlg.m_sURLToDownload = m_sURL;
    dlg.m_sFileToDownloadInto = m_sFile;

    if (m_sUserName.GetLength())
    {
      dlg.m_sUserName = m_sUserName;
      dlg.m_sPassword = m_sPassword;
    }

    if (dlg.DoModal() == IDOK)
      AfxMessageBox(_T("File has been downloaded"));
  }
}

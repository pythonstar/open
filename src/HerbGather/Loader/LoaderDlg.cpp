// LoaderDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Loader.h"
#include "LoaderDlg.h"
#include "Tlhelp32.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLoaderDlg �Ի���




CLoaderDlg::CLoaderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoaderDlg::IDD, pParent)
	, m_szClassName(_T(""))
	, m_nHandle(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	HMODULE hModule=LoadLibrary("Hook.dll");
	if (hModule)
	{
		StartHook=(TStartHook)GetProcAddress(hModule,"StartHook");
		StopHook=(TStopHook)GetProcAddress(hModule,"StopHook");
	}

	m_bIsDrag		= FALSE;
	m_bMouseDown	= FALSE;
	m_hIconOrig		= NULL;
	m_hIconEmpty	= NULL;
	m_hWndLastFocus	= NULL;
}

void CLoaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FINDER, m_stcFinder);
	DDX_Text(pDX, IDC_EDIT_CLASS_NAME, m_szClassName);
	DDX_Text(pDX, IDC_EDIT_HANDLE, m_nHandle);
}

BEGIN_MESSAGE_MAP(CLoaderDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_HOOK, &CLoaderDlg::OnBnClickedButtonHook)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CLoaderDlg ��Ϣ�������

BOOL CLoaderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	/************************************************************************/
	m_hMapFile=OpenFileMapping(FILE_MAP_WRITE,FALSE,MappingFileName);
	if (m_hMapFile==NULL)
	{
		m_hMapFile=CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,sizeof(SHAREMEM),MappingFileName);
	}
	if (m_hMapFile==NULL)
	{
		::MessageBox(NULL,"���ܽ��������ڴ�!",NULL,MB_OK|MB_ICONERROR);
	}
	m_pShareMem=(PSHAREMEM)MapViewOfFile(m_hMapFile,FILE_MAP_WRITE|FILE_MAP_READ,0,0,0);
	if (m_pShareMem==NULL)
	{
		CloseHandle(m_hMapFile);
		::MessageBox(NULL,"����ӳ�乲���ڴ�!",NULL,MB_OK|MB_ICONERROR);
	}

	/************************************************************************/
	m_hIconOrig  = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ORIG));
	m_hIconEmpty = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_EMPTY));
	m_stcFinder.GetWindowRect(&m_rcFinder);
	ScreenToClient(&m_rcFinder);
	/************************************************************************/
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CLoaderDlg::OnPaint()
{
	CDialog::OnPaint();
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CLoaderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//�ɽ���ID��ȡ��Ӧ�����߳�ID
DWORD GetThreadIdFromPID(DWORD dwProcessId)
{
	HANDLE ThreadHandle;
	THREADENTRY32 ThreadStruct;

	ThreadHandle=CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,dwProcessId);
	ThreadStruct.dwSize=sizeof(ThreadStruct);
	if(Thread32First(ThreadHandle,&ThreadStruct))
	{
		do 
		{
			if (ThreadStruct.th32OwnerProcessID==dwProcessId)
			{
				CloseHandle(ThreadHandle);
				return ThreadStruct.th32ThreadID;
			}
		} while (Thread32Next(ThreadHandle,&ThreadStruct));
	}
	CloseHandle(ThreadHandle);
	return 0;
}

void CLoaderDlg::OnBnClickedButtonHook()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_nHandle)
	{
		DWORD cpid;

		GetWindowThreadProcessId((HWND)m_nHandle,&cpid);
		m_hProcess=OpenProcess(PROCESS_VM_READ,false,cpid);
		m_dwThreadId=GetThreadIdFromPID(cpid);
		if (m_dwThreadId == 0)
		{
			::MessageBox(NULL,"û��ѡ��Ŀ�꣡",NULL,MB_OK|MB_ICONERROR);
		}
		else
		{
			StartHook((HANDLE)m_nHandle,m_dwThreadId);
		}
	}
}

void CLoaderDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_rcFinder.PtInRect(point))
	{
		m_bMouseDown = TRUE;	
		SetCapture();
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CLoaderDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_bIsDrag)
	{
		ReleaseCapture();
		m_bIsDrag = FALSE;
		m_stcFinder.SetIcon(m_hIconOrig);
		if(m_hWndLastFocus)
		{
			::InvalidateRect(NULL,NULL,TRUE);
			m_hWndLastFocus = NULL;
		}
	}
	m_bMouseDown = FALSE;

	CDialog::OnLButtonUp(nFlags, point);
}

void CLoaderDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_bMouseDown)
	{
		::SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_DRAG)));
		m_stcFinder.SetIcon(m_hIconEmpty);
		m_bIsDrag = TRUE;

		//get the window
		CPoint ptCursor(point);
		ClientToScreen(&ptCursor);
		HiliTheWindow(ptCursor);
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CLoaderDlg::HiliTheWindow(CPoint point)
{
	HWND hWnd = ::WindowFromPoint(point);
	if(!hWnd) return;
	DWORD dwProcess = 0;
	GetWindowThreadProcessId(hWnd,&dwProcess);
	if(dwProcess == GetCurrentProcessId()) 
		return;

	GetClassName(hWnd,m_szClassName.GetBuffer(128),128);
	m_szClassName.ReleaseBuffer();
	m_nHandle=(int)hWnd;

	UpdateData(FALSE);

	HDC hdc = ::GetWindowDC(hWnd);
	if(hdc)
	{
		if(m_hWndLastFocus && m_hWndLastFocus != hWnd)
			::InvalidateRect(m_hWndLastFocus,NULL,TRUE); //::RedrawWindow(m_hWndLastFocus,NULL,NULL,RDW_FRAME|RDW_ERASE|RDW_UPDATENOW);
		m_hWndLastFocus = hWnd;

		CRect rcWnd;
		::GetWindowRect(hWnd,rcWnd);
		::MapWindowPoints(NULL,hWnd,(LPPOINT)&rcWnd,2);
		rcWnd.OffsetRect(-rcWnd.left,-rcWnd.top);
		//TRACE2("left %d,top %d,\n",rcWnd.left,rcWnd.top);

		::SelectObject(hdc,::GetStockObject(NULL_BRUSH));
		HPEN hPen = ::CreatePen(PS_SOLID,3,RGB(0,0,0));

		HPEN hPenOld = (HPEN)::SelectObject(hdc,hPen);
		::Rectangle(hdc,rcWnd.left,rcWnd.top,rcWnd.Width(),rcWnd.Height());

		::SelectObject(hdc,hPenOld);
		::DeleteObject(hPenOld);
		::ReleaseDC(hWnd,hdc);
	}
}
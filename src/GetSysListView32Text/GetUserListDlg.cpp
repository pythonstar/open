// GetUserListDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GetUserList.h"
#include "GetUserListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGetUserListDlg 对话框




CGetUserListDlg::CGetUserListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGetUserListDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGetUserListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTNSHOOT, m_btnShoot);
	DDX_Control(pDX, IDC_LIST1, m_lstCopyed);
}

BEGIN_MESSAGE_MAP(CGetUserListDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON2, &CGetUserListDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CGetUserListDlg 消息处理程序

BOOL CGetUserListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_lstCopyed.SetExtendedStyle(LVS_EX_FULLROWSELECT); 

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGetUserListDlg::OnPaint()
{
	CDialog::OnPaint();
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CGetUserListDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGetUserListDlg::OnBnClickedButton2()
{
	CListCtrl lstCtrl;
	CHeaderCtrl *pHeader;
	CString strText;
	DWORD PID;
	HANDLE hProcess;
	int iItem=0; 
	LVITEM lvitem; 
	LV_COLUMN  lvColumn;
	TCHAR ItemBuf[512], *pItemOrColumn, *pBuffer;
	BOOL bRet = FALSE;

	while(m_lstCopyed.DeleteColumn(0));
	m_lstCopyed.DeleteAllItems();

	if (m_btnShoot.m_hHandle)
	{
		lstCtrl.m_hWnd=m_btnShoot.m_hHandle;
		GetWindowThreadProcessId(m_btnShoot.m_hHandle, &PID); 
		hProcess=OpenProcess(PROCESS_ALL_ACCESS,false,PID);
		if (!hProcess)
			MessageBox(_T("获取进程句柄操作失败！"), _T("错误！"), NULL);
		else 
		{ 
			pItemOrColumn = (TCHAR*)VirtualAllocEx(hProcess, NULL, max(sizeof(LVITEM), sizeof(LV_COLUMN)) * 100, MEM_COMMIT, PAGE_READWRITE); 
			pBuffer = (TCHAR*)VirtualAllocEx(hProcess, NULL, 512, MEM_COMMIT, PAGE_READWRITE); 
			if ( pItemOrColumn == NULL || pBuffer == NULL) 
				MessageBox(_T("无法分配内存！"), _T("错误！"), NULL); 
			else 
			{ 
				//获取列数
				pHeader=lstCtrl.GetHeaderCtrl();
				int nColumnCnt=pHeader->GetItemCount();

				//获取列信息
				lvColumn.mask=LVCF_TEXT;
				lvColumn.cchTextMax=512;
				lvColumn.pszText=pBuffer;
				lvColumn.iSubItem=0;
				bRet = WriteProcessMemory(hProcess, pItemOrColumn, &lvColumn, sizeof(LV_COLUMN), NULL);
				if ( bRet==FALSE ) {
					MessageBox(_T("WriteProcessMemory failed!")); 
				}
				for(int i=0; i<nColumnCnt; i++) {
					bRet = ::SendMessage(lstCtrl.m_hWnd, LVM_GETCOLUMN, (WPARAM)i, (LPARAM)pItemOrColumn);
					bRet = ReadProcessMemory(hProcess, pBuffer, ItemBuf, 512, NULL);
					m_lstCopyed.InsertColumn(i, ItemBuf);
					m_lstCopyed.SetColumnWidth(i, 80);
				}

				//获取行信息
				memset(&lvitem, 0, sizeof(LVITEM));
				lvitem.pszText= pBuffer;
				lvitem.cchTextMax = 512;
				for (int i=0;i<lstCtrl.GetItemCount();i++)
				{
					m_lstCopyed.InsertItem(m_lstCopyed.GetItemCount(),NULL);
					lvitem.iItem=i;
					for (int nSubItem=0;nSubItem<nColumnCnt;nSubItem++)
					{
						lvitem.iSubItem = nSubItem;
						bRet = WriteProcessMemory(hProcess, pItemOrColumn, &lvitem, sizeof(LVITEM), NULL);
						bRet = ::SendMessage(lstCtrl.m_hWnd, LVM_GETITEMTEXT, (WPARAM)i, (LPARAM)pItemOrColumn);
						bRet = ReadProcessMemory(hProcess,pBuffer,ItemBuf,512,NULL);
						m_lstCopyed.SetItemText(i,nSubItem,ItemBuf);
					}
				}
				//释放内存 
				CloseHandle(hProcess); 
				VirtualFreeEx(hProcess, pItemOrColumn, 0, MEM_RELEASE); 
				VirtualFreeEx(hProcess, pBuffer, 0, MEM_RELEASE); 
				lstCtrl.m_hWnd=NULL;
			} 
		} 
	}
}

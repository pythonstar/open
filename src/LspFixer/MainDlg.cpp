// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <strsafe.h>

#include "resource.h"

#include "MainDlg.h"
#include "LspFix_Tool.h"
#include "lsp_helper.h"


CMainDlg::CMainDlg()
{
}


CMainDlg::~CMainDlg()
{
	DeleteAllLspData();
}


LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

	// 添加"关于"到菜单项中
	AddAboutToMenu();

	InitListCtrl();
	EnumLsp();
	DlgResize_Init();

	return TRUE;
}


BOOL CMainDlg::EnumLsp()
{
	m_wndLspList.DeleteAllItems();
	DeleteAllLspData();

	// 开始枚举LSP
	if ( !lspfix_tool::EnumLspData( EnumLspCallBack, this ) )
	{
		MessageBox(_T("枚举LSP失败..."), _T("Error"), MB_OK | MB_ICONERROR );
		PostQuitMessage(-1);
		return FALSE;
	}

	return TRUE;
}


void CMainDlg::DeleteAllLspData()
{
	while ( m_vLspInfo.size() )
	{
		PLSP_INFO p = m_vLspInfo.back();
		delete p;
		m_vLspInfo.pop_back();
	}
}


void CMainDlg::InitListCtrl()
{
	m_wndLspList.Attach(GetDlgItem(IDC_LSP_LIST));
	m_wndLspList.InsertColumn( 0, _T("名称"), LVCFMT_LEFT, 190 );
	m_wndLspList.InsertColumn( 1, _T("文件"), LVCFMT_LEFT, 230 );
	m_wndLspList.InsertColumn( 2, _T("文件描述"), LVCFMT_LEFT, 200 );
	m_wndLspList.InsertColumn( 3, _T("出品公司"), LVCFMT_LEFT, 100 );
	m_wndLspList.InsertColumn( 4, _T("CLSID"), LVCFMT_LEFT, 200 );

#ifndef LVS_EX_AUTOSIZECOLUMNS
	#define LVS_EX_AUTOSIZECOLUMNS		0x10000000
#endif
	DWORD dwExStyle =  LVS_EX_AUTOSIZECOLUMNS | LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | 
		LVS_EX_INFOTIP | LVS_EX_GRIDLINES;
	m_wndLspList.SetExtendedListViewStyle( dwExStyle );

	m_font.CreatePointFont( 90, _T("宋体") );
	m_wndLspList.SetFont( m_font );

	m_imglist.Create(16, 16, ILC_COLOR32 | ILC_MASK, 2, 2 );
	m_imglist.AddIcon( LoadIcon(_Module.m_hInst, MAKEINTRESOURCE(IDI_OK)) );
	m_imglist.AddIcon( LoadIcon(_Module.m_hInst, MAKEINTRESOURCE(IDI_UNKNOWN)) );

 	m_wndLspList.SetImageList(m_imglist, LVSIL_SMALL);
}


/*
 * 添加"关于"到菜单项中
 * code by Root
 * E-mail: cppcoffee@gmail.com
*/
void CMainDlg::AddAboutToMenu()
{
	CMenu SysMenu = GetSystemMenu(FALSE);
	if ( !SysMenu.IsMenu() )
		return;

	BOOL bNameValid;
	CString strAboutMenu;
	bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
	_ASSERT(bNameValid);
	if ( !strAboutMenu.IsEmpty() )
	{
		SysMenu.AppendMenu(MF_SEPARATOR);
		SysMenu.AppendMenu(MF_STRING, ID_APP_ABOUT, strAboutMenu);
	}
}


LRESULT CMainDlg::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if ( (wParam & 0xFFF0) == ID_APP_ABOUT )
	{
		CSimpleDialog<IDD_ABOUTBOX> dlg;
		dlg.DoModal();
		return 0;
	}

	bHandled = FALSE;
	return 0;
}


/*
 * 回调函数
 * code by Root
 * E-mail: cppcoffee@gmail.com
*/
BOOL CMainDlg::EnumLspCallBack( void* param,
								const LPCTSTR pszLspName, 
								const LPCTSTR pszFilePath, 
								const LPCTSTR pszClsid )
{
	if ( param == NULL )
		return FALSE;

	CMainDlg* dlg = (CMainDlg*)param;
	CListViewCtrl* list_ctrl = &dlg->m_wndLspList;

	// 是否已知的LSP
	int image_index;
	int item_index;
	BOOL bIsValidLsp = lspfix_tool::IsValidLsp(pszFilePath, pszClsid);
	if ( bIsValidLsp )
	{
		image_index = 0;
		item_index = list_ctrl->GetItemCount();
	}
	else
	{
		image_index = 1;
		item_index = 0;
	}

	list_ctrl->InsertItem( item_index, pszLspName, image_index );

	list_ctrl->SetItemText( item_index, 1, pszFilePath );
	list_ctrl->SetItemText( item_index, 4, pszClsid );

	// 获取文件相关说明信息
	CString file_descript;
	CString company_name;
	lsp_helper::GetFileDescAndCompany( pszFilePath, file_descript, company_name );

	list_ctrl->SetItemText( item_index, 2, file_descript );
	list_ctrl->SetItemText( item_index, 3, company_name );

	PLSP_INFO pInfo = new LSP_INFO;
	pInfo->clsid = pszClsid;
	pInfo->dll_path = pszFilePath;
	pInfo->bIsValidLsp = bIsValidLsp;
	pInfo->lsp_name = pszLspName;
	dlg->m_vLspInfo.push_back(pInfo);

	return TRUE;
}


/*
 * 修复
 * code by Root
 * E-mail: cppcoffee@gmail.com
*/
LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CSimpleDialog<IDD_FIX_QUESTION_DLG> dlg;
	if ( IDCANCEL == dlg.DoModal() )
		return 0;

	lspfix_tool::FixLSP( m_vLspInfo );
	EnumLsp();

	return 0;
}


/*
 * 恢复LSP
 * code by Root
 * E-mail: cppcoffee@gmail.com
*/
LRESULT CMainDlg::OnRestoreLsp(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CSimpleDialog<IDD_RESTORE_QUESTION_DLG> dlg;
	if ( IDCANCEL == dlg.DoModal() )
		return 0;

	lspfix_tool::RestoreLSP();
	EnumLsp();

	return 0;
}


LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}


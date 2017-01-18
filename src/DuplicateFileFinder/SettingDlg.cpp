// SettingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DuplicateFileFinder.h"
#include "SettingDlg.h"
#include "global.h"
#include <StarLib/File/File.h>
#include "FindTypeDlg.h"

CSettingDlg *g_settingDlg=NULL;

// CSettingDlg �Ի���

IMPLEMENT_DYNAMIC(CSettingDlg, CDialog)

CSettingDlg::CSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingDlg::IDD, pParent)
{
	g_settingDlg=this;
}

CSettingDlg::~CSettingDlg()
{
}

void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FILE_TYPE, m_lstFindType);
}


BEGIN_MESSAGE_MAP(CSettingDlg, CDialog)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_FILE_TYPE, &CSettingDlg::OnNMRclickListFileType)
	ON_BN_CLICKED(IDOK, &CSettingDlg::OnBnClickedOk)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FILE_TYPE, &CSettingDlg::OnNMDblclkListFileType)
	ON_BN_CLICKED(IDCANCEL, &CSettingDlg::OnBnClickedCancel)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CSettingDlg ��Ϣ�������
BOOL CSettingDlg::PreTranslateMessage(MSG *pMsg)
{
	if( pMsg->message==WM_KEYDOWN && (pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE ) ){
		return TRUE;
	}else{
		return CDialog::PreTranslateMessage(pMsg);
	}
}

BOOL CSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitAllControls();

	return TRUE;
}

void CSettingDlg::InitAllControls()
{
	m_lstFindType.SetExtendedStyle( m_lstFindType.GetExtendedStyle() | 
		LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT & (~LVS_EX_GRIDLINES) );
	m_lstFindType.ModifyStyle( LVS_LIST | LVS_ICON | LVS_SMALLICON, LVS_REPORT );

	CRect rcList;
	m_lstFindType.GetClientRect(&rcList);
	m_lstFindType.InsertColumn( 0, _T("���ҷ���"), LVCFMT_LEFT, 80 );
	m_lstFindType.InsertColumn( 1, _T("�ļ�����"), LVCFMT_LEFT, rcList.Width()-80-4 );

	CString strConfigFile=GetStartPath()+"config.ini";
	int nCount=Star::File::GetIniInt(strConfigFile,"findtype","count");
	if ( nCount==0 ){	//û�в��ҷ�������Ԥ�ü���
		InitFindType(strConfigFile);
	}

	LoadFindType(strConfigFile);
}

void CSettingDlg::InitFindType(CString strConfigFile)
{
	Star::File::SetIniString(strConfigFile,"findtype","1",
		"�����ļ�|*.*");
	Star::File::SetIniString(strConfigFile,"findtype","2",
		"ͼƬ�ļ�|*.jpg|*.bmp|*.gif|*.png");
	Star::File::SetIniString(strConfigFile,"findtype","3",
		"ѹ�����ļ�|*.rar|*.zip|*.7z");
	Star::File::SetIniString(strConfigFile,"findtype","4",
		"��ʱ�ļ�|*.tmp");
	Star::File::SetIniString(strConfigFile,"findtype","5",
		"�ĵ�����|*.doc|*.docx|*.xls|*.ppt");
	Star::File::SetIniInt(strConfigFile,"findtype","count",5);
}

void CSettingDlg::LoadFindType(CString strConfigFile)
{
	CString strKey;
	CString strText;
	CString strTypeDesc;
	CString strTypeFilter;
	int nPos=-1;

	m_lstFindType.DeleteAllItems();
	int nCount=Star::File::GetIniInt(strConfigFile,"findtype","count");
	for ( int i=0; i<nCount; ++i ){
		strKey.Format("%d",i+1);
		strText=Star::File::GetIniString(strConfigFile,"findtype",strKey);
		nPos=strText.Find('|');
		if ( nPos==-1 ){
			continue;
		}

		strTypeDesc=strText.Left(nPos);
		strTypeFilter=strText.Mid(nPos+1);
		int nItem=m_lstFindType.InsertItem(m_lstFindType.GetItemCount(),strTypeDesc);
		m_lstFindType.SetItemText(nItem,1,strTypeFilter);
	}
}

void CSettingDlg::SaveFindType()
{
	CString strKey;
	CString strText;
	CString strConfigFile=GetStartPath()+"config.ini";
	int nCount=m_lstFindType.GetItemCount();
	Star::File::SetIniInt(strConfigFile,"findtype","count",nCount);

	for ( int nItem=0; nItem<nCount; ++nItem ){
		strKey.Format("%d",nItem+1);
		strText=m_lstFindType.GetItemText(nItem,0)+"|"+
			m_lstFindType.GetItemText(nItem,1);
		Star::File::SetIniString(strConfigFile,"findtype",strKey,strText);
	}
}

void CSettingDlg::OnNMRclickListFileType(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem=pNMListView->iItem;
	int nSubItem=pNMListView->iSubItem;

	//���������˵�
	CMenu m_popmenu;
	m_popmenu.LoadMenu(IDR_MENU_FIND_TYPE);	
	CMenu *psub = (CMenu*)m_popmenu.GetSubMenu(0);
	CPoint pt;
	GetCursorPos(&pt);
	DWORD dwID =psub->TrackPopupMenu((TPM_LEFTALIGN|TPM_RIGHTBUTTON|TPM_NONOTIFY|TPM_RETURNCMD),
		pt.x,pt.y, this);

	if ( dwID==ID_MENU_ADD_FIND_TYPE ){
		CFindTypeDlg dlg;
		if ( dlg.DoModal()==IDOK ){
			nItem=m_lstFindType.InsertItem(m_lstFindType.GetItemCount(),
				dlg.m_strFindTypeDesc);
			m_lstFindType.SetItemText(nItem,1,dlg.m_strFindTypeFilter);
		}
	}else if ( dwID==ID_MENU_DEL_FIND_TYPE ){
		m_lstFindType.DeleteItem(nItem);
	}else if ( dwID==ID_MENU_UPDATE_FIND_TYPE ){
		OnUpdateFindType(pNMListView->iItem);
	}

	m_popmenu.DestroyMenu();

	*pResult = 0;
}

void CSettingDlg::OnBnClickedOk()
{
	SaveFindType();
	OnOK();
}

void CSettingDlg::OnNMDblclkListFileType(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	OnUpdateFindType(pNMListView->iItem);

	*pResult = 0;
}

void CSettingDlg::OnUpdateFindType(int nItem)
{
	CFindTypeDlg dlg;
	dlg.m_strFindTypeDesc=m_lstFindType.GetItemText(nItem,0);
	dlg.m_strFindTypeFilter=m_lstFindType.GetItemText(nItem,1);
	if ( dlg.DoModal()==IDOK ){
		m_lstFindType.SetItemText(nItem,0,dlg.m_strFindTypeDesc);
		m_lstFindType.SetItemText(nItem,1,dlg.m_strFindTypeFilter);
	}
}

void CSettingDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//OnCancel();
}

void CSettingDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
}

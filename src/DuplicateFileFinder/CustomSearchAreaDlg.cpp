// CustomSearchAreaDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DuplicateFileFinder.h"
#include "CustomSearchAreaDlg.h"
#include <StarLib/File/File.h>
#include "global.h"
#include <Shlwapi.h>
#include <StarLib/PathDialog/PathDialog.h>

// CCustomSearchAreaDlg 对话框

IMPLEMENT_DYNAMIC(CCustomSearchAreaDlg, CDialog)

CCustomSearchAreaDlg::CCustomSearchAreaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCustomSearchAreaDlg::IDD, pParent)
{
	m_pFindAreaList=NULL;
}

CCustomSearchAreaDlg::~CCustomSearchAreaDlg()
{
}

void CCustomSearchAreaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_AREA, m_lstArea);
}


BEGIN_MESSAGE_MAP(CCustomSearchAreaDlg, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_LIST_AREA, &CCustomSearchAreaDlg::OnNMClickListArea)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON_ADD_FOLDER, &CCustomSearchAreaDlg::OnBnClickedButtonAddFolder)
	ON_BN_CLICKED(IDOK, &CCustomSearchAreaDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_DEL_FOLDER, &CCustomSearchAreaDlg::OnBnClickedButtonDelFolder)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_AREA, &CCustomSearchAreaDlg::OnNMRclickListArea)
END_MESSAGE_MAP()


// CCustomSearchAreaDlg 消息处理程序

BOOL CCustomSearchAreaDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	InitAllControls();
	DragAcceptFiles();

	return TRUE;
}

void CCustomSearchAreaDlg::PostNcDestroy()
{
	//m_imagelist.Detach();
}

void CCustomSearchAreaDlg::InitAllControls()
{
	CString strText;

	CRect rcList;
	m_lstArea.SetExtendedStyle( m_lstArea.GetExtendedStyle() | 
		LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT & (~LVS_EX_GRIDLINES) );
	m_lstArea.ModifyStyle( LVS_LIST | LVS_ICON | LVS_SMALLICON, LVS_REPORT );
	m_lstArea.GetClientRect(&rcList);
	m_lstArea.InsertColumn( 0, _T(""), LVCFMT_LEFT, rcList.Width()-4 );

	SHFILEINFO sfi={};
	m_lstArea.SetImageList(m_imagelist,LVSIL_SMALL);

	m_lstArea.DeleteAllItems();
	CString strConfigFile=GetStartPath()+"config.ini";
	CString strFolderPath;
	int nPos;
	int nFlag;
	int nCount=Star::File::GetIniInt(strConfigFile,"searcharea","count");
	if ( nCount==0 ){
		CStringList lstDriver;
		GetDirverList(lstDriver);
		POSITION pos=lstDriver.GetHeadPosition();
		while ( pos ){
			strFolderPath=lstDriver.GetNext(pos);
			::SHGetFileInfo(strFolderPath,0,&sfi,sizeof(sfi),
				SHGFI_SYSICONINDEX|SHGFI_SMALLICON);
			int nItem=m_lstArea.InsertItem(LVIF_TEXT|LVIF_IMAGE,
				m_lstArea.GetItemCount(),strFolderPath,0,0,sfi.iIcon,0);
			m_lstArea.SetCheck(nItem,TRUE);
		}
	}else{
		for ( int i=0; i<nCount; ++i ){
			strText.Format("%d",i+1);
			strText=Star::File::GetIniString(strConfigFile,"searcharea",strText);
			nPos=strText.Find("|");
			if ( nPos!=-1 ){
				strFolderPath=strText.Left(nPos);
				strText=strText.Mid(nPos+1);
				nFlag=StrToInt(strText);
			}else{
				strFolderPath=strText;
				nFlag=0;
			}

			if ( GetFileAttributes(strFolderPath)!=-1 ){
				::SHGetFileInfo(strFolderPath,0,&sfi,sizeof(sfi),
					SHGFI_SYSICONINDEX|SHGFI_SMALLICON);
				int nItem=m_lstArea.InsertItem(LVIF_TEXT|LVIF_IMAGE,
					m_lstArea.GetItemCount(),strFolderPath,0,0,sfi.iIcon,0);
				m_lstArea.SetCheck(nItem,nFlag!=0);
			}
		}
	}

// 	::SHGetFileInfo("c:\\",0,&sfi,sizeof(sfi),
// 		SHGFI_SYSICONINDEX|SHGFI_SMALLICON);
	//m_lstCtrl.InsertItem(LVIF_TEXT|LVIF_IMAGE,nItem,strText,0,0,sfi.iIcon,0);
}

void CCustomSearchAreaDlg::OnNMClickListArea(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

// 	LVHITTESTINFO hitinfo;
// 
// 	//Copy click point
// 	hitinfo.pt = pNMListView->ptAction;
// 
// 	//Make the hit test...
// 	int nItem = m_lstArea.HitTest(&hitinfo);
// 	if ( nItem!=-1 ){
// 		// Didn't click on an icon
// 		if(hitinfo.flags != LVHT_ONITEMSTATEICON){
// 			BOOL bChecked=m_lstArea.GetCheck(nItem);
// 			m_lstArea.SetCheck(nItem,!bChecked);
// 		}
// 	}

	*pResult = 0;
}

void CCustomSearchAreaDlg::OnDropFiles(HDROP hDropInfo)
{
	char szFileName[MAX_PATH + 1] = {0};
	UINT nFiles = DragQueryFile(hDropInfo, -1, NULL, 0);
	for( UINT i=0; i<nFiles; ++i ){
		DragQueryFile(hDropInfo, i, szFileName, MAX_PATH);
		DWORD dwAttribute=GetFileAttributes(szFileName);
		if ( dwAttribute!=-1 && (dwAttribute&FILE_ATTRIBUTE_DIRECTORY)!=0  ){
			BOOL bFound=FALSE;
			int nItem=0;
			for ( nItem=0; nItem<m_lstArea.GetItemCount(); ++nItem ){
				if ( m_lstArea.GetItemText(nItem,0).CompareNoCase(szFileName)==0 ){
					bFound=TRUE;
					break;
				}
			}

			if ( bFound==FALSE ){
				SHFILEINFO sfi={};
				::SHGetFileInfo(szFileName,0,&sfi,sizeof(sfi),
					SHGFI_SYSICONINDEX|SHGFI_SMALLICON);
				nItem=m_lstArea.InsertItem(LVIF_TEXT|LVIF_IMAGE,
					m_lstArea.GetItemCount(),szFileName,0,0,sfi.iIcon,0);
				m_lstArea.SetCheck(nItem,TRUE);
			}else{
				m_lstArea.SetCheck(nItem,TRUE);
			}
		}
	}
	DragFinish(hDropInfo);

	CDialog::OnDropFiles(hDropInfo);
}

//添加目录
void CCustomSearchAreaDlg::OnBnClickedButtonAddFolder()
{
	CString strFolderPath;
	CPathDialog dlg(APPNAME_CH, "请选择目录", "");

	if (dlg.DoModal()==IDOK){
		strFolderPath=dlg.GetPathName();
		BOOL bFound=FALSE;
		int nItem=0;
		for ( nItem=0; nItem<m_lstArea.GetItemCount(); ++nItem ){
			if ( m_lstArea.GetItemText(nItem,0).CompareNoCase(strFolderPath)==0 ){
				bFound=TRUE;
				break;
			}
		}

		if ( bFound==FALSE ){
			SHFILEINFO sfi={};
			::SHGetFileInfo(strFolderPath,0,&sfi,sizeof(sfi),
				SHGFI_SYSICONINDEX|SHGFI_SMALLICON);
			nItem=m_lstArea.InsertItem(LVIF_TEXT|LVIF_IMAGE,
				m_lstArea.GetItemCount(),strFolderPath,0,0,sfi.iIcon,0);
			m_lstArea.SetCheck(nItem,TRUE);
		}else{
			m_lstArea.SetCheck(nItem,TRUE);
		}
	}
}

//删除目录
void CCustomSearchAreaDlg::OnBnClickedButtonDelFolder()
{
	int nItem=m_lstArea.GetSelectionMark();
	if ( nItem<0 ){
		AfxMessageBox("没有选中任何项目!");
		return;
	}

	m_lstArea.DeleteItem(nItem);
}

void CCustomSearchAreaDlg::OnBnClickedOk()
{
	CString strConfigFile=GetStartPath()+"config.ini";
	CString strText;
	CString strTemp;

	Star::File::SetIniInt(strConfigFile,"searcharea","count",m_lstArea.GetItemCount());
	for ( int nItem=0; nItem<m_lstArea.GetItemCount(); ++nItem ){
		strText.Format("%d",nItem+1);
		strTemp.Format("%s|%d",m_lstArea.GetItemText(nItem,0),m_lstArea.GetCheck(nItem));
		Star::File::SetIniString(strConfigFile,"searcharea",strText,strTemp);

		if ( m_lstArea.GetCheck(nItem)==TRUE ){
			m_pFindAreaList->AddTail( m_lstArea.GetItemText(nItem,0) );
		}
	}

	if ( m_pFindAreaList->GetCount()==0 ){
		AfxMessageBox("没有勾选查找目录，请重新设置!");
		return;
	}

	OnOK();
}

void CCustomSearchAreaDlg::OnNMRclickListArea(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem=pNMListView->iItem;
	int nSubItem=pNMListView->iSubItem;

	//弹出操作菜单
	CMenu m_popmenu;
	m_popmenu.LoadMenu(IDR_MENU_AREA_OPERATION);	
	CMenu *psub = (CMenu*)m_popmenu.GetSubMenu(0);
	CPoint pt;
	GetCursorPos(&pt);
	DWORD dwID =psub->TrackPopupMenu((TPM_LEFTALIGN|TPM_RIGHTBUTTON|TPM_NONOTIFY|TPM_RETURNCMD),
		pt.x,pt.y, this);

	if ( dwID==IDC_BUTTON_ADD_FOLDER ){
		OnBnClickedButtonAddFolder();
	}else if ( dwID==IDC_BUTTON_DEL_FOLDER ){
		OnBnClickedButtonDelFolder();
	}

	m_popmenu.DestroyMenu();

	*pResult = 0;
}

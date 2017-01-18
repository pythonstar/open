// MainDlg.cpp : 实现文件
//

#include "stdafx.h"
#include <afxmt.h>
#include "global.h"
#include "DuplicateFileFinder.h"
#include "MainDlg.h"
#include "CustomSearchAreaDlg.h"
#include <algorithm>
#include <functional>
#include <StarLib/MD5/Md5.h>

/************************************************************************/
CMainDlg*g_pMainDlg=NULL;
CEvent CMainDlg::m_eventStop(true,true);
int CMainDlg::m_nRunningFlag=0;
int CMainDlg::m_nFileTotal=0;
CSqliteDB CMainDlg::m_sqlitedb;

CString CMainDlg::m_strDupFileName;
CString CMainDlg::m_strSrcFileName;

vector<CString>CMainDlg::m_vtFileTypeFilter;
/************************************************************************/

struct find_filter: binary_function<CString,CString,bool>
{
	bool operator()( CString &value, CString filter) const
	{
		return ( value.CompareNoCase(filter)==0 );
	}
};

// CMainDlg 对话框

IMPLEMENT_DYNAMIC(CMainDlg, CDialog)

CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
{

}

CMainDlg::~CMainDlg()
{
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ALBUM, m_lstFile);
	DDX_Control(pDX, IDC_CHECK_SELECT_ALL, m_chkSelectAll);
	DDX_Control(pDX, IDC_STATIC_CURRENT_FILE, m_stcCurrentMsg);
	DDX_Control(pDX, IDC_ANIMATE1, m_animate);
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	ON_MESSAGE(WM_FINDONEDUPFILE,&CMainDlg::OnFindOneDupFile)
	ON_MESSAGE(WM_FINDSTART,&CMainDlg::OnFindDupFileStart)
	ON_MESSAGE(WM_FINDEND,&CMainDlg::OnFindDupFileEnd)
	ON_NOTIFY(NM_CLICK, IDC_LIST_ALBUM, &CMainDlg::OnNMClickListAlbum)
	ON_BN_CLICKED(IDC_CHECK_SELECT_ALL, &CMainDlg::OnBnClickedCheckSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_SETTING, &CMainDlg::OnBnClickedSetting)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH_ALL, &CMainDlg::OnBnClickedButtonSearchAll)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH_CUSTOM, &CMainDlg::OnBnClickedButtonSearchCustom)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, &CMainDlg::OnBnClickedButtonPause)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CMainDlg::OnBnClickedButtonStop)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_ALBUM, &CMainDlg::OnNMRclickListAlbum)
END_MESSAGE_MAP()


// CMainDlg 消息处理程序
BOOL CMainDlg::PreTranslateMessage(MSG *pMsg)
{
	if( pMsg->message==WM_KEYDOWN && (pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE ) ){
		return TRUE;
	}else{
		return CDialog::PreTranslateMessage(pMsg);
	}
}

BOOL CMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitAllControls();
	m_bInited=TRUE;

	return TRUE;
}

void CMainDlg::InitAllControls()
{
	CString strText;
	g_pMainDlg=this;

	strText.Format( "%s v%s", APPNAME_CH, STRFILEVER );
	this->SetWindowText(strText);

	m_strApplicationPath=GetStartPath();
	//创建一个临时目录
	CString strTempPath=m_strApplicationPath+"tmp\\";
	if ( GetFileAttributes(strTempPath)==-1 ){
		::CreateDirectory(strTempPath,NULL);
	}

	m_lstFile.SetExtendedStyle( m_lstFile.GetExtendedStyle() | 
		LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT & (~LVS_EX_GRIDLINES) );
	m_lstFile.ModifyStyle( LVS_LIST | LVS_ICON | LVS_SMALLICON, LVS_REPORT );

	CRect rcList;
	m_lstFile.GetClientRect(&rcList);
	m_lstFile.InsertColumn( 0, _T("重复文件"), LVCFMT_LEFT, rcList.Width()/2 );
	m_lstFile.InsertColumn( 1, _T("源文件"), LVCFMT_LEFT, rcList.Width()/2 );

	SHFILEINFO sfi={};
	LPITEMIDLIST spidl;
	HRESULT hr=::SHGetSpecialFolderLocation(m_hWnd,CSIDL_DESKTOP,&spidl);
	HIMAGELIST hImageListSmall=(HIMAGELIST)::SHGetFileInfo((LPCTSTR)spidl,0,&sfi,sizeof(sfi),
		SHGFI_PIDL|SHGFI_SYSICONINDEX|SHGFI_SMALLICON);
	//m_imagelist.Create(IMAGE_WIDTH,IMAGE_HEIGHT,ILC_COLOR32,10,1);
	m_imageList.Attach(hImageListSmall);
	m_lstFile.SetImageList(&m_imageList,LVSIL_SMALL);

	/************************************************************************/

	GetDlgItem(IDC_BUTTON_PAUSE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	m_animate.Open(IDR_AVI_FIND_FILE);

	UpdateData(FALSE);
}


void CMainDlg::OnNMClickListAlbum(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// 	int nIndex=pNMListView->iItem;
	// 	if ( nIndex!=-1 ){
	// 		
	// 	}

	LVHITTESTINFO hitinfo;

	//Copy click point
	hitinfo.pt = pNMListView->ptAction;

	//Make the hit test...
	int nItem = m_lstFile.HitTest(&hitinfo);
	if ( nItem!=-1 ){
		// Didn't click on an icon
		if(hitinfo.flags != LVHT_ONITEMSTATEICON){
			BOOL bChecked=m_lstFile.GetCheck(nItem);
			m_lstFile.SetCheck(nItem,!bChecked);
		}
	}

	*pResult = 0;
}

void CMainDlg::OnBnClickedCheckSelectAll()
{
	CButton*pButton=(CButton*)GetDlgItem(IDC_CHECK_SELECT_ALL);
	if ( pButton!=NULL ){
		BOOL bChecked=(pButton->GetCheck()==BST_CHECKED); 
		for ( int nItem=0; nItem<m_lstFile.GetItemCount(); ++nItem ){
			m_lstFile.SetCheck(nItem,bChecked);
		}
	}
}

void CMainDlg::OnBnClickedSetting()
{
	CSettingDlg dlg;
	dlg.DoModal();
}

void CMainDlg::OnBnClickedButtonSearchAll()
{
	AfxBeginThread(ThreadFindDupFile,NULL);
}

void CMainDlg::OnBnClickedButtonSearchCustom()
{
	CStringList *pFindAreaList=new CStringList;
	CCustomSearchAreaDlg dlg;
	dlg.m_pFindAreaList=pFindAreaList;
	dlg.m_imagelist=&m_imageList;
	if ( dlg.DoModal()==IDOK && pFindAreaList->GetCount()>0 ){
		AfxBeginThread(ThreadFindDupFile,pFindAreaList);
	}else{
		delete pFindAreaList;
	}
}

LRESULT CMainDlg::OnFindDupFileStart(WPARAM wParam,LPARAM lParam)
{
	CString strText;
	CString strTemp;
	int nPos1=0;
	int nPos2=0;

	m_animate.Play(0,-1,-1);
	GetDlgItem(IDC_BUTTON_SEARCH_ALL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SEARCH_CUSTOM)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_PAUSE)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_PAUSE)->SetWindowText("暂停");
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
	m_lstFile.DeleteAllItems();
	m_eventStop.SetEvent();
	InitDbFile();

	//初始化查找文件过滤类型
	m_vtFileTypeFilter.clear();
	CListCtrl *pListCtrl=&g_settingDlg->m_lstFindType;
	for ( int nItem=0; nItem<pListCtrl->GetItemCount(); ++nItem ){
		if ( pListCtrl->GetCheck(nItem)==TRUE ){
			strText=pListCtrl->GetItemText(nItem,1);
			nPos1=strText.Find('.');
			while ( nPos1!=-1 ){
				nPos2=strText.Find('|',nPos1);
				if ( nPos2==-1 ){
					m_vtFileTypeFilter.push_back( strText.Mid(nPos1) );
					break;
				}else{
					m_vtFileTypeFilter.push_back( strText.Mid(nPos1,nPos2-nPos1) );
					nPos1=strText.Find('.',nPos2);
				}
			}
		}
	}

	if ( find_if(m_vtFileTypeFilter.begin(),m_vtFileTypeFilter.end(),
		bind2nd(find_filter(),".*"))!=m_vtFileTypeFilter.end() ){
			//说明设置了查找全部文件
			m_vtFileTypeFilter.clear();
	}

	return 0;
}

LRESULT CMainDlg::OnFindDupFileEnd(WPARAM wParam,LPARAM lParam)
{
	FreeDbFile();
	m_stcCurrentMsg.SetWindowText("查找结束");
	GetDlgItem(IDC_BUTTON_SEARCH_ALL)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_SEARCH_CUSTOM)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_PAUSE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	m_animate.Stop();

	return 0;
}

UINT CMainDlg::ThreadFindDupFile(LPVOID lpParam)
{
	g_pMainDlg->SendMessage(WM_FINDSTART,0,0);

	CStringList *pFindAreaList=NULL;

	if ( lpParam==NULL ){
		pFindAreaList=new CStringList;
		GetDirverList(*pFindAreaList);
	}else{
		pFindAreaList=(CStringList *)lpParam;
	}

	m_nFileTotal=0;
	m_nRunningFlag=0;

	POSITION pos=pFindAreaList->GetHeadPosition();
	while ( pos!=NULL ){
		FindDupFile(pFindAreaList->GetNext(pos));
	}

	if ( pFindAreaList!=NULL ){
		delete pFindAreaList;
	}

	g_pMainDlg->SendMessage(WM_FINDEND,0,0);


	return 0;
}

void CMainDlg::InitDbFile()
{
	bool bFirstCreate=GetFileAttributes(GetStartPath()+"file.db")==-1;
	m_sqlitedb.Connect( GetStartPath()+"file.db" );
	if(bFirstCreate==TRUE){
		m_sqlitedb.ExecuteSQL(
			"CREATE TABLE file ("
			"	hash CHAR(32) PRIMARY KEY,"
			"	name VARCHAR(100)"
			");"
			"create index indexhash on file(hash);"
			);
	}

	m_sqlitedb.BeginTransaction();
}

void CMainDlg::FreeDbFile()
{
	m_sqlitedb.CommitTransaction();
	m_sqlitedb.Disconnect();
	DeleteFile(GetStartPath()+"file.db");
}

void CMainDlg::FindDupFile(CString&strFolder)
{
	CFileFind finder; 
	CString strFinderFilter;

	strFinderFilter.Format("%s*.*",strFolder);
	BOOL bWorking = finder.FindFile(strFinderFilter); 
	while ( bWorking==TRUE ){
		if ( SignalToStop(&m_eventStop)==TRUE ){
			//线程需要暂停或停止，检测标志位
			if ( m_nRunningFlag==1 ){//需要暂停
				::WaitForSingleObject(m_eventStop.m_hObject,INFINITE);
				if ( m_nRunningFlag==2 ){
					break;	//在暂停的时候命令停止
				}
			}else if ( m_nRunningFlag==2 ){//需要停止
				break;
			}
		}

		bWorking = finder.FindNextFile(); 
		if (finder.IsDots()){
			continue;
		}else if ( finder.IsDirectory()==TRUE ){
			FindDupFile(finder.GetFilePath()+"\\");
		}else{ 
			++m_nFileTotal;
			CString strFileName=finder.GetFilePath();
			int nPos=strFileName.ReverseFind('.');
			if ( nPos!=-1 ){
				CString strFileExt=strFileName.Mid(nPos);
				if ( m_vtFileTypeFilter.empty() || 
					find_if(m_vtFileTypeFilter.begin(),m_vtFileTypeFilter.end(),
					bind2nd(find_filter(),strFileExt))!=m_vtFileTypeFilter.end() ){
						g_pMainDlg->m_stcCurrentMsg.SetWindowText("正在检测文件："+strFileName);
						AnalyzeFile(strFileName);
				}else{
					if ( m_nFileTotal%10==0 ){
						g_pMainDlg->m_stcCurrentMsg.SetWindowText("正在检测文件："+strFileName);
					}
				}
			}

		} 
	}

	finder.Close();
}

void CMainDlg::AnalyzeFile(CString&strFileName)
{
	CString strText;
	CString strMd5;
	CStringA strSQL;

	strMd5 = Md5File(strFileName);

	int result;
	char*szErrorMsg = NULL;
	char**dbResult;
	int nRow, nColumn;

	strSQL.Format( "SELECT name FROM file WHERE hash='%s';", strMd5 );
	result = sqlite3_get_table( m_sqlitedb.m_pDB, strSQL, &dbResult, &nRow, &nColumn, &szErrorMsg );
	if(result!=SQLITE_OK){
		TRACE(szErrorMsg);
		sqlite3_free(szErrorMsg);
		return;
	}

	if ( dbResult==NULL )
		return;

	if( nRow > 0 ){
		//重复文件找到
		m_strDupFileName=strFileName;
		m_strSrcFileName=dbResult[1];
		g_pMainDlg->SendMessage(WM_FINDONEDUPFILE,0,0);
	}else{
		//记录不存在，直接插入
		strSQL.Format( "INSERT INTO file (hash,name) VALUES('%s','%s');",
			strMd5, strFileName );
		m_sqlitedb.ExecuteSQL(strSQL);
	}

	sqlite3_free_table(dbResult);

}

LRESULT CMainDlg::OnFindOneDupFile(WPARAM wParam,LPARAM lParam)
{
	SHFILEINFO sfi={};
	::SHGetFileInfo((LPCSTR)(LPCTSTR)m_strDupFileName,0,&sfi,sizeof(sfi),
		SHGFI_SYSICONINDEX|SHGFI_SMALLICON);
	int nItem=m_lstFile.InsertItem(LVIF_TEXT|LVIF_IMAGE,m_lstFile.GetItemCount(),
		m_strDupFileName,0,0,sfi.iIcon,0);
	m_lstFile.SetItemText(nItem,1,m_strSrcFileName);

	return 0;
}

//暂停查找
void CMainDlg::OnBnClickedButtonPause()
{
	CString strText;
	CButton *pButton=(CButton *)GetDlgItem(IDC_BUTTON_PAUSE);
	pButton->GetWindowText(strText);
	if ( strText=="暂停" ){
		m_nRunningFlag=1;
		m_eventStop.ResetEvent();
		pButton->SetWindowText("继续");
		m_animate.Stop();
	}else{
		m_nRunningFlag=0;
		m_eventStop.SetEvent();
		pButton->SetWindowText("暂停");
		m_animate.Play(0,-1,-1);
	}
}

//停止查找
void CMainDlg::OnBnClickedButtonStop()
{
	m_nRunningFlag=2;
	m_eventStop.SetEvent();
	m_eventStop.ResetEvent();
}

void CMainDlg::OnMenuLocateFile(int nItem,int nSubItem)
{
	// 	int nItem=m_lstFile.GetSelectionMark();
	// 	if ( nItem<0 ){
	// 		AfxMessageBox("没有选中任何项目!");
	// 		return;
	// 	}

	CString strFileName=m_lstFile.GetItemText(nItem,nSubItem);
	if ( GetFileAttributes(strFileName)==-1 ){	//文件不存在打开它所在的目录
		CString strFolder="";
		int nPos=strFileName.ReverseFind('\\');
		if ( nPos!=-1 ){
			strFolder=strFileName.Left(nPos);
		}

		if ( GetFileAttributes(strFolder)==-1 ){
			AfxMessageBox("以下文件不存在，可能已经被删除了：\n"+strFileName);
		}else{
			ShellExecute(NULL,"open","explorer.exe",strFolder,NULL,SW_NORMAL);
		}
	}else{
		CString strCmdLine;
		strCmdLine.Format( "/select, \"%s\"", strFileName );
		ShellExecute(NULL,"open","explorer.exe",strCmdLine,NULL,SW_NORMAL);
	}
}

void CMainDlg::OnNMRclickListAlbum(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem=pNMListView->iItem;
	int nSubItem=pNMListView->iSubItem;

	//弹出操作菜单
	CMenu m_popmenu;
	m_popmenu.LoadMenu(IDR_MENU_FILE_OPERATION);	
	CMenu *psub = (CMenu*)m_popmenu.GetSubMenu(0);
	CPoint pt;
	GetCursorPos(&pt);
	DWORD dwID =psub->TrackPopupMenu((TPM_LEFTALIGN|TPM_RIGHTBUTTON|TPM_NONOTIFY|TPM_RETURNCMD),
		pt.x,pt.y, this);

	if ( dwID==ID_MENU_LOCATE_FILE ){
		OnMenuLocateFile(nItem,nSubItem);
	}else if ( dwID==ID_MENU_DEL_FILE ){
		CString strFileName=m_lstFile.GetItemText(nItem,nSubItem);
		if ( ::AfxMessageBox("您确定要删除以下文件吗？\n"+strFileName,
			MB_YESNO)==IDYES ){
				::DeleteFile(strFileName);
		}
	}

	m_popmenu.DestroyMenu();

	*pResult = 0;
}

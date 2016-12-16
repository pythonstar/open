// HerbGatherEditorDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HerbGatherEditor.h"
#include "HerbGatherEditorDlg.h"
#include "PreviewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHerbGatherEditorDlg 对话框




CHerbGatherEditorDlg::CHerbGatherEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHerbGatherEditorDlg::IDD, pParent)
	, m_strPosx(_T("0"))
	, m_strPosy(_T("0"))
	, m_strMethodName(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHerbGatherEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST3, m_lstGatherMethod);
	DDX_Control(pDX, IDC_LIST2, m_lstHerbPlace);
	DDX_Text(pDX, IDC_EDIT1, m_strPosx);
	DDX_Text(pDX, IDC_EDIT2, m_strPosy);
	DDX_Text(pDX, IDC_EDIT3, m_strMethodName);
	DDX_Control(pDX, IDC_BUTTON3, m_btnPosCmd);
}

BEGIN_MESSAGE_MAP(CHerbGatherEditorDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON2, &CHerbGatherEditorDlg::OnBnClickedButton2)
	ON_NOTIFY(NM_CLICK, IDC_LIST3, &CHerbGatherEditorDlg::OnNMClickList3)
	ON_BN_CLICKED(IDC_BUTTON3, &CHerbGatherEditorDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CHerbGatherEditorDlg::OnBnClickedButton4)
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, &CHerbGatherEditorDlg::OnNMRclickList2)
	ON_COMMAND(ID_ADD_POSITION, &CHerbGatherEditorDlg::OnAddPosition)
	ON_COMMAND(ID_ALTER_POSITION, &CHerbGatherEditorDlg::OnAlterPosition)
	ON_COMMAND(ID_DELETE_POSITION, &CHerbGatherEditorDlg::OnDeletePosition)
	ON_COMMAND(ID_DELETE_METHOD, &CHerbGatherEditorDlg::OnDeleteMethod)
	ON_COMMAND(ID_PREVIEW_METHOD, &CHerbGatherEditorDlg::OnPreviewMethod)
	ON_NOTIFY(NM_RCLICK, IDC_LIST3, &CHerbGatherEditorDlg::OnNMRclickList3)
END_MESSAGE_MAP()


// CHerbGatherEditorDlg 消息处理程序

BOOL CHerbGatherEditorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	char szBuff[MAX_PATH*2];
	GetModuleFileName(NULL,szBuff,sizeof(szBuff));
	strrchr(szBuff,'\\')[1]=0;

	m_strMethodPath=szBuff;
	m_strMethodPath=m_strMethodPath+HERB_GATHER_METHOD_PATH+"\\";
	InitHerbGatherMethod(m_strMethodPath);
	InitHerbPlaceList();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
void CHerbGatherEditorDlg::InitHerbGatherMethod(CString strPath)
{
	m_lstGatherMethod.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES );
	m_lstGatherMethod.InsertColumn(0,"序号",LVCFMT_LEFT,40);
	m_lstGatherMethod.InsertColumn(1,"名称",LVCFMT_LEFT,120);
	//m_lstGatherMethod.InsertColumn(-1,"文件",LVCFMT_LEFT,100);

	m_nMethodCnt=0;
	while(TRUE)
	{
		CString strFile;
		strFile.Format("%s%s%d.dat",strPath,HERB_GATHER_METHOD_FILE,m_nMethodCnt+1);
		if (PathFileExists(strFile))
		{
			CFile file(strFile,CFile::modeRead);
			CArchive ar(&file,CArchive::load);
			CString strMethodName;

			int nItem=m_lstGatherMethod.InsertItem(m_lstGatherMethod.GetItemCount(),NULL);
			CString strText;
			strText.Format("%d",m_nMethodCnt+1);
			m_lstGatherMethod.SetItemText(nItem,0,strText);
			
			if (ar.IsLoading())
				ar>>strMethodName;
			m_lstGatherMethod.SetItemText(nItem,1,strMethodName);

			ar.Close();
			file.Close();

			m_nMethodCnt++;
		}
		else
		{
			break;
		}
	}
	if (m_nMethodCnt==0)
	{
		MessageBox("尚未创建采药配置方案！","提示",MB_OK|MB_ICONINFORMATION);
		return;
	}
}

void CHerbGatherEditorDlg::InitHerbPlaceList()
{
	m_lstHerbPlace.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES );
	m_lstHerbPlace.InsertColumn(0,"序号",LVCFMT_LEFT,40);
	m_lstHerbPlace.InsertColumn(1,"坐标(x)",LVCFMT_LEFT,60);
	m_lstHerbPlace.InsertColumn(2,"坐标(y)",LVCFMT_LEFT,60);
}


// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CHerbGatherEditorDlg::OnPaint()
{
	CDialog::OnPaint();
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CHerbGatherEditorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//新建方案
void CHerbGatherEditorDlg::OnBnClickedButton2()
{
	m_nMethodCnt++;
	CString strText;
	UpdateData();

	int nItem=m_lstGatherMethod.InsertItem(m_lstGatherMethod.GetItemCount(),NULL);
	strText.Format("%d",m_nMethodCnt);
	m_lstGatherMethod.SetItemText(nItem,0,strText);
	m_lstGatherMethod.SetItemText(nItem,1,m_strMethodName);
	strText.Format("Method%d",m_nMethodCnt);
	CFile file(m_strMethodPath+strText+".dat",CFile::modeCreate|CFile::modeWrite);
	CArchive ar(&file,CArchive::store);
	ar<<m_strMethodName;
	ar<<0;
	ar.Close();
	file.Close();
}

//预览方案
void CHerbGatherEditorDlg::OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	//m_lstGatherMethod.GetCurSel()
	POSITION pos=m_lstGatherMethod.GetFirstSelectedItemPosition();
	int nItem=m_lstGatherMethod.GetNextSelectedItem(pos);
	CString strText=m_lstGatherMethod.GetItemText(nItem,0);
	CString strFile=m_strMethodPath+HERB_GATHER_METHOD_FILE+strText+".dat";
	m_strMethodName=m_lstGatherMethod.GetItemText(nItem,1);
	UpdateData(false);

	m_lstHerbPlace.DeleteAllItems();

	if (PathFileExists(strFile))
	{
		CFile file(strFile,CFile::modeRead);
		CArchive ar(&file,CArchive::load);
		int nPlaceCnt;
		int nPosx,nPosy;
		ar>>m_strMethodName;
		ar>>nPlaceCnt;
		m_lstHerbPlace.SetRedraw(FALSE);
		for (int i=0;i<nPlaceCnt;i++)
		{
			ar>>nPosx;
			ar>>nPosy;
			nItem=m_lstHerbPlace.InsertItem(m_lstHerbPlace.GetItemCount(),NULL);
			strText.Format("%d",i);
			m_lstHerbPlace.SetItemText(nItem,0,strText);
			strText.Format("%d",nPosx);
			m_lstHerbPlace.SetItemText(nItem,1,strText);
			strText.Format("%d",nPosy);
			m_lstHerbPlace.SetItemText(nItem,2,strText);
		}
		m_lstHerbPlace.SetRedraw(TRUE);
		ar.Close();
		file.Close();
	}

	*pResult = 0;
}

//插入坐标
void CHerbGatherEditorDlg::OnBnClickedButton3()
{
	CString strText;
	m_btnPosCmd.GetWindowText(strText);

	POSITION pos=m_lstHerbPlace.GetFirstSelectedItemPosition();
	int nSelItem=m_lstHerbPlace.GetNextSelectedItem(pos);

	if (strText=="插入")
	{
		UpdateData();
		int nItem=m_lstHerbPlace.InsertItem(m_lstHerbPlace.GetItemCount(),NULL);
		strText.Format("%d",nItem);
		m_lstHerbPlace.SetItemText(nItem,0,strText);
		m_lstHerbPlace.SetItemText(nItem,1,m_strPosx);
		m_lstHerbPlace.SetItemText(nItem,2,m_strPosy);
		SaveMethod();
	}
	else if (strText=="修改")
	{
		UpdateData();
		m_lstHerbPlace.SetItemText(nSelItem,1,m_strPosx);
		m_lstHerbPlace.SetItemText(nSelItem,2,m_strPosy);
		SaveMethod();
	}
	else
	{
		
	}

}

//保存方案
void CHerbGatherEditorDlg::OnBnClickedButton4()
{
	SaveMethod();
}

void CHerbGatherEditorDlg::SaveMethod()
{
	POSITION pos=m_lstGatherMethod.GetFirstSelectedItemPosition();
	int nItem=m_lstGatherMethod.GetNextSelectedItem(pos);
	CString strText=m_lstGatherMethod.GetItemText(nItem,0);
	CString strFile=m_strMethodPath+HERB_GATHER_METHOD_FILE+strText+".dat";
	UpdateData();
	m_lstGatherMethod.SetItemText(nItem,1,m_strMethodName);

	if (PathFileExists(strFile))
	{
		CFile file(strFile,CFile::modeWrite);
		CArchive ar(&file,CArchive::store);

		int nPlaceCnt=m_lstHerbPlace.GetItemCount();
		int nPosx,nPosy;
		ar<<m_strMethodName;
		ar<<nPlaceCnt;
		for (int i=0;i<nPlaceCnt;i++)
		{
			strText=m_lstHerbPlace.GetItemText(i,1);
			nPosx=atoi(strText);
			strText=m_lstHerbPlace.GetItemText(i,2);
			nPosy=atoi(strText);
			ar<<nPosx;
			ar<<nPosy;
		}
		ar.Close();
		file.Close();
	}
}

//删除方案
void CHerbGatherEditorDlg::OnDeleteMethod()
{
	POSITION pos=m_lstGatherMethod.GetFirstSelectedItemPosition();
	int nItem=m_lstGatherMethod.GetNextSelectedItem(pos);
	CString strText=m_lstGatherMethod.GetItemText(nItem,0);
	CString strFile=m_strMethodPath+HERB_GATHER_METHOD_FILE+strText+".dat";
	m_strMethodName=m_lstGatherMethod.GetItemText(nItem,1);

	if (PathFileExists(strFile))
	{
		strText.Format("确定删除方案\"%s\"吗？",m_strMethodName);
		if(MessageBox(strText,"提示",MB_OKCANCEL|MB_ICONWARNING)==1)
		{
			DeleteFile(strFile);
			m_lstGatherMethod.DeleteItem(nItem);
		}
	}
}

//预览方案
void CHerbGatherEditorDlg::OnPreviewMethod()
{
	POSITION pos=m_lstGatherMethod.GetFirstSelectedItemPosition();
	int nItem=m_lstGatherMethod.GetNextSelectedItem(pos);
	if (nItem>=0)
	{
		CString strText=m_lstGatherMethod.GetItemText(nItem,0);
		CString strFile=m_strMethodPath+HERB_GATHER_METHOD_FILE+strText+".dat";

		if (PathFileExists(strFile))
		{
			CPreviewDlg dlg;
			dlg.m_strMethodFile=strFile;
			dlg.DoModal();
		}
	}
}	

void CHerbGatherEditorDlg::OnNMRclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	COfficeXPMenu m_popmenu;
	m_popmenu.LoadMenu(IDR_MENU_EDIT_HERB_PLACE);	
	m_popmenu.SetType(TYPE_XP);
	CMenu *psub = (CMenu *)m_popmenu.GetSubMenu(1);
	CPoint pt;
	GetCursorPos(&pt);
	DWORD dwID =psub->TrackPopupMenu((TPM_LEFTALIGN|TPM_RIGHTBUTTON),
		pt.x,pt.y, this);
	m_popmenu.DestroyMenu();

	*pResult = 0;
}


void CHerbGatherEditorDlg::OnNMRclickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	COfficeXPMenu m_popmenu;
	m_popmenu.LoadMenu(IDR_MENU_EDIT_HERB_PLACE);	
	m_popmenu.SetType(TYPE_XP);
	CMenu *psub = (CMenu *)m_popmenu.GetSubMenu(0);
	CPoint pt;
	GetCursorPos(&pt);
	DWORD dwID =psub->TrackPopupMenu((TPM_LEFTALIGN|TPM_RIGHTBUTTON),
		pt.x,pt.y, this);
	m_popmenu.DestroyMenu();

	*pResult = 0;
}

void CHerbGatherEditorDlg::OnAddPosition()
{
	m_btnPosCmd.SetWindowText("插入");
}

void CHerbGatherEditorDlg::OnAlterPosition()
{
	m_btnPosCmd.SetWindowText("修改");
}

void CHerbGatherEditorDlg::OnDeletePosition()
{
	int nItems[1024];
	int nCount=0;

	m_lstHerbPlace.SetRedraw(FALSE);
	POSITION pos=m_lstHerbPlace.GetFirstSelectedItemPosition();
	while (pos)
	{
		nItems[nCount++]=m_lstHerbPlace.GetNextSelectedItem(pos);
		//m_lstHerbPlace.DeleteItem(nSelItem);
	}
	while(nCount>=0)
	{
		m_lstHerbPlace.DeleteItem(nItems[nCount--]);
	}
	m_lstHerbPlace.SetRedraw(TRUE);
	SaveMethod();
}






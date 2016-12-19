// SignSearchDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SignSearch.h"
#include "SignSearchDlg.h"
#include "UpdateSignDlg.h"
#include "AddSignDlg.h"
#include "SignMatch.h"
#include <TlHelp32.h>
#include <Shlwapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSignSearchDlg �Ի���




CSignSearchDlg::CSignSearchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSignSearchDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bScanning=FALSE;
	m_strStartPath=GetStartPath();
	m_strSignFile=m_strStartPath+"sign.txt";
}

void CSignSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SIGN, m_lstSign);
	DDX_Control(pDX, IDC_EDIT_MSG, m_edtMsg);
}

BEGIN_MESSAGE_MAP(CSignSearchDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_RCLICK, IDC_LIST_SIGN, &CSignSearchDlg::OnNMRclickListSign)
	ON_MESSAGE(WM_SCANSIGNBEGIN,OnScanSignBegin)
	ON_MESSAGE(WM_SCANSIGNEND,OnScanSignEnd)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SIGN, &CSignSearchDlg::OnNMDblclkListSign)
END_MESSAGE_MAP()


// CSignSearchDlg ��Ϣ�������

BOOL CSignSearchDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//////////////////////////////////////////////////////////////////////////
	InitAllControls();

	LoadSigns();
	//////////////////////////////////////////////////////////////////////////

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSignSearchDlg::OnPaint()
{
	CDialog::OnPaint();
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CSignSearchDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CSignSearchDlg::PreTranslateMessage(MSG *pMsg)
{
	if( pMsg->message==WM_KEYDOWN && (pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE ) ){
		return TRUE;
	}else{
		return CDialog::PreTranslateMessage(pMsg);
	}
}

void CSignSearchDlg::OnOK()
{
	SaveSigns();
	__super::OnOK();
}

void CSignSearchDlg::OnCancel()
{
	SaveSigns();
	__super::OnCancel();
}

void CSignSearchDlg::InitAllControls()
{
	m_lstSign.SetExtendedStyle( m_lstSign.GetExtendedStyle() | 
		LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT & (~(LVS_EX_GRIDLINES)) );
	m_lstSign.ModifyStyle( LVS_LIST | LVS_SMALLICON | LVS_ICON | LVS_NOCOLUMNHEADER, LVS_REPORT);

	CRect rcList;
	m_lstSign.GetClientRect(&rcList);
	m_lstSign.InsertColumn( 0, _T("ɨ�����ַ"), LVCFMT_LEFT, 80 );
	m_lstSign.InsertColumn( 1, _T("��ʼRVA"), LVCFMT_LEFT, 80 );
	m_lstSign.InsertColumn( 2, _T("����RVA"), LVCFMT_LEFT, 80 );
	m_lstSign.InsertColumn( 3, _T("������"), LVCFMT_LEFT, rcList.Width()-80-80-80-4 );

	m_lstSign.InsertItem(NULL);
}

//���ı��ļ��м����ϴα��ֵ�����
void CSignSearchDlg::LoadSigns()
{
	CString strLine;
	CString strSigns;
	CStdioFile file;
	if ( file.Open(m_strSignFile,CFile::modeRead) ){
		while ( file.ReadString(strLine) ){
			strSigns+=strLine+"\n";
		}
		file.Close();
	}
	

	AddSigns(strSigns);
}

void CSignSearchDlg::SaveSigns()
{
	int nItemCnt=m_lstSign.GetItemCount();
	if ( nItemCnt<=0 ){
		return;
	}

	CString strText;
	CString strLine;
	CFile file;
	if ( file.Open(m_strSignFile,CFile::modeCreate|CFile::modeReadWrite) ){

		for ( int i=0; i<nItemCnt; ++i ){
			strLine.Format("%s %s %s %s\r\n",
				m_lstSign.GetItemText(i,0), m_lstSign.GetItemText(i,1),
				m_lstSign.GetItemText(i,2), m_lstSign.GetItemText(i,3) );
			strText+=strLine;
		}

		file.Write( (LPCTSTR)strText, strText.GetLength() );
		file.Close();
	}

	
}

CString CSignSearchDlg::GetStartPath()
{
	TCHAR szTemp[MAX_PATH*2];
	GetModuleFileName(NULL,szTemp,sizeof(szTemp)/sizeof(TCHAR));
	StrRChr(szTemp,NULL,'\\')[1]=0;
	return szTemp;
}

void CSignSearchDlg::OnNMRclickListSign(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 0;

	int nItemCnt=m_lstSign.GetItemCount();
	int nSelectItem=m_lstSign.GetSelectionMark();
	int nSelectCnt=0;
	for ( int i=0; i<nItemCnt; ++i ){
		if ( m_lstSign.GetCheck(i) ){
			++nSelectCnt;
		}
	}
	

	POSITION pos=m_lstSign.GetFirstSelectedItemPosition();
	int nItem=0;
	BOOL bChecked=FALSE;

	CMenu m_popmenu;
	m_popmenu.LoadMenu(IDR_MENU_POP);	
	CMenu *psub = (CMenu*)m_popmenu.GetSubMenu(0);
	CPoint pt;
	GetCursorPos(&pt);
	psub->EnableMenuItem(ID_MENU_SELECTALL,nItemCnt<=0);
	psub->EnableMenuItem(ID_MENU_UPDATESIGN,nSelectItem<0);
	psub->EnableMenuItem(ID_MENU_DELETESIGN,nSelectCnt<=0);
	psub->EnableMenuItem(ID_MENU_SCANSIGN,m_bScanning|nSelectCnt<=0);
	DWORD dwID =psub->TrackPopupMenu((TPM_LEFTALIGN|TPM_RIGHTBUTTON|TPM_NONOTIFY|TPM_RETURNCMD),
		pt.x,pt.y, this);

	if ( dwID==ID_MENU_SELECTALL ){
		for ( int nItem=0; nItem<m_lstSign.GetItemCount(); ++nItem ){
			m_lstSign.SetCheck(nItem,TRUE);
		}
	}else if ( dwID==ID_MENU_ADDSIGN ){
		CAddSignDlg dlg;
		if ( dlg.DoModal()==IDOK ){
			AddSigns(dlg.m_strSigns);
		}
	}else if ( dwID==ID_MENU_UPDATESIGN ){
		OnUpdateSign(nSelectItem);
	}else if ( dwID==ID_MENU_DELETESIGN ){

		if ( AfxMessageBox(_T("ȷ��ɾ����"),MB_YESNO)==IDYES ){
			for ( int i=nItemCnt-1; i>=0; --i ){
				if ( m_lstSign.GetCheck(i) ){
					m_lstSign.DeleteItem(i);
				}
			}
		}
	}else if ( dwID==ID_MENU_SCANSIGN ){
		AfxBeginThread(ThreadScanSign,this);
	}

	m_popmenu.DestroyMenu();
}


void CSignSearchDlg::OnNMDblclkListSign(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	LPNMLISTVIEW pNMLV = reinterpret_cast <LPNMLISTVIEW>(pNMHDR); 
	OnUpdateSign(pNMLV->iItem);
}

void CSignSearchDlg::OnUpdateSign(int nItem)
{
	if ( nItem<0 ){
		return;
	}

	CUpdateSignDlg dlg;
	dlg.m_strSearchBase=m_lstSign.GetItemText(nItem,0);
	dlg.m_strStartRva=m_lstSign.GetItemText(nItem,1);
	dlg.m_strEndRva=m_lstSign.GetItemText(nItem,2);
	dlg.m_strSign=m_lstSign.GetItemText(nItem,3);
	if ( dlg.DoModal()==IDOK ){
		m_lstSign.SetItemText(nItem,0,dlg.m_strSearchBase);
		m_lstSign.SetItemText(nItem,1,dlg.m_strStartRva);
		m_lstSign.SetItemText(nItem,2,dlg.m_strEndRva);
		m_lstSign.SetItemText(nItem,3,dlg.m_strSign);
	}
}

int SplitString(CString strSrc,CString strSep,CStringArray&strArr)
{
	int nCnt=0;
	strArr.RemoveAll();
	char *pToken=strtok( (char*)(LPCTSTR)strSrc, (char*)(LPCTSTR)strSep ); 
	while( pToken!=NULL ){ 
		strArr.Add(pToken);
		++nCnt; 
		pToken=strtok( NULL,  (char*)(LPCTSTR)strSep );
	}

	return nCnt;
}

int CSignSearchDlg::AddSigns(CString&strSigns)
{
	int nSignCnt=0;
	int nItem=0;
	CString strLine;
	CString strText;
	int nSplitPos1=-1;
	int nSplitPos2=-1;
	int nPos1=0;
	int nPos2=0;

	strSigns.Trim();
	if ( strSigns.IsEmpty() ){
		return nSignCnt;
	}
	int nLen=strSigns.GetLength();
	
	do{
		nSplitPos2=strSigns.Find('\n',nSplitPos1+1);
		if ( nSplitPos2==-1 ){
			nSplitPos2=nLen;
		}

		strLine=strSigns.Mid(nSplitPos1+1,nSplitPos2-nSplitPos1-1);
		strLine.Trim();
		CStringArray strArr;
		if ( SplitString(strLine," ",strArr)!=4 ){
			AfxMessageBox(_T("��������������������룡"));
			break;
		}

		nItem=m_lstSign.GetItemCount();
		m_lstSign.InsertItem(nItem,strArr[0]);
		m_lstSign.SetItemText(nItem,1,strArr[1]);
		m_lstSign.SetItemText(nItem,2,strArr[2]);
		m_lstSign.SetItemText(nItem,3,strArr[3]);

		nSplitPos1=nSplitPos2;
		
	}while ( nSplitPos2!=nLen );

	return nSignCnt;
}

void CSignSearchDlg::AppendText(const CString&strText)
{
	int nLine=m_edtMsg.GetLineCount();
	if ( nLine>1000 ){
		m_edtMsg.Clear();
	}
	LONG nLen=(LONG)m_edtMsg.SendMessage(WM_GETTEXTLENGTH);
	m_edtMsg.SetSel(nLen,-1);
	m_edtMsg.ReplaceSel(strText+"\r\n");
}


LRESULT CSignSearchDlg::OnScanSignBegin(WPARAM wParam,LPARAM lParam)
{
	AppendText("������������ʼ����");
	m_bScanning=TRUE;
	return 0;
}

LRESULT CSignSearchDlg::OnScanSignEnd(WPARAM wParam,LPARAM lParam)
{
	m_bScanning=FALSE;
	AppendText("������������������");
	return 0;
}

UINT CSignSearchDlg::ThreadScanSign(LPVOID lParam)
{
	CString strText;

	if ( lParam==NULL ){
		return 0;
	}

	//ɨ�迪ʼ
	CSignSearchDlg*pDlg=(CSignSearchDlg*)lParam;
	pDlg->SendMessage(WM_SCANSIGNBEGIN);
	//////////////////////////////////////////////////////////////////////////

	//��ȡѡ������������
	CListCtrl&listSigns=pDlg->m_lstSign;
	int nSelectCnt=0;
	int nIndex=0;
	for ( int i=0; i<listSigns.GetItemCount(); ++i ){
		if ( listSigns.GetCheck(i) ){
			++nSelectCnt;
		}
	}

	//�����ڲ������벢��ʼ��
	CSignMatch*pSignArr=new CSignMatch[nSelectCnt];
	for ( int i=0,nIndex=0; i<listSigns.GetItemCount(); ++i,++nIndex ){
		if ( listSigns.GetCheck(i) ){
			strText=listSigns.GetItemText(i,0);
			DWORD dwBase=strtol(strText,NULL,16);
			strText=listSigns.GetItemText(i,1);
			DWORD dwRvaBegin=strtol(strText,NULL,16);
			strText=listSigns.GetItemText(i,2);
			DWORD dwRvaEnd=strtol(strText,NULL,16);
			strText=listSigns.GetItemText(i,3);
			pSignArr[nIndex].SetOrigSign( PBYTE(NULL)+dwBase,dwRvaBegin,dwRvaEnd,(char*)(LPCTSTR)strText );
		}
	}
	
	//ö�ٽ��̲�ɨ���ڴ�
	//////////////////////////////////////////////////////////////////////////

	BOOL	bFound=FALSE; 
	PROCESSENTRY32 stProcess;
	stProcess.dwSize=sizeof(stProcess);

	HANDLE hSnapShot=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if ( hSnapShot!=NULL && Process32First(hSnapShot,&stProcess) ){
		do 
		{
			HANDLE hProcess=OpenProcess(PROCESS_ALL_ACCESS,FALSE,stProcess.th32ProcessID);
			if ( hProcess==NULL ){
				continue;
			}

			for ( int i=0; i<nSelectCnt; ++i ){
				PBYTE pMatchPos=pSignArr[i].SearchProcessSign(hProcess);
				if ( pMatchPos!=NULL ){
					strText.Format("�ڡ�%d��������ƥ�䣬λ�ã�%08X , RVA��%08X , ���̣�%s",
						i+1,(DWORD)pMatchPos,(DWORD)pSignArr[i].m_dwSearchBegin,stProcess.szExeFile);
					pDlg->AppendText(strText);
				}else{
					break;
				}
			}

		} while( Process32Next(hSnapShot,&stProcess) );
		CloseHandle(hSnapShot); 
	}

	//�ͷ���Դ
	delete[]pSignArr;

	//ɨ�����
	//////////////////////////////////////////////////////////////////////////
	pDlg->SendMessage(WM_SCANSIGNEND);
	return 0;
}

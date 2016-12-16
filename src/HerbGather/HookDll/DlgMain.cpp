// DlgResult.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Hook.h"
#include "DlgMain.h"

static POINT g_ptHerbGrowth[1024];
int g_nNextPlace=0;
int g_nPlaceCount=1;

//ͳ��һ����ͼ�ϵĲ�ҩ����
static POINT g_ptKeyPoint[65536];

// CDlgMain �Ի���

IMPLEMENT_DYNAMIC(CDlgMain, CDialog)

CDlgMain::CDlgMain(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMain::IDD, pParent)
	, m_bCheckSend(FALSE)
	, m_bCheckRecv(FALSE)
	, m_strGame(_T("game"))
	, m_strMessage(_T("LiveSkillFire(\"��ͷ\",\"����6\",262218)"))
	, m_nDstPosX(10)
	, m_nDstPosY(10)
	, m_nGatherLevel(1)
	, m_strRoleName(_T(""))
{

}

CDlgMain::~CDlgMain()
{
}

void CDlgMain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MSG, m_edtMsg);
	DDX_Check(pDX, IDC_CHECK_DATA_SEND, m_bCheckSend);
	DDX_Check(pDX, IDC_CHECK_DATA_RECV, m_bCheckRecv);
	DDX_Text(pDX, IDC_EDIT_GAME, m_strGame);
	DDX_Text(pDX, IDC_EDIT_MESSAGE, m_strMessage);
	DDX_Control(pDX, IDC_COMBO_ORDER, m_cmbOrder);
	DDX_Text(pDX, IDC_EDIT_POSX, m_nDstPosX);
	DDV_MinMaxInt(pDX, m_nDstPosX, 1, 1000);
	DDX_Text(pDX, IDC_EDIT_POSY, m_nDstPosY);
	DDV_MinMaxInt(pDX, m_nDstPosY, 1, 1000);
	DDX_Text(pDX, IDC_EDIT_GATHER_LEVEL, m_nGatherLevel);
	DDV_MinMaxInt(pDX, m_nGatherLevel, 1, 1000);
	DDX_Text(pDX, IDC_EDIT_ROLE_NAME, m_strRoleName);
	DDX_Control(pDX, IDC_COMBO_HERBS, m_cmbHerbs);
	DDX_Control(pDX, IDC_BUTTON_GATHER_HERB, m_btnGatherHerb);
}


BEGIN_MESSAGE_MAP(CDlgMain, CDialog)
	ON_BN_CLICKED(IDC_CHECK_DATA_SEND, &CDlgMain::OnBnClickedCheckDataSend)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CDlgMain::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_WALK, &CDlgMain::OnBnClickedButtonWalk)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT_MSG, &CDlgMain::OnEnChangeEditMsg)
	ON_BN_CLICKED(IDC_BUTTON_GATHER_HERB, &CDlgMain::OnBnClickedButtonGatherHerb)
	ON_CBN_SELCHANGE(IDC_COMBO_HERBS, &CDlgMain::OnCbnSelchangeComboHerbs)
	ON_CBN_DROPDOWN(IDC_COMBO_HERBS, &CDlgMain::OnCbnDropdownComboHerbs)
	ON_BN_CLICKED(IDC_BUTTON_COUNT_HERB, &CDlgMain::OnBnClickedButtonCountHerb)
	ON_BN_CLICKED(IDC_BUTTON3, &CDlgMain::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgMain::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDlgMain ��Ϣ�������

BOOL CDlgMain::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cmbHerbs.SetCurSel(-1);

	char szBuff[MAX_PATH];
	m_strCfgFile=theApp.m_strDllPath+"user.dat";
	GetPrivateProfileString("main","RoleName","",szBuff,sizeof(szBuff),m_strCfgFile);
	m_strRoleName=szBuff;
	m_nGatherLevel=GetPrivateProfileInt("main","HuntSkillLevel",1,m_strCfgFile);
	UpdateData(FALSE);

	m_cmbOrder.InsertString(-1,"76");
	m_cmbOrder.InsertString(-1,"77");
	m_cmbOrder.InsertString(-1,"82");
	m_cmbOrder.InsertString(-1,"90");
	m_cmbOrder.SetCurSel(-1);

	return TRUE;
}

void CDlgMain::OnOK() 
{ 
	if (!UpdateData(TRUE)) 
	{
		TRACE0("UpdateData failed during dialog termination\n");
		// The UpdateData routine will set focus to correct item
		return;
	}
	DestroyWindow();
}

void CDlgMain::OnCancel()
{
	DestroyWindow();
}

BOOL CDlgMain::DestroyWindow()
{
	ClearHerbArr();

	UpdateData(TRUE);
	WritePrivateProfileString("main","RoleName",m_strRoleName,m_strCfgFile);
	CString str;
	str.Format("%d",m_nGatherLevel);
	WritePrivateProfileString("main","HuntSkillLevel",str,m_strCfgFile);

	return CDialog::DestroyWindow();
}

void CDlgMain::ClearHerbArr()
{
	for (int i=0;i<m_arrHerb.GetCount();i++)
	{
		PHERB pHerb=(PHERB)m_arrHerb[i];
		if(pHerb)
			delete pHerb;
	}
	m_arrHerb.RemoveAll();
	m_arrHerb.FreeExtra();
}

void CDlgMain::AppendText(CString strText)
{
	LONG nLen=(LONG)m_edtMsg.SendMessage(WM_GETTEXTLENGTH);
	m_edtMsg.SetSel(nLen,-1);
	m_edtMsg.ReplaceSel(strText+"\r\n");
}
void CDlgMain::OnBnClickedCheckDataSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (m_bCheckSend)
	{
		theApp.Hook.Change();
	}
	else
	{
		theApp.Hook.Restore();
	}
}

void CDlgMain::OnBnClickedButtonSend()
{
	UpdateData();
	CString  strText;
	m_cmbOrder.GetWindowText(strText);
	int nOrder=atoi(strText);
	TSendOrder(ADDR_SEND)((char*)(LPCTSTR)m_strGame,nOrder,(char*)(LPCTSTR)m_strMessage);
}

void CDlgMain::WalkTo(int x,int y)
{
	DWORD dwThis=0;
	TWalkTo pFunc;

	__asm
	{
		pushad
		mov	 ecx,ADDR_MAPOBJ_THIS
		mov	 ecx,[ecx]
		mov  dwThis,ecx
	}
	if(dwThis==0)
	{
		MessageBox("thisָ��δ����ʼ�������С��ͼ�ƶ�һ�Σ�",NULL,MB_OK);
	}
	else
	{
		__asm
		{
			push -1
			push -1
			push y
			push x
			mov  pFunc,ADDR_WALKTO
			call pFunc
		}
		m_nDstPosX=x;
		m_nDstPosY=y;
		UpdateData(FALSE);

		CString strText;
		strText.Format("Ŀ��λ�ã�{%d:%d}",x,y);
		AppendText(strText);

		KillTimer(TIMER_GET_POS);
		SetTimer(TIMER_GET_POS,1000,NULL);
	}

	__asm
	{
		popad
	}
}

void CDlgMain::OnTimer(UINT_PTR nIDEvent)
{
	CDialog::OnTimer(nIDEvent);

	switch (nIDEvent)
	{
	case TIMER_GET_POS:
		{
			int nPosx=*((int*)(*(PDWORD32(ADDR_MAINOBJ_THIS))+0x14));
			int nPosy=*((int*)(*(PDWORD32(ADDR_MAINOBJ_THIS))+0x18));
			if (nPosx==m_nCurPosX&&nPosy==m_nCurPosY)
			{
				//˵���Ѿ�ֹͣ�ƶ���
				int nDeltaX=abs(m_nCurPosX-m_nDstPosX);
				int nDeltaY=abs(m_nCurPosY-m_nDstPosY);
				if (nDeltaX<2&&nDeltaY<2)
				{
					//������Ϊ�Ѿ�����Ŀ�ĵ���
					KillTimer(TIMER_GET_POS);
					m_nCurPosX=m_nDstPosX;
					m_nCurPosY=m_nDstPosY;
					Arrived();
				}
				else
				{
					//��;ֹͣ�ˣ���û�е���Ŀ�ĵ�,�����ƶ�
					WalkTo(m_nDstPosX,m_nDstPosY);
				}
			}
			else
			{
				//˵�������ƶ�
				m_nCurPosX=nPosx;
				m_nCurPosY=nPosy;
			}
		}
		
		break;

	case TIMER_CHECK_HERB_GATHERED:
		{
			PVOID*arrHerb;
			int nCount=0;

			arrHerb=*((PVOID**)ADDR_HERB_ARR);
			PVOID pHerb=arrHerb[m_uGatheringHerbID];
			if (pHerb==NULL)
			{
				//��ҩ������˵���Ѿ����ɼ��ˣ���ɼ���һ��
				KillTimer(TIMER_CHECK_HERB_GATHERED);
				GatherHerb(0);
			}
		}
		break;

	case TIMER_COUNT_HERB:
		{
			const int HERB_COUNT=65535;
			PVOID*arrHerb=*((PVOID**)ADDR_HERB_ARR);
			int nCount=0;

			for (int i=0;i<=HERB_COUNT;i++)
			{
				PVOID pHerb=arrHerb[i];
				if (pHerb)
				{
					g_ptKeyPoint[i].x=*(PINT32(*(PDWORD32(DWORD(pHerb)+0x1C))+8));
					g_ptKeyPoint[i].y=*(PINT32(*(PDWORD32(DWORD(pHerb)+0x1C))+0x0C));			
				}
			}
		}
		break;
	}
}

void CDlgMain::Arrived()
{
	//AppendText("�Ѿ�����Ŀ�ĵأ���ʼִ����һ������");
	GatherHerb(0);
}

void CDlgMain::GatherHerb(UINT ID)
{
	if (ID)
	{
		CString strMessage;
		strMessage.Format("LiveSkillFire(\"%s\",\"����%d\",%u)",m_strRoleName,m_nGatherLevel,ID);// LiveSkillFire("��ͷ","����6",262218)
		TSendOrder(ADDR_SEND)("game",90,(char*)(LPCTSTR)strMessage);
		m_uGatheringHerbID=ID;
		KillTimer(TIMER_CHECK_HERB_GATHERED);
		SetTimer(TIMER_CHECK_HERB_GATHERED,2000,NULL);
	}
	else
	{
		SearchHerb();
		PHERB pHerb=GetNearestHerb(4);
		if (pHerb)
		{
			GatherHerb(pHerb->ID);	
		}
		else
		{
			//����û�в�ҩ���ƶ�����һλ��
			g_nNextPlace=g_nNextPlace%g_nPlaceCount;
			WalkTo(g_ptHerbGrowth[g_nNextPlace].x,g_ptHerbGrowth[g_nNextPlace].y);
			g_nNextPlace++;
		}
	}

}

int CDlgMain::SearchHerb()
{
	const int HERB_COUNT=65535;
	PVOID*arrHerb=*((PVOID**)ADDR_HERB_ARR);
	int nCount=0;

	ClearHerbArr();

	for (int i=0;i<=HERB_COUNT;i++)
	{
		PVOID pHerb=arrHerb[i];
		if (pHerb)
		{
			PHERB pItem=new HERB;
			pItem->ID=*(PUINT(DWORD(pHerb)+0x10));
			pItem->nPosx=*(PINT32(*(PDWORD32(DWORD(pHerb)+0x1C))+8));
			pItem->nPosy=*(PINT32(*(PDWORD32(DWORD(pHerb)+0x1C))+0x0C));
			pItem->nOffset=i;
			m_arrHerb.Add(pItem);
			nCount++;			
		}
	}
	
	CString strText;
	strText.Format("���ҵ� %d �ò�ҩ",nCount);
	AppendText(strText);

	return nCount;
}

PHERB CDlgMain::GetNearestHerb(int nMinLen)
{
	PHERB pResult=NULL;
	int nLen;

	for (int i=0;i<m_arrHerb.GetCount();i++)
	{
		PHERB pHerb=(PHERB)m_arrHerb[i];
		nLen=(pHerb->nPosx-m_nCurPosX)*(pHerb->nPosx-m_nCurPosX)
			+(pHerb->nPosy-m_nCurPosY)*(pHerb->nPosy-m_nCurPosY);
		if(nLen<0)
			return pResult;

		if (nLen==0)
		{
			//�����Ѿ��ܽ���
			return pHerb;
		}

		if (nLen<nMinLen)
		{
			pResult=pHerb;
			nMinLen=nLen;
		}
	}	

	CString strText;
	strText.Format("����Ĳ�ҩ���룺%d",nMinLen);
	AppendText(strText);

	return pResult;
}

void CDlgMain::OnBnClickedButtonWalk()
{
	UpdateData();
	WalkTo(m_nDstPosX,m_nDstPosY);
}

void CDlgMain::OnEnChangeEditMsg()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if(m_edtMsg.GetLineCount()>1000)
		m_edtMsg.SetWindowText(NULL);
}

void CDlgMain::OnBnClickedButtonGatherHerb()
{
	CString strText;
	m_btnGatherHerb.GetWindowText(strText);
	if (strText=="��ҩ")
	{
		m_btnGatherHerb.SetWindowText("ֹͣ");

		UpdateData();
		int nIndex=m_cmbHerbs.GetCurSel();
		if(nIndex<0)
			return;
		int nFile=m_cmbHerbs.GetItemData(nIndex);
		if(nFile<=0)
			return;

		CString strFile;
		strFile.Format("%sHerb\\Method%d.dat",theApp.m_strDllPath,nFile);
		//MessageBox(strFile);
		if (PathFileExists(strFile))
		{
			CFile file(strFile,CFile::modeRead);
			CArchive ar(&file,CArchive::load);
			CString strMethodName;

			if (ar.IsLoading())
				ar>>strMethodName;

			ar>>g_nPlaceCount;
			g_nPlaceCount=min(g_nPlaceCount,sizeof(g_ptHerbGrowth)/sizeof(POINT));
			for (int i=0;i<g_nPlaceCount;i++)
			{
				ar>>g_ptHerbGrowth[i].x;
				ar>>g_ptHerbGrowth[i].y;
				strText.Format("%d %d",g_ptHerbGrowth[i].x,g_ptHerbGrowth[i].y);
				AppendText(strText);
			}
			ar.Close();
			file.Close();
			
			g_nNextPlace=0;
			GatherHerb(0);
		}
	}
	else
	{
		m_btnGatherHerb.SetWindowText("��ҩ");
		KillTimer(TIMER_CHECK_HERB_GATHERED);
		KillTimer(TIMER_GET_POS);
	}

}

void CDlgMain::OnCbnSelchangeComboHerbs()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CDlgMain::OnCbnDropdownComboHerbs()
{
	while(m_cmbHerbs.DeleteString(0)>=0);

	int nMethodCnt=0;
	while(TRUE)
	{
		CString strFile;
		strFile.Format("%sHerb\\Method%d.dat",theApp.m_strDllPath,nMethodCnt+1);
		if (PathFileExists(strFile))
		{
			CFile file(strFile,CFile::modeRead);
			CArchive ar(&file,CArchive::load);
			CString strMethodName;

			if (ar.IsLoading())
				ar>>strMethodName;
			int nItem=m_cmbHerbs.InsertString(m_cmbHerbs.GetCount(),strMethodName);
			m_cmbHerbs.SetItemData(nItem,nMethodCnt+1);

			ar.Close();
			file.Close();

			nMethodCnt++;
		}
		else
		{
			break;
		}
	}
	if (nMethodCnt==0)
	{
		MessageBox("��δ������ҩ���÷�����","��ʾ",MB_OK|MB_ICONINFORMATION);
		return;
	}
}

void CDlgMain::OnBnClickedButtonCountHerb()
{
	KillTimer(TIMER_COUNT_HERB);
	SetTimer(TIMER_COUNT_HERB,1000,NULL);
}

void CDlgMain::OnBnClickedButton3()
{
	KillTimer(TIMER_COUNT_HERB);
	m_edtMsg.SetWindowText(NULL);

	for (int i=0;i<sizeof(g_ptKeyPoint)/sizeof(POINT);i++)
	{
		CString strText;
		if (g_ptKeyPoint[i].x!=0 && g_ptKeyPoint[i].y!=0)
		{
			strText.Format("%d %d",g_ptKeyPoint[i].x,g_ptKeyPoint[i].y);
			AppendText(strText);
		}
	}
}

void CDlgMain::OnBnClickedButton1()
{
	ShellExecute(NULL,"open",theApp.m_strDllPath+"HerbGatherEditor.exe",NULL,NULL,SW_SHOWNORMAL); 
}

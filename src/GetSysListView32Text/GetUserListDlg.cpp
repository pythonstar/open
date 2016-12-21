// GetUserListDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GetUserList.h"
#include "GetUserListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGetUserListDlg �Ի���




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


// CGetUserListDlg ��Ϣ�������

BOOL CGetUserListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_lstCopyed.SetExtendedStyle(LVS_EX_FULLROWSELECT); 

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CGetUserListDlg::OnPaint()
{
	CDialog::OnPaint();
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
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
	BOOL bUnicode = FALSE;

	while(m_lstCopyed.DeleteColumn(0));
	m_lstCopyed.DeleteAllItems();

	if (m_btnShoot.m_hHandle)
	{
		lstCtrl.m_hWnd=m_btnShoot.m_hHandle;
		bUnicode = ::IsWindowUnicode(lstCtrl.m_hWnd);
		GetWindowThreadProcessId(m_btnShoot.m_hHandle, &PID); 
		hProcess=OpenProcess(PROCESS_ALL_ACCESS,false,PID);
		if (!hProcess)
			MessageBox(_T("��ȡ���̾������ʧ�ܣ�"), _T("����"), NULL);
		else 
		{ 
			pItemOrColumn = (TCHAR*)VirtualAllocEx(hProcess, NULL, max(sizeof(LVITEM), sizeof(LV_COLUMN)) * 100, MEM_COMMIT, PAGE_READWRITE); 
			pBuffer = (TCHAR*)VirtualAllocEx(hProcess, NULL, 512, MEM_COMMIT, PAGE_READWRITE); 
			if ( pItemOrColumn == NULL || pBuffer == NULL) 
				MessageBox(_T("�޷������ڴ棡"), _T("����"), NULL); 
			else 
			{ 
				//��ȡ����
				pHeader=lstCtrl.GetHeaderCtrl();
				int nColumnCnt=pHeader->GetItemCount();

				//��ȡ����Ϣ
				lvColumn.mask=LVCF_TEXT;
				lvColumn.cchTextMax=512;
				lvColumn.pszText=pBuffer;
				lvColumn.iSubItem=0;
				bRet = WriteProcessMemory(hProcess, pItemOrColumn, &lvColumn, sizeof(LV_COLUMN), NULL);
				if ( bRet==FALSE ) {
					MessageBox(_T("WriteProcessMemory failed!")); 
				}
				for(int i=0; i<nColumnCnt; i++) {
					if ( bUnicode==TRUE ) {
						bRet = ::SendMessageW(lstCtrl.m_hWnd, LVM_GETCOLUMN, (WPARAM)i, (LPARAM)pItemOrColumn);
					}else{
						bRet = ::SendMessageA(lstCtrl.m_hWnd, LVM_GETCOLUMN, (WPARAM)i, (LPARAM)pItemOrColumn);
					}
					bRet = ReadProcessMemory(hProcess, pBuffer, ItemBuf, 512, NULL);
					strText = ItemBuf;
					m_lstCopyed.InsertColumn(i, strText);
					m_lstCopyed.SetColumnWidth(i, 80);
				}

				//��ȡ����Ϣ
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
						if ( bUnicode==TRUE ) {
							bRet = ::SendMessageW(lstCtrl.m_hWnd, LVM_GETITEMTEXT, (WPARAM)i, (LPARAM)pItemOrColumn);
						}else{
							bRet = ::SendMessageA(lstCtrl.m_hWnd, LVM_GETITEMTEXT, (WPARAM)i, (LPARAM)pItemOrColumn);
						}
						bRet = ReadProcessMemory(hProcess,pBuffer,ItemBuf,512,NULL);
						strText = ItemBuf;
						m_lstCopyed.SetItemText(i,nSubItem,strText);
					}
				}
				//�ͷ��ڴ� 
				CloseHandle(hProcess); 
				VirtualFreeEx(hProcess, pItemOrColumn, 0, MEM_RELEASE); 
				VirtualFreeEx(hProcess, pBuffer, 0, MEM_RELEASE); 
				lstCtrl.m_hWnd=NULL;
			} 
		} 
	}
}

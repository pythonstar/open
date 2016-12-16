
/************************************************************************
�ļ���PreviewDlg.cpp
���ߣ�������
���ڣ�2009-6-6
˵��������Ԥ������ʵ���ļ�����Ҫ����ʾ���������в�ҩ�ڵ�ͼ�еĴ��·ֲ������
Ԥ�����������󻯣��������ܲ鿴��ȫ��
************************************************************************/

#include "stdafx.h"
#include "HerbGatherEditor.h"
#include "PreviewDlg.h"


// CPreviewDlg �Ի���

IMPLEMENT_DYNAMIC(CPreviewDlg, CDialog)

CPreviewDlg::CPreviewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPreviewDlg::IDD, pParent)
{
	m_bInited=FALSE;
	m_nPlaceCnt=0;
}

CPreviewDlg::~CPreviewDlg()
{
}

void CPreviewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPreviewDlg, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CPreviewDlg ��Ϣ�������

/************************************************************************
������
������
���أ�
���ߣ�������
���ڣ�2009-6-6
˵������ȡm_strMethodFile�����ļ�������������Ϣ
************************************************************************/
BOOL CPreviewDlg::OnInitDialog()
{
	CString strMethodName;
	CFile	file(m_strMethodFile,CFile::modeRead);
	CArchive ar(&file,CArchive::load);

	ar>>strMethodName;
	ar>>m_nPlaceCnt;
	for (int i=0;i<m_nPlaceCnt;i++)
	{
		ar>>m_ptArr[i].x;
		ar>>m_ptArr[i].y;
		//�Ŵ�һ�������򻭳��ĵ�̫����
		m_ptArr[i].x*=2;	
		m_ptArr[i].y*=2;
	}
	ar.Close();
	file.Close();
	m_bInited=TRUE;
	this->SetWindowText(strMethodName);

	//�����ʾ���ڣ��������ܲ鿴ȫ����������Ϣ
	ShowWindow(SW_SHOWMAXIMIZED);
	return CDialog::OnInitDialog();
}

void CPreviewDlg::OnPaint()
{
	CDialog::OnPaint();
	if (!m_bInited || m_nPlaceCnt==0)
		return;

	CClientDC dc(this);
	CBrush brush(RGB(0,128,0));		//����ɫ
	dc.SelectObject(brush);
	dc.SetBkMode(TRANSPARENT);

	dc.MoveTo(m_ptArr[0]);

	for (int i=0;i<m_nPlaceCnt;i++)
	{
		CString strText;
		CRect rect(m_ptArr[i].x-4,m_ptArr[i].y-4,m_ptArr[i].x+4,m_ptArr[i].y+4);
		dc.LineTo(m_ptArr[i]);
		dc.Ellipse(rect);			//ԭ���ʾ
		strText.Format("%d",i);
		dc.TextOut(m_ptArr[i].x,m_ptArr[i].y,strText);	//�������
	}
	dc.LineTo(m_ptArr[0]);

}

/************************************************************************
文件：PreviewDlg.cpp
作者：朱星星
日期：2009-6-6
说明：方案预览窗口实现文件，主要是显示各个方案中草药在地图中的大致分布情况。
预览窗口最好最大化，这样才能查看完全。
************************************************************************/

#include "stdafx.h"
#include "HerbGatherEditor.h"
#include "PreviewDlg.h"


// CPreviewDlg 对话框

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


// CPreviewDlg 消息处理程序

/************************************************************************
函数：
参数：
返回：
作者：朱星星
日期：2009-6-6
说明：读取m_strMethodFile方案文件，加载坐标信息
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
		//放大一倍，否则画出的点太紧凑
		m_ptArr[i].x*=2;	
		m_ptArr[i].y*=2;
	}
	ar.Close();
	file.Close();
	m_bInited=TRUE;
	this->SetWindowText(strMethodName);

	//最大化显示窗口，这样才能查看全部的坐标信息
	ShowWindow(SW_SHOWMAXIMIZED);
	return CDialog::OnInitDialog();
}

void CPreviewDlg::OnPaint()
{
	CDialog::OnPaint();
	if (!m_bInited || m_nPlaceCnt==0)
		return;

	CClientDC dc(this);
	CBrush brush(RGB(0,128,0));		//草绿色
	dc.SelectObject(brush);
	dc.SetBkMode(TRANSPARENT);

	dc.MoveTo(m_ptArr[0]);

	for (int i=0;i<m_nPlaceCnt;i++)
	{
		CString strText;
		CRect rect(m_ptArr[i].x-4,m_ptArr[i].y-4,m_ptArr[i].x+4,m_ptArr[i].y+4);
		dc.LineTo(m_ptArr[i]);
		dc.Ellipse(rect);			//原点标示
		strText.Format("%d",i);
		dc.TextOut(m_ptArr[i].x,m_ptArr[i].y,strText);	//坐标序号
	}
	dc.LineTo(m_ptArr[0]);

}
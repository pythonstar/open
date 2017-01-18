// TipDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "���ݴ���.h"
#include "TipDlg.h"


// CTipDlg �Ի���

IMPLEMENT_DYNAMIC(CTipDlg, CDialog)

CTipDlg::CTipDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTipDlg::IDD, pParent)
{

}

CTipDlg::~CTipDlg()
{
}

void CTipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTipDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_KILLFOCUS()
	ON_WM_TIMER()
END_MESSAGE_MAP()


//ע��HRGN���ͱ���ֻ�ֲܾ�ʹ�ã�ʹ�ú�DeleteObjectɾ��֮��
BOOL CTipDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//��ʼ�������ε�����������
	m_Points[0].x=238;
	m_Points[0].y=25;
	m_Points[1].x=238;
	m_Points[1].y=50;
	m_Points[2].x=213;
	m_Points[2].y=25;

	//������������
	HRGN hSrcRgn1=CreateRoundRectRgn(0,0,251,38,13,13);
	HRGN hSrcRgn2=CreatePolygonRgn(m_Points,3,ALTERNATE);
	HRGN hRgn=CreateRectRgn(0,0,251,63);
	CombineRgn(hRgn,hSrcRgn1,hSrcRgn2,RGN_OR);
	SetWindowRgn(hRgn,TRUE);
	DeleteObject(hRgn);
	DeleteObject(hSrcRgn2);
	DeleteObject(hSrcRgn1);

	//���ô���λ�ã��������εĵ�λ����긽��
	POINT pt;
	GetCursorPos(&pt);
	SetWindowPos(0,pt.x-238,pt.y-50,0x0fb,0x3f,0x50);
	
	m_strTip="���ܷ�����Ϣ����ֻ������ʽ������Ϣ";

	//������ʱ����3�����Զ��رմ���
	m_dwShowTime=3;
	SetTimer(0x200,1000,NULL);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CTipDlg::SetTipString(CString strTip)
{
	m_strTip=strTip;
}

//ע��HRGN���ͱ���ֻ�ֲܾ�ʹ�ã�ʹ�ú�DeleteObjectɾ��֮��
void CTipDlg::OnPaint()
{
	HRGN hSrcRgn1=CreateRoundRectRgn(0,0,251,38,13,13);
	HRGN hSrcRgn2=CreatePolygonRgn(m_Points,3,ALTERNATE);
	HRGN hRgn=CreateRectRgn(0,0,251,63);
	CombineRgn(hRgn,hSrcRgn1,hSrcRgn2,RGN_OR);

	CRect rectText(0,0,251,38);
	CDC* pDc=GetDC();
	CFont font;
	font.CreatePointFont(0x5a,"����");
	pDc->SelectObject(font);
	FillRgn(pDc->m_hDC,hRgn,CreateSolidBrush(RGB(255,251,239)));
	FrameRgn(pDc->m_hDC,hRgn,CreateSolidBrush(RGB(198,195,198)),1,1);

	pDc->SetBkMode(TRANSPARENT);
	pDc->DrawText(m_strTip,&rectText,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	ReleaseDC(pDc);
	
	DeleteObject(hRgn);
	DeleteObject(hSrcRgn2);
	DeleteObject(hSrcRgn1);

	CDialog::OnPaint();
}

//ʧȥ����ʱ�Զ��رմ���
void CTipDlg::OnKillFocus(CWnd* pNewWnd)
{
	this->~CTipDlg();
}

//3�����Զ��رմ���
void CTipDlg::OnTimer( UINT )
{
	if (!m_dwShowTime--)
	{
		KillTimer(0x200);
		this->~CTipDlg();
	}
}

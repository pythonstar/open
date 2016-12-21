// MyButton.cpp : 实现文件
//

#include "stdafx.h"
#include "GetUserList.h"
#include "MyButton.h"


// CMyButton

IMPLEMENT_DYNAMIC(CMyButton, CButton)

CMyButton::CMyButton()
{
	m_bMouseDown=FALSE;
}

CMyButton::~CMyButton()
{
}


BEGIN_MESSAGE_MAP(CMyButton, CButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CMyButton 消息处理程序

void CMyButton::OnLButtonDown(
						   UINT nFlags,
						   CPoint point 
						   )
{
	m_bMouseDown=TRUE;
	SetCursor(AfxGetApp()->LoadCursor(IDC_CROSS));
	SetCapture();
}
void CMyButton::OnLButtonUp(
							   UINT nFlags,
							   CPoint point 
							   )
{
	m_bMouseDown=FALSE;
	ReleaseCapture();
	SetCursor(AfxGetApp()->LoadCursor(IDC_ARROW));
}
void CMyButton::OnMouseMove(
							   UINT nFlags,
							   CPoint point 
							   )
{
	if (m_bMouseDown)
	{
		POINT pt;
		GetCursorPos(&pt);
		m_hHandle=::WindowFromPoint(pt);
		TCHAR szClass[100];
		GetClassName(m_hHandle,szClass,100);
		this->SetWindowText(szClass);
	}
	
}

#pragma once


// CMyButton

class CMyButton : public CButton
{
	DECLARE_DYNAMIC(CMyButton)
private:
	BOOL m_bMouseDown;
public:
	HWND m_hHandle;
public:
	CMyButton();
	virtual ~CMyButton();

protected:
	afx_msg void OnLButtonDown(
		UINT nFlags,
		CPoint point 
		);
	afx_msg void OnLButtonUp(
		UINT nFlags,
		CPoint point 
		);
	afx_msg void OnMouseMove(
		UINT nFlags,
		CPoint point 
		);

	DECLARE_MESSAGE_MAP()
};



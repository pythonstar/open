#pragma once


// CStaticTypeWord

class CStaticTypeWord : public CStatic
{
	DECLARE_DYNAMIC(CStaticTypeWord)
private:
	HDC   hMemDC;
	CDC   *ImgDC;
	DWORD dwFlashCounts;
	BOOL  bIsStrOver;
	BOOL  bIsFlashing;
	SIZE  TextSize;
	DWORD dwPosx,dwPosy;
	DWORD dwIndex;
	TCHAR  szCopyRight[1024];
public:
	void SetTypedWords(TCHAR *szWords);	//设置绘画内容，换行符为\n,结束符为0
	void StartTpyeWords();	//开启绘画

public:
	CStaticTypeWord();
	virtual ~CStaticTypeWord();
protected:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	DECLARE_MESSAGE_MAP()
};



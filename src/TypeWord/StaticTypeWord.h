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
	void SetTypedWords(TCHAR *szWords);	//���û滭���ݣ����з�Ϊ\n,������Ϊ0
	void StartTpyeWords();	//�����滭

public:
	CStaticTypeWord();
	virtual ~CStaticTypeWord();
protected:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	DECLARE_MESSAGE_MAP()
};



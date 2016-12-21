// StaticTypeWord.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TypeWord.h"
#include "StaticTypeWord.h"


// CStaticTypeWord

IMPLEMENT_DYNAMIC(CStaticTypeWord, CStatic)

CStaticTypeWord::CStaticTypeWord()
{
	dwFlashCounts=0;
	dwIndex=0;
	dwPosx=dwPosy=10;
	wcscpy(szCopyRight,_T("����Ч�������\n֧����Ӣ�Ļ�������\n����ʹ��\\n�ַ���\n�ı�����ʹ��\\0�ַ���\n    CopyRight: singer\n    2009-03-05\n\0"));
}

CStaticTypeWord::~CStaticTypeWord()
{
}
void CStaticTypeWord::SetTypedWords(TCHAR *szWords)
{
	wcscpy(szCopyRight,szWords);
}
void CStaticTypeWord::StartTpyeWords()
{
	HBITMAP hBmp;
	LOGFONT LogFont;
	HFONT hFont;

	ImgDC=this->GetDC();
	hMemDC=CreateCompatibleDC(ImgDC->m_hDC);
	hBmp=CreateCompatibleBitmap(ImgDC->m_hDC,178,157);
	SelectObject(hMemDC,hBmp);
	DeleteObject(hBmp);
	SetTextColor(hMemDC,0x00ff00);//������ɫѡ����ɫ
	SetBkMode(hMemDC,TRANSPARENT);


	memset(&LogFont,0,sizeof(LogFont));
	LogFont.lfHeight=12;
	LogFont.lfCharSet=1;
	wcscpy(LogFont.lfFaceName,_T("����"));
	hFont=CreateFontIndirect(&LogFont);	//�����߼�����
	SelectObject(hMemDC,hFont);			//ѡ���ڴ��豸������,׼��ʹ��

	GetTextExtentPoint32(hMemDC,szCopyRight,1,&TextSize);//��ȡ������
	::SetTimer(this->m_hWnd,0x200,100,NULL);
}

// CTypeWordDlg ��Ϣ�������
void CStaticTypeWord::OnTimer(UINT_PTR nIDEvent)
{
	if (dwFlashCounts==0)//����Ҫ��˸
	{
		PatBlt(hMemDC,dwPosx,dwPosy,TextSize.cx,TextSize.cy,BLACKNESS);//�����ϴε��»���
		TextOut(hMemDC,dwPosx,dwPosy,&szCopyRight[dwIndex],1);//ÿ��ֻ���һ���ַ�
		
		if (szCopyRight[dwIndex+1]=='\n')//��Ҫ����
		{
			bIsStrOver=(szCopyRight[dwIndex+2]==0);//�Ƿ񵽴ﴮβ,���¿�ʼ
			//���ﱾ��ĩβʱ,����һ��,ʵ��"����Ч��"
			bIsFlashing=TRUE;
			dwFlashCounts=5;
		}
		else
			TextOut(hMemDC,dwPosx+TextSize.cx,dwPosy,_T("_"),1);//��һ���ַ�����ӡһ��'_'

		if (szCopyRight[dwIndex]&0xff00)//����
			dwPosx+=TextSize.cx;
		else//Ӣ���ַ������ֵ�
			dwPosx+=TextSize.cx/2;

		dwIndex++;
	}
	else//��Ҫ��˸
	{
		if (bIsFlashing)
			TextOut(hMemDC,dwPosx,dwPosy,_T("_"),1);//���»���
		else
			PatBlt(hMemDC,dwPosx,dwPosy,TextSize.cx,TextSize.cy,BLACKNESS);//�����»���
		bIsFlashing=!bIsFlashing;

		if (--dwFlashCounts==0)
		{
			if (bIsStrOver)//��Ϣ�Ѿ���ʾ���
			{
				dwPosy=TextSize.cy;
				bIsStrOver=FALSE;
				PatBlt(hMemDC,0,0,178,157,BLACKNESS);
				dwIndex=0;
				dwPosx=10;
			}
			else
			{
				PatBlt(hMemDC,dwPosx,dwPosy,TextSize.cx,TextSize.cy,BLACKNESS);//�����»���
				bIsFlashing=FALSE;
				dwPosy+=TextSize.cy+TextSize.cy;
				dwPosx=10-TextSize.cx;
			}
		}
	}
	BitBlt(ImgDC->m_hDC,0,0,178,157,hMemDC,0,0,SRCCOPY);
}


BEGIN_MESSAGE_MAP(CStaticTypeWord, CStatic)
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CStaticTypeWord ��Ϣ�������



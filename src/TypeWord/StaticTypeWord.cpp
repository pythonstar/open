// StaticTypeWord.cpp : 实现文件
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
	wcscpy(szCopyRight,_T("打字效果组件，\n支持中英文混合输出，\n换行使用\\n字符，\n文本结束使用\\0字符，\n    CopyRight: singer\n    2009-03-05\n\0"));
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
	SetTextColor(hMemDC,0x00ff00);//字体颜色选用绿色
	SetBkMode(hMemDC,TRANSPARENT);


	memset(&LogFont,0,sizeof(LogFont));
	LogFont.lfHeight=12;
	LogFont.lfCharSet=1;
	wcscpy(LogFont.lfFaceName,_T("宋体"));
	hFont=CreateFontIndirect(&LogFont);	//创建逻辑字体
	SelectObject(hMemDC,hFont);			//选金内存设备描述表,准备使用

	GetTextExtentPoint32(hMemDC,szCopyRight,1,&TextSize);//获取字体宽高
	::SetTimer(this->m_hWnd,0x200,100,NULL);
}

// CTypeWordDlg 消息处理程序
void CStaticTypeWord::OnTimer(UINT_PTR nIDEvent)
{
	if (dwFlashCounts==0)//不需要闪烁
	{
		PatBlt(hMemDC,dwPosx,dwPosy,TextSize.cx,TextSize.cy,BLACKNESS);//擦除上次的下划线
		TextOut(hMemDC,dwPosx,dwPosy,&szCopyRight[dwIndex],1);//每次只打出一个字符
		
		if (szCopyRight[dwIndex+1]=='\n')//需要换行
		{
			bIsStrOver=(szCopyRight[dwIndex+2]==0);//是否到达串尾,重新开始
			//到达本行末尾时,锁定一下,实现"闪标效果"
			bIsFlashing=TRUE;
			dwFlashCounts=5;
		}
		else
			TextOut(hMemDC,dwPosx+TextSize.cx,dwPosy,_T("_"),1);//下一个字符处打印一个'_'

		if (szCopyRight[dwIndex]&0xff00)//汉字
			dwPosx+=TextSize.cx;
		else//英文字符，数字等
			dwPosx+=TextSize.cx/2;

		dwIndex++;
	}
	else//需要闪烁
	{
		if (bIsFlashing)
			TextOut(hMemDC,dwPosx,dwPosy,_T("_"),1);//画下划线
		else
			PatBlt(hMemDC,dwPosx,dwPosy,TextSize.cx,TextSize.cy,BLACKNESS);//擦除下划线
		bIsFlashing=!bIsFlashing;

		if (--dwFlashCounts==0)
		{
			if (bIsStrOver)//信息已经显示完毕
			{
				dwPosy=TextSize.cy;
				bIsStrOver=FALSE;
				PatBlt(hMemDC,0,0,178,157,BLACKNESS);
				dwIndex=0;
				dwPosx=10;
			}
			else
			{
				PatBlt(hMemDC,dwPosx,dwPosy,TextSize.cx,TextSize.cy,BLACKNESS);//擦除下划线
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



// CStaticTypeWord 消息处理程序



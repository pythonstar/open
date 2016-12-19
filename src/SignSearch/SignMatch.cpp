
#include "stdafx.h"
#include "SignMatch.h"

CSignMatch::CSignMatch()
{
	memset(m_szOrigSign,0,CSignMatch::MaxSignLen);
	m_pPureSign=NULL;
	m_pMaskSign=NULL;
	m_nSignLen=0;
	m_dwSearchBegin=0;
	m_dwSearchEnd=0;
	m_dwMatchPos=0;
}

CSignMatch::CSignMatch(PBYTE pBase,DWORD dwStartRva,DWORD dwEndRva,char*szOrigSign)
{
	memset(m_szOrigSign,0,CSignMatch::MaxSignLen);
	m_pPureSign=NULL;
	m_pMaskSign=NULL;
	m_nSignLen=0;

	SetOrigSign(pBase,dwStartRva,dwEndRva,szOrigSign);
}

CSignMatch::CSignMatch(const CSignMatch&obj)
{
	memcpy(m_szOrigSign,obj.m_szOrigSign,CSignMatch::MaxSignLen);
	m_nSignLen=obj.m_nSignLen;
	m_pPureSign=new BYTE[m_nSignLen];
	m_pMaskSign=new BYTE[m_nSignLen];
	memcpy(m_pPureSign,obj.m_pPureSign,m_nSignLen);
	memcpy(m_pMaskSign,obj.m_pMaskSign,m_nSignLen);

	m_pBase=obj.m_pBase;
	m_dwSearchBegin=obj.m_dwSearchBegin;
	m_dwSearchEnd=obj.m_dwSearchEnd;
	m_nRange=obj.m_nRange;
	m_dwMatchPos=obj.m_dwMatchPos;
}

CSignMatch::~CSignMatch()
{
	FreeMemory();
}

void CSignMatch::FreeMemory()
{
	if (m_pPureSign)
	{
		delete[] m_pPureSign;
		m_pPureSign=NULL;
	}
	if (m_pMaskSign)
	{
		delete[] m_pMaskSign;
		m_pMaskSign=NULL;
	}
}

BYTE CSignMatch::HexCharToBin(BYTE chHex)
{
	if ( chHex>='0' && chHex<='9' )
	{
		return chHex-'0';
	}
	else
	{
		return chHex-'a'+10;
	}

/*
	if ( chHex>='0' && chHex<='9' )
	{
		return chHex-'0';
	}
	else if( (chHex>='a'&&chHex<='f') || (chHex>='A'&&chHex<='F') )
	{
		return chHex-'a'+10;
	}
	else
	{
		MessageBox(NULL,"错误的十六进制字符",NULL,MB_OK);
		return 0;
	}*/

}

/*------------------------------------------------------------------------
说明:   接受16进制的特征码字符串,((orig or mask) xor pure) ==0
------------------------------------------------------------------------*/
BOOL CSignMatch::SetOrigSign(PBYTE pBase,DWORD dwStartRva,DWORD dwEndRva,char*szOrigSign)
{
	m_pBase=pBase;
	m_dwSearchBegin=dwStartRva;
	m_dwSearchEnd=dwEndRva;
	m_dwMatchPos=0;
	m_nRange=dwEndRva-dwStartRva;

	int nStrLen=(int)strlen(szOrigSign);
	//太短太长都不行
	if (nStrLen<CSignMatch::MinSignLen || nStrLen>CSignMatch::MaxSignLen)
	{
		//MessageBox(NULL,"特征码设置不合理,长度限制在5~100字节.",NULL,MB_OK);
		return false;
	}
	else
	{
		if (nStrLen%2!=0)
		{
			//奇数长度,则尾补一个?构成一个字节
			memcpy(m_szOrigSign,szOrigSign,nStrLen);
			nStrLen++;
			if ( nStrLen<CSignMatch::MaxSignLen ){
				m_szOrigSign[nStrLen]='?';
			}
		}
		else
		{
			memcpy(m_szOrigSign,szOrigSign,nStrLen);
		}
		m_nSignLen=nStrLen/2;

		FreeMemory();
		m_pPureSign=new BYTE[m_nSignLen];
		m_pMaskSign=new BYTE[m_nSignLen];
		BYTE ch;
		for (int i=0;i<m_nSignLen;i++)
		{
			//一个字符只占4位(半个字节),处理第一个半字节
			ch=m_szOrigSign[i*2];
			if (ch=='?')
			{
				m_pPureSign[i]=0xf0;
				m_pMaskSign[i]=0xf0;
			}
			else
			{
				m_pPureSign[i]=(HexCharToBin(ch)<<4)&0xf0;
				m_pMaskSign[i]=0;
			}

			//处理第二个半字节
			ch=m_szOrigSign[i*2+1];
			if (ch=='?')
			{
				m_pPureSign[i]|=0x0f;
				m_pMaskSign[i]|=0x0f;
			}
			else
			{
				m_pPureSign[i]|= HexCharToBin(ch)&0x0f;
				m_pMaskSign[i]&=0xf0;
			}

		}

		return true;
	}
}

PBYTE CSignMatch::SearchSign()
{
	PBYTE pMatchAddr=NULL;
	PBYTE pStart=m_pBase+m_dwSearchBegin;
	if (m_nRange<m_nSignLen)
	{
		return NULL;
	}
	int*p=new int[m_nSignLen+1];

	//TRACE("m_nSignLen:%d m_nRange:%d\n",m_nSignLen,m_nRange);
	//TRACE("pStart:%d\n",(DWORD)pStart);

	__try
	{
		p[1]=0;
		int i=1,j=0;
		for (;i<(int)m_nSignLen;i++)
		{
			while (j>0 && m_pPureSign[j]!=m_pPureSign[i])
				j=p[j];
			if(m_pPureSign[j]==m_pPureSign[i])
				j++;
			p[i+1]=j;
		}

		for (i=0,j=0;i<(int)m_nRange;i++)
		{
			while( j>0 &&m_pPureSign[j]!=(pStart[i]|m_pMaskSign[j]) )
				j=p[j];
			if(m_pPureSign[j]==(pStart[i]|m_pMaskSign[j]))
				j++;
			if (j==m_nSignLen)
			{
				pMatchAddr=pStart+i-m_nSignLen+1;
				m_dwMatchPos=m_dwSearchBegin+i-m_nSignLen+1;
				break;
			}
		}
		
		delete[]p;
	}
	__except(1)
	{
		if (p)
		{
			delete[]p;
		}
		pMatchAddr=NULL;
	}

	return pMatchAddr;
}

PBYTE CSignMatch::SearchProcessSign(HANDLE hProcess)
{
	PBYTE pMatchAddr=NULL;
	DWORD dwReaded=0;
	MODULEINFO mi;
	BYTE*pBuffer=NULL;
	BYTE*pSrc=NULL;

	//获取模块的大小
	GetModuleInformation(hProcess,(HMODULE)m_pBase,&mi,sizeof(mi));

	//不能越界
	if (m_dwSearchBegin>=mi.SizeOfImage || m_dwSearchEnd>=mi.SizeOfImage || m_nRange<m_nSignLen)
	{
		return NULL;
	}

	//开辟内存
	pBuffer=new BYTE[m_nRange];
	if( ReadProcessMemory(hProcess,(LPCVOID)(m_pBase+m_dwSearchBegin),pBuffer,m_nRange,&dwReaded) )
	{
		PBYTE pStart=pBuffer;
		
		int*p=new int[m_nSignLen+1];

		__try
		{
			p[1]=0;
			int i=1,j=0;
			for (;i<(int)m_nSignLen;i++)
			{
				while (j>0 && m_pPureSign[j]!=m_pPureSign[i])
					j=p[j];
				if(m_pPureSign[j]==m_pPureSign[i])
					j++;
				p[i+1]=j;
			}

			for (i=0,j=0;i<(int)m_nRange;i++)
			{
				while(j>0 &&m_pPureSign[j]!=(pStart[i]|m_pMaskSign[j]) )
					j=p[j];
				if(m_pPureSign[j]==(pStart[i]|m_pMaskSign[j]))
					j++;
				if (j==m_nSignLen)
				{
					pMatchAddr=m_pBase+m_dwSearchBegin+i-m_nSignLen+1;
					m_dwMatchPos=m_dwSearchBegin+i-m_nSignLen+1;
					break;
				}
			}

			delete[]p;
		}
		__except(1)
		{
			if (p)
			{
				delete[]p;
			}
			pMatchAddr=NULL;
		}

		
	}

	delete[] pBuffer;
	return pMatchAddr;

}

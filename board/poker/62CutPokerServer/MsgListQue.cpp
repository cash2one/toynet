
#include "stdafx.h"
#include "MsgListQue.h"

CMsgListQue::CMsgListQue()
{
	InitializeCriticalSection(&m_CriticalSection);

	memset(m_pBuf, 0, sizeof(m_pBuf));
	memset(m_nBufSize, 0, sizeof(m_nBufSize));
	memset(m_nSignal, 0, sizeof(m_nSignal));
	memset(m_nSID, 0, sizeof(m_nSID));

	m_nNowSPos = 0;
	m_nQuededNum = 0;
	m_nTotBufSize = 0;
}

CMsgListQue::~CMsgListQue()
{
	DeleteCriticalSection(&m_CriticalSection);
	Clear();
}

void CMsgListQue::Clear()
{
	for(int i=0; i<MAX_MSGLISTNUM; i++)
	{
		if(m_pBuf[i]) { delete m_pBuf[i]; m_pBuf[i]=NULL; }
	}

	memset(m_nBufSize, 0, sizeof(m_nBufSize));
	m_nNowSPos = 0;
	m_nQuededNum = 0;
	m_nTotBufSize = 0;
}

BOOL CMsgListQue::EnQue(void *pData, int size, int signal, int sid)
{
	EnterCriticalSection(&m_CriticalSection);

	if(pData==NULL || size<=0) { LeaveCriticalSection(&m_CriticalSection); return FALSE; }
	if(m_nQuededNum>=MAX_MSGLISTNUM) { LeaveCriticalSection(&m_CriticalSection); return FALSE; }
	if(m_nTotBufSize + size > MAX_MSGBUFFERLIMIT) { LeaveCriticalSection(&m_CriticalSection); return FALSE; }

	int pos = m_nNowSPos+m_nQuededNum;
	if(pos >= MAX_MSGLISTNUM) pos -= MAX_MSGLISTNUM;

	m_pBuf[pos] = (char*)pData;
	m_nBufSize[pos] = size;
	m_nTotBufSize += size;
	m_nSignal[pos] = signal;
	m_nSID[pos] = sid;

	m_nQuededNum++;
	LeaveCriticalSection(&m_CriticalSection);
	return TRUE;
}

BOOL CMsgListQue::DeQue()
{
	EnterCriticalSection(&m_CriticalSection);

	if(m_nQuededNum<=0) { LeaveCriticalSection(&m_CriticalSection); return FALSE; }
	if(m_pBuf[m_nNowSPos]) { delete m_pBuf[m_nNowSPos]; m_pBuf[m_nNowSPos]=NULL; }
	m_nTotBufSize -= m_nBufSize[m_nNowSPos];
	m_nBufSize[m_nNowSPos] = 0;
	m_nQuededNum--;
	m_nNowSPos++;
	if(m_nNowSPos >= MAX_MSGLISTNUM) m_nNowSPos=0;

	LeaveCriticalSection(&m_CriticalSection);
	return TRUE;
}
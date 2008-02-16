#include "stdafx.h"
#include "StreamBuf.h"


// 스트림 버퍼 관리 클래스

CStreamBuf::CStreamBuf()
{
	pBuf = NULL;
	nBufSize = nUsedSize = 0;
}

CStreamBuf::~CStreamBuf()
{
	Destroy();
}

BOOL CStreamBuf::Init(int size)
{
	if(size==0 || size<0) return FALSE;
	if(pBuf != NULL) delete pBuf;
	pBuf = new char[size];
	if(pBuf == NULL)
	{
		nBufSize = nUsedSize = 0;
		return FALSE;
	}
	nBufSize = size;
	nUsedSize = 0;
	return TRUE;
}

void CStreamBuf::Destroy()
{
	if(pBuf) delete pBuf;
	pBuf = NULL;
	nBufSize = nUsedSize = 0;
}

BOOL CStreamBuf::AddUsedSize(int size)
{
	if(nUsedSize+size > nBufSize) return FALSE;
	else
	{
		nUsedSize += size;
		return TRUE;
	}
}

BOOL CStreamBuf::DelData(int size)
{
	if( nUsedSize < size || size < 0) 
		return FALSE;
	else
	{
		if(size==0) return TRUE;
		//if(IsBadReadPtr(pBuf, 4)) return FALSE;
		
		memcpy(pBuf, pBuf+size, nUsedSize-size);
		nUsedSize -= size;
		return TRUE;
	}
}

int CStreamBuf::AddData(void* pData, int size)
{
	if(pData == NULL) return 0;
	if(size<=0 || size>(nBufSize-nUsedSize)) return 0;
	memcpy(pBuf+nUsedSize, pData, size);
	nUsedSize += size;
	return size;
}

int CStreamBuf::AddData_Encode(void* pData, int size)
{
	if(pData == NULL) return 0;
	if(size<=0 || size>(nBufSize-nUsedSize)) return 0;
	// 암호화 기록
	for(int i=0; i<size; i++) pBuf[nUsedSize+i] = ((char*)pData)[i]^'J'^'H';
	nUsedSize += size;
	return size;
}

int CStreamBuf::Data_Decode(int start, int size)
{
	if(pBuf == NULL) return 0;
	if(size<=0 || start<0 || start>=nBufSize) return 0;
	if(start+size<=0 || start+size>nBufSize) return 0;
	// 암호화 디코딩
	for(int i=start; i<start+size; i++) pBuf[i] = pBuf[i]^'J'^'H';
	return size;
}

int CStreamBuf::GetData(void *pTarget, int tarsize)
{
	if(nUsedSize==0) return 0;
	if(tarsize < nUsedSize) return 0;
	memcpy(pTarget, pBuf, nUsedSize);
	int rtn = nUsedSize;
	nUsedSize = 0;
	return rtn;
}

int CStreamBuf::PeekData(void *pTarget, int peeksize, int tarsize)
{
	if(tarsize < nUsedSize) return 0;
	if(nUsedSize==0 || nUsedSize<peeksize) return 0;
	memcpy(pTarget, pBuf, peeksize);
	return 0;
}


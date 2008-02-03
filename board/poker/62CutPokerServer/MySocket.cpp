
#include "stdafx.h"
#include "MySocket.h"

#include "SockMan.h"
#include "CommMsg.h"

#pragma message("linking with SockEngine library")
#pragma comment(lib, "SockEngine.lib")

#define MAX_PACKETPOOL 30

class CPacketPool
{
public:
	int  m_Index;
	char m_Pool[MAX_PACKETPOOL][50*1024];

	CPacketPool()
	{
		m_Index = 0;
		memset(m_Pool, 0, sizeof(m_Pool));
	}

	char* GetNext()
	{
		char* prtn = m_Pool[m_Index];
		m_Index++;
		if(m_Index >= MAX_PACKETPOOL)
			m_Index = 0;
		return prtn;
	}
};

// 전역 센드 패킷 풀
CPacketPool g_sSendPool;

CMySocket::CMySocket(HWND hWnd, HANDLE hIocp, int SendQueSize, int RecvQueSize) : CFastSocket(hIocp, SendQueSize, RecvQueSize)
{
	m_hParentWnd = hWnd;
	m_iAcceptedHostIndex = -1;
}

CMySocket::~CMySocket()
{
	// 베이스 소멸자는 자동으로 호출된다
	m_hParentWnd = NULL;
}

// 이벤트 가상함수
void CMySocket::OnAccept(int nErrorCode)
{
	if(m_hParentWnd) {
		::PostMessage(m_hParentWnd, UM_SOCK_ACCEPT,  nErrorCode, (LONG)this);
	}
}

void CMySocket::OnClose(int nErrorCode)
{
	if(m_hParentWnd) {
		int sid = m_dwUserVal;
		::PostMessage(m_hParentWnd, UM_SOCK_CLOSE,  sid, (LONG)this);
	}
}

void CMySocket::OnConnect(int nErrorCode)
{
	if(m_hParentWnd) {
		int sid = m_dwUserVal;
		::PostMessage(m_hParentWnd, UM_SOCK_CONNECT,  sid, (LONG)this);
	}
}

void CMySocket::OnReceive(int nErrorCode)
{
	// 아무일도 하지 않음 (리시브 데이터 처리는 타이머 풀링을 이용한다)
}

BOOL CMySocket::Send(void *pData, int size)
{
	if(!pData || size <= 0) return FALSE;

	// 임시 버퍼에 데이터를 복사(이 임시버퍼는 멀티 쓰레드로부터 완전히 자유롭지는 않다 - 속도를 위하여 편법 적용)
	char* pbuf = g_sSendPool.GetNext();
	memcpy(pbuf, pData, size);

	// 패킷 번호를 순차 증가(패킷 스니핑 체크용)
	USERVAL *pUserVal = (USERVAL*)(m_pUserPtr);
	pUserVal->SendPackCnt++;
	// 패킷 암호화
	PACKETHEADER* phead = (PACKETHEADER*)pbuf;
	if(phead->Dummy[1] != 2) {
		phead->PackCnt = pUserVal->SendPackCnt;
		CCommMsg::EncodeAll((char*)pbuf);
	}
	return CFastSocket::Send(pbuf, size);
}


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

// ���� ���� ��Ŷ Ǯ
CPacketPool g_sSendPool;

CMySocket::CMySocket(HWND hWnd, HANDLE hIocp, int SendQueSize, int RecvQueSize) : CFastSocket(hIocp, SendQueSize, RecvQueSize)
{
	m_hParentWnd = hWnd;
	m_iAcceptedHostIndex = -1;
}

CMySocket::~CMySocket()
{
	// ���̽� �Ҹ��ڴ� �ڵ����� ȣ��ȴ�
	m_hParentWnd = NULL;
}

// �̺�Ʈ �����Լ�
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
	// �ƹ��ϵ� ���� ���� (���ú� ������ ó���� Ÿ�̸� Ǯ���� �̿��Ѵ�)
}

BOOL CMySocket::Send(void *pData, int size)
{
	if(!pData || size <= 0) return FALSE;

	// �ӽ� ���ۿ� �����͸� ����(�� �ӽù��۴� ��Ƽ ������κ��� ������ ���������� �ʴ� - �ӵ��� ���Ͽ� ��� ����)
	char* pbuf = g_sSendPool.GetNext();
	memcpy(pbuf, pData, size);

	// ��Ŷ ��ȣ�� ���� ����(��Ŷ ������ üũ��)
	USERVAL *pUserVal = (USERVAL*)(m_pUserPtr);
	pUserVal->SendPackCnt++;
	// ��Ŷ ��ȣȭ
	PACKETHEADER* phead = (PACKETHEADER*)pbuf;
	if(phead->Dummy[1] != 2) {
		phead->PackCnt = pUserVal->SendPackCnt;
		CCommMsg::EncodeAll((char*)pbuf);
	}
	return CFastSocket::Send(pbuf, size);
}

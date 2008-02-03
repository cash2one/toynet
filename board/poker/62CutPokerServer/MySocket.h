#ifndef __MYSOCKET_H__
#define __MYSOCKET_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FastSocket.h"

#define UM_SOCK_ACCEPT	(WM_USER+700)			// 소켓 억셉트
#define UM_SOCK_CONNECT	(WM_USER+701)			// 연결 성공
#define UM_SOCK_RECEIVE	(WM_USER+702)			// 자체 버퍼에 데이터가 수신되어 있음
#define UM_SOCK_CLOSE	(WM_USER+703)			// 소켓이 닫혀야할 상황임

class CMySocket : public CFastSocket
{
public:
	HWND m_hParentWnd;
	int  m_iAcceptedHostIndex;

	CMySocket(HWND hWnd, HANDLE hIocp, int SendQueSize, int RecvQueSize);
	virtual ~CMySocket();

	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);

	BOOL Send(void *pData, int size);
};


#endif

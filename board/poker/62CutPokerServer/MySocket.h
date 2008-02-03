#ifndef __MYSOCKET_H__
#define __MYSOCKET_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FastSocket.h"

#define UM_SOCK_ACCEPT	(WM_USER+700)			// ���� ���Ʈ
#define UM_SOCK_CONNECT	(WM_USER+701)			// ���� ����
#define UM_SOCK_RECEIVE	(WM_USER+702)			// ��ü ���ۿ� �����Ͱ� ���ŵǾ� ����
#define UM_SOCK_CLOSE	(WM_USER+703)			// ������ �������� ��Ȳ��

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

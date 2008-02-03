// ServerSock.cpp : implementation file
//

#include "stdafx.h"

#include "ServerSock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerSock

CServerSock::CServerSock()
{
	hWND = NULL;
}

CServerSock::~CServerSock()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CServerSock, CAsyncSocket)
	//{{AFX_MSG_MAP(CServerSock)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CServerSock member functions

BOOL CServerSock::CreateSocket(HWND hwnd, UINT lPortNum, char *lpszSocketAddress)
{
	if(hWND) Close();
	
	/*
	// 소켓 생성
	WSADATA wsaData;
	SOCKADDR_IN sockAddr; 
	ZeroMemory(&sockAddr, sizeof(sockAddr));
	WORD wVerRequested = MAKEWORD(1,1);
	
	WSAStartup(wVerRequested, &wsaData);

	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sockAddr.sin_port = htons((u_short)m_portNum);

	m_hSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(m_hSrvSock == INVALID_SOCKET) 
	{
		AfxMessageBox("서버 소켓을 생성할 수 없습니다.");
		return FALSE;
	}
	
	if(bind(m_hSrvSock, (SOCKADDR*)&sockAddr, sizeof(sockAddr)) == 0)
	{
		wsprintf(strError,"bind() error : %d - 서버 소켓 초기화 에러", GetLastError());
		//AfxMessageBox(strError);
	}
	
	listen(m_hSrvSock, 5);
	*/

	// 소켓 생성

	if(Create( lPortNum, SOCK_STREAM, FD_READ|FD_WRITE|FD_OOB|FD_ACCEPT|FD_CONNECT|FD_CLOSE, lpszSocketAddress)==0)
	{
		m_strError.Format("서버용 소켓을 생성할 수 없습니다 CServerSock::Create() Error : %d", GetLastError());
		return FALSE;
	}
	
	if(Listen()==0)
	{
		m_strError.Format("서버용 소켓 대기에 실패했습니다. CServerSock::Listen() error : %d", GetLastError());
		return FALSE;
	}
	
	hWND=hwnd;
	return TRUE;
}

BOOL CServerSock::AcceptSocket(HWND hwnd, CDataSock *pSock, int sid)
{
	if(pSock==NULL) return FALSE;
	
	if(Accept(*pSock) == 0)
	{
		m_strError.Format("CServerSock::Accept() error : %d", GetLastError());
		return FALSE;
	}
	
	pSock->hSID = sid;				// 소켓 클래스 아이디
	pSock->bIsSrvSock = TRUE;		// 서버용 소켓임을 알림
	pSock->hWND = hwnd;
	pSock->bAvailable = TRUE;		// 유효한 소켓임을 알림
	return TRUE;
}


void CServerSock::OnAccept(int nErrorCode)
{
	if(nErrorCode == 0) SendMessage(hWND, UM_SERV_ACCEPT, NULL, (long)this);
	CAsyncSocket::OnAccept(nErrorCode);
}

void CServerSock::OnClose(int nErrorCode)
{
	PostMessage(hWND, UM_SERV_CLOSE, NULL, (long)this);
	CAsyncSocket::OnClose(nErrorCode);
}

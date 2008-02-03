// SockMan.cpp

#include "stdafx.h"
#include "MySocket.h"
#include "SockMan.h"


//////////////////////////////////////////////////////////////////

CSockMan::CSockMan()
{
	hWs2Dll = NULL;
	hIOCP = NULL;
	m_hWindow = NULL;

	memset(UserVal, 0, sizeof(UserVal));
	
	m_TotHostInfo = 0;
	m_PrimaryHostIndex = 0;
	memset(HostInfo, 0, sizeof(HostInfo));

	m_UsedHostNum = 0;
	memset(UsedHostInfo, 0, sizeof(UsedHostInfo));

	memset(m_strDomain, 0, sizeof(m_strDomain));

	pServerSock = NULL;
	for(int i=0; i<MAX_DATA_SOCKET; i++) pDataSock[i] = NULL;
}

CSockMan::~CSockMan()
{
	Destroy();
}

BOOL CSockMan::Init(HWND hwnd, int workerthreadnum, BOOL bUseAllHost, int primaryhost)
{
	Destroy();

	// Winsock2 DLL
	hWs2Dll = LoadLibrary("WS2_32.DLL");

	if(hWs2Dll == NULL) {
		MessageBox(hwnd, "Winsock2 DLL(WS2_32.DLL)�� �� �� �����ϴ�", "Winsock ����", MB_OK);
		return FALSE;
	}
	
	// ���� �ʱ�ȭ
	WSADATA wd = { 0 };
	int e = WSAStartup( MAKEWORD( 2, 2 ), &wd );
	SetLastError( e ); // for _err()

	if(e != 0) {
		MessageBox(hwnd, "Failed on start up winsock", "Winsock Error", MB_OK);
		return FALSE;
	}
	
	if(LOBYTE( wd.wVersion ) < 2) {
		MessageBox(hwnd, "Winsock�� ������ �����ϴ�", "Winsock Error", MB_OK);
		return FALSE;
	}

	if(IocpThread.Create(workerthreadnum)==FALSE) 
		return FALSE;

	hIOCP = IocpThread.GetIocpHandle();
	m_hWindow = hwnd;

	for(int i=0; i<MAX_DATA_SOCKET; i++) {
		if(!pDataSock[i]) pDataSock[i] = new CMySocket(m_hWindow, hIOCP, SENDQUESIZE, RECVQUESIZE);
	}

	CheckHostInfo();	// Host ������ üũ�Ѵ�

	m_UsedHostNum = 0;
	m_PrimaryHostIndex = 0;
	memset(UsedHostInfo, 0, sizeof(UsedHostInfo));

	//if(primaryhost >= 0 && primaryhost < m_UsedHostNum) {
	if(primaryhost >= 0 && primaryhost < m_TotHostInfo) {
		m_PrimaryHostIndex = primaryhost;
	}
	else {
		m_PrimaryHostIndex = 0;
		MessageBox(hwnd, "������ HOST�ּҴ� ��ȿ���� �ʱ� ������ 0�� ȣ��Ʈ�� Primary�� ���� ��ȯ�մϴ�.", "SockMan Initializing Warning", MB_OK);
	}

	if(bUseAllHost) {
		for(i=0; i < m_TotHostInfo; i++) {
			strcpy(UsedHostInfo[m_UsedHostNum].szSockAddr, HostInfo[i].szSockAddr);
			m_UsedHostNum++;
		}
	}
	else {
		strcpy(UsedHostInfo[0].szSockAddr, HostInfo[m_PrimaryHostIndex].szSockAddr);
		m_UsedHostNum = 1;
	}

	return TRUE;
}

// ������ ��� ���ϰ� Ŭ������ �ı�
void CSockMan::Destroy()
{
	IocpThread.Destroy();

	CloseServerSocket();

	m_UsedHostNum = 0;

	for(int i=0; i<MAX_DATA_SOCKET; i++) {
		if(pDataSock[i] != NULL) {
			pDataSock[i]->Destroy();
			delete pDataSock[i]; 
			pDataSock[i] = NULL;
		}
	}

	if(hWs2Dll) {
		WSACleanup();			// ����DLL Ŭ����
		FreeLibrary(hWs2Dll);	// Winsock2 DLL ����
		hWs2Dll = NULL;
	}
}

// Ŭ���� �ʱ�ȭ
BOOL CSockMan::CreateServerSocket(UINT port, char *lpszSockAddr)
{
	CloseServerSocket();

	pServerSock = new CMySocket(m_hWindow, hIOCP, SENDQUESIZE, RECVQUESIZE);

	if(pServerSock==NULL) {
		::MessageBox(m_hWindow, "Create failed at Server Socket", "Error", MB_OK);
		return FALSE;
	}
	
	// ���� ����
	if(pServerSock->Create()==FALSE) {
		::MessageBox(m_hWindow, "Create failed at Server Socket", "Error", MB_OK);
		return FALSE;
	}

	// ���ε�
	if(pServerSock->Bind(port, lpszSockAddr)==FALSE) {
		::MessageBox(m_hWindow, "Binding Error at Server Socket", "Error", MB_OK);
		return FALSE;
	}

	// ������ ������ ����
	if(pServerSock->Listen(15)==FALSE) {
		::MessageBox(m_hWindow, pServerSock->GetLastErrorString(), "Error", MB_OK);
		return FALSE;
	}

	return TRUE;
}

BOOL CSockMan::CloseServerSocket()
{
	if(pServerSock) {
		pServerSock->Destroy();
		delete pServerSock; 
		pServerSock = NULL;
	}

	return TRUE;
}

// ���� ��û�� �޾Ƶ��̰� ������ ������ �ʱ�ȭ��
int CSockMan::AcceptSocket(int *pHostIndex)
{
	if(pServerSock == NULL) {
		return -1;
	}

	for(int i=0; i<MAX_DATA_SOCKET; i++) {
		if(pDataSock[i] && !pDataSock[i]->m_bCreated) {
			// ������ ���� �迭 ��ȣ ����
			pDataSock[i]->m_dwUserVal = i;
			// ������ ���� ���� ������ ����
			pDataSock[i]->m_pUserPtr = &UserVal[i];
			memset(&UserVal[i], 0, sizeof(USERVAL));
			UserVal[i].hSID = i;

			// ������ ������ Accept�ϰ� �ʱ�ȭ �Ѵ�
			if(pServerSock->Accept(pDataSock[i])==FALSE) {
			#ifdef _DEBUG
				//::MessageBox(NULL, pDataSock[i]->GetLastErrorString(), "���Ʈ ����", MB_OK);
			#endif
				pDataSock[i]->Destroy();
				return -1; 
			}

			int hostindex = 0;

			if(m_UsedHostNum >= 2) {
				// Accept�� ȣ��Ʈ�� IP��巹���� ����
				SOCKADDR_IN addr_in;
				int namelen = sizeof(addr_in);
				if(SOCKET_ERROR  == getsockname(pDataSock[i]->m_hSocket, (SOCKADDR*)&addr_in, &namelen)) {
					pDataSock[i]->Destroy();
					return -1;
				}

				char* strHostAddr = inet_ntoa(addr_in.sin_addr);

				int findhost = -1;
				for(int h=0; h<m_UsedHostNum; h++) {
					if(strcmp(strHostAddr, UsedHostInfo[h].szSockAddr) == 0) {
						//AfxMessageBox(strHostAddr);
						findhost = h;
						break;
					}
				}

				if(findhost < 0) {
					pDataSock[i]->Destroy();
					return -1;
				}

				hostindex = findhost;
			}

			if(pHostIndex) *pHostIndex = hostindex;

			// ����Ʈ�� �Ͼ ȣ��Ʈ �ε����� ����
			pDataSock[i]->m_iAcceptedHostIndex = hostindex;
			// �ش� ȣ��Ʈ ���Ʈ ���� ���� ����
			UsedHostInfo[hostindex].AcceptedNum++;

			// �ùٸ��� ������ ��� ���� Ŭ���� �迭 ��ȣ�� �����Ѵ�
			return i;
		}
	}
	
	// ������ ������ ���� ��� �ϴ� ���Ʈ�ϰ� ������ Ŭ�����Ͽ� ���ҽ� ���� ����
	SOCKET s = accept(pServerSock->m_hSocket, NULL, NULL);
	if(s != INVALID_SOCKET) closesocket(s);

	//TRACE("CSockMan::AcceptSocket() - ���� ���� ���� �ʰ�\n");
	OutputDebugString("CSockMan::AcceptSocket() - ���� ���� ���� �ʰ�\n");
	return -1;
}

// Ŭ���̾�Ʈ������ ������ ������ ����, ���ϴ� �ּҿ� �����Ų��
// -1�� �����ϸ� ���� ���и� �������� ���� ����� ���߿� �޼����� �뺸�ȴ�
int CSockMan::ConnectSocket(char *strIP, UINT nPortNum)
{
	for(int i=0; i<MAX_DATA_SOCKET; i++) {
		if(pDataSock[i] && !pDataSock[i]->m_bCreated) {		
			// ���� ����
			pDataSock[i]->Create();
			// ������ ���� �迭 ��ȣ ����
			pDataSock[i]->m_dwUserVal = i;
			// ������ ���� ���� ������ ����
			pDataSock[i]->m_pUserPtr = &UserVal[i];
			memset(&UserVal[i], 0, sizeof(USERVAL));
			UserVal[i].hSID = i;

			if(pDataSock[i]->Connect(strIP, nPortNum)==FALSE) {
				pDataSock[i]->Destroy();
				return -1; // Ŭ���̾�Ʈ������ �ʱ�ȭ
			}
			return i;
		}
	}
	
	//TRACE("CSockMan::ConnectSocket() - ���� ���� ���� �ʰ�\n");
	OutputDebugString("CSockMan::ConnectSocket() - ���� ���� ���� �ʰ�\n");
	return -1;
}

CMySocket* CSockMan::GetDataSocketPtr(int sid)
{
	if(sid<0 || sid>=MAX_DATA_SOCKET) return NULL;
	return pDataSock[sid];
}

BOOL CSockMan::SendData(CMySocket* pSock, void *pdata, int size)
{
	if(pSock==NULL) {
		//TRACE("CSockMan::SendData() - pSock = NULL\n");
		OutputDebugString("CSockMan::SendData() - pSock = NULL\n");
		return FALSE;
	}
	int sid = pSock->m_dwUserVal;
	return SendData(sid, pdata, size);
}

BOOL CSockMan::SendData(int sid, void *pdata, int size)
{
	if(sid<0 || sid>=MAX_DATA_SOCKET) return FALSE;
	if(pDataSock[sid] == NULL) return FALSE;
	return pDataSock[sid]->Send(pdata, size);
}

BOOL CSockMan::ReservClose(int sid)
{
	if(sid<0 || sid>=MAX_DATA_SOCKET) return FALSE;
	if(pDataSock[sid] == NULL) return FALSE;
	pDataSock[sid]->ReservClose();
	return TRUE;
}

BOOL CSockMan::CloseDataSocket(CMySocket* pSock)
{
	if(pSock==NULL) return FALSE;
	if(!pSock->m_bCreated) return FALSE;
	int sid = pSock->m_dwUserVal;
	return CloseDataSocket(sid);
}

BOOL CSockMan::CloseDataSocket(int sid)
{
	if(sid<0 || sid>=MAX_DATA_SOCKET) return FALSE;
	if(pDataSock[sid] == NULL) return FALSE;
	if(!pDataSock[sid]->m_bCreated) return FALSE;
	
	int hostindex = pDataSock[sid]->m_iAcceptedHostIndex;
	if(hostindex >= 0) {
		UsedHostInfo[hostindex].AcceptedNum--;
	}
	
	pDataSock[sid]->Close();
	pDataSock[sid]->m_iAcceptedHostIndex = -1;

	memset(&UserVal[sid], 0, sizeof(USERVAL));
	return TRUE;
}

BOOL CSockMan::CheckHostInfo()
{
	/*
	// ȣ��Ʈ ���� ��ȸ
	char strHostName[256]={0,};
	char strPort[10]={0,};
	
	PHOSTENT pHostEntry;
	IN_ADDR inAddr;

	if(gethostname(strHostName, sizeof(strHostName)) != 0) return FALSE;
	else {
		pHostEntry = gethostbyname(strHostName);
		if(pHostEntry==NULL) return FALSE;
		else memcpy(&inAddr, pHostEntry->h_addr,4);
	}

	m_strSrvIP = inet_ntoa(inAddr);		// ȣ��Ʈ�� IP�� ���ڿ��� ��ȯ
	m_strDomain = pHostEntry->h_name;	// ȣ��Ʈ�� Domain Name�� ����
	*/

	/////////////////////////////////////////

	m_TotHostInfo = 0;
	memset(HostInfo, 0, sizeof(HostInfo));

	char buf[256] = {0,};
	gethostname (buf, 256);
	HOSTENT *p = gethostbyname(buf);

	struct in_addr *ptr;
	char **listptr = p->h_addr_list;

	switch(p->h_addrtype) {
	case AF_INET:
		while((ptr = (struct in_addr *) *listptr++) != NULL) {
			strcpy(HostInfo[m_TotHostInfo].szSockAddr, inet_ntoa(*ptr));
			m_TotHostInfo++;
			//AfxMessageBox(inet_ntoa(*ptr));
		}
	}

	//m_strSrvIP = HostInfo[m_PrimaryHostIndex].szSockAddr;
	//m_strDomain = p->h_name;
	strcpy(m_strDomain, p->h_name);

	return TRUE;
}

// ������ IP�� ����
char* CSockMan::GetServerIP()
{
	//return ((char*)m_strSrvIP.operator LPCTSTR());
	return HostInfo[m_PrimaryHostIndex].szSockAddr;
}

// ������ �������� ����
char* CSockMan::GetServerDomain()
{
	//return ((char*)m_strDomain.operator LPCTSTR());
	return m_strDomain;
}

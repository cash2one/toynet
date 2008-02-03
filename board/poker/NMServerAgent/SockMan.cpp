// SockMan.cpp

#include "stdafx.h"
#include "SockMan.h"
//(��ܿ� ��� �߰�)
#include "CommMsg.h"


CSockMan::CSockMan()
{
	nServerState=0;
	pServerSock = NULL;
	for(int i=0; i<MAX_DATA_SOCKET; i++)
	{
		pDataSock[i]=NULL;
	}

	m_nSPortNum = 0;

	AfxSocketInit();	// ����DLL �ʱ�ȭ(MFC������ �ڵ����� �������� API�󿡼��� ������ �𸣹Ƿ�)
}

CSockMan::~CSockMan()
{
	Destroy();
	WSACleanup();		// ����DLL Ŭ����
}

void CSockMan::Init(HWND hwnd) //, UINT serverport)
{
	Destroy();
	m_hWindow = hwnd;
	//m_nSPortNum = serverport;
	CheckHostInfo();	// Host ������ üũ�Ѵ�
}

// ������ ��� ���ϰ� Ŭ������ �ı�
void CSockMan::Destroy()
{
	if(pServerSock) { delete pServerSock; pServerSock=NULL; }
	for(int i=0; i<MAX_DATA_SOCKET; i++)
	{
		if(pDataSock[i] != NULL) { delete pDataSock[i]; pDataSock[i]=NULL; }
	}
	nServerState=0;
}

// Ŭ���� �ʱ�ȭ
BOOL CSockMan::CreateServerSocket(UINT port, char *lpszSockAddr)
{
	if(pServerSock) Destroy();
	pServerSock = new CServerSock();
	if(pServerSock==NULL){
		Error("���� Ŭ������ ���� �޸� �Ҵ� ����");
		return FALSE;
	}
	
	if(pServerSock->CreateSocket(m_hWindow, port, lpszSockAddr)==FALSE) return FALSE;
		//Error((char*)pServerSock->m_strError.operator LPCTSTR());

	// Send �����̸� ����(���۸� ���� �ʰ� ��ٷ� ������)
	//BOOL OptVal = TRUE;
	//pServerSock->SetSockOpt(TCP_NODELAY, &OptVal, sizeof(BOOL));

	nServerState = 1;						// ���Ӵ�������� ��Ÿ��
	return TRUE;
}

BOOL CSockMan::CloseServerSocket()
{
	if(pServerSock == NULL) return FALSE;
	delete pServerSock;
	pServerSock=NULL; 
	return TRUE;
}

BOOL CSockMan::PostCloseMsg(CDataSock* pSock)
{
	if(pSock==NULL)
	{
		TRACE("CSockMan::PostCloseMsg() - pSock == NULL\n");
		return FALSE;
	}

	int sid = pSock->hSID;
	return PostCloseMsg(sid);
}

BOOL CSockMan::PostCloseMsg(int sid)
{
	if(sid<0 || sid>=MAX_DATA_SOCKET)
	{
		TRACE("CSockMan::PostCloseMsg() - sid ���� �ʰ�(sid=%d)\n", sid);
		return FALSE;
	}
	if(pDataSock[sid] == NULL)
	{
		TRACE("CSockMan::PostCloseMsg() - pDataSock[sid] == NULL\n");
		return FALSE;
	}

	if(pDataSock[sid]->bPostCloseMsg == TRUE)
	{
		TRACE("CSockMan::PostCloseMsg() - �̹� ���� �ݱ� �޼����� ������(hSID=%d)\n", pDataSock[sid]->hSID);
		return FALSE;
	}

	PostMessage(m_hWindow, UM_DATA_CLOSE, NULL, (long)this); 
	pDataSock[sid]->bPostCloseMsg = TRUE;

	return TRUE;
}

void CSockMan::CheckHostInfo()
{
	// ȣ��Ʈ ���� ��ȸ
	char strHostName[256];
	char strPort[10];
	
	PHOSTENT pHostEntry;
	IN_ADDR inAddr;

	if(gethostname(strHostName, sizeof(strHostName)) != 0) 
		m_strError.Format("gethostname() error : %d", WSAGetLastError());
	else
	{
		pHostEntry = gethostbyname(strHostName);
		if(pHostEntry==NULL)
		{
			m_strError.Format("gethostbyname() error : %d", WSAGetLastError());
		}
		else memcpy(&inAddr, pHostEntry->h_addr,4);
	}

	//SOCKET m_hSrvSock;
	m_strSrvIP = inet_ntoa(inAddr);		// ȣ��Ʈ�� IP�� ���ڿ��� ��ȯ
	m_strDomain = pHostEntry->h_name;	// ȣ��Ʈ�� Domain Name�� ����
	// ::ltoa(m_nSPortNum, strPort, 10);	// ȣ��Ʈ�� ��Ʈ��ȣ�� ���ڿ��� ��ȯ
	m_strPort = strPort;				// ��ȯ�� ��Ʈ��ȣ�� ���ڿ��� ����
}

// ���� ��û�� �޾Ƶ��̰� ������ ������ �ʱ�ȭ��
int CSockMan::AcceptSocket()
{
	if(pServerSock == NULL) return -1;

	for(int i=0; i<MAX_DATA_SOCKET; i++)
	{
		if(pDataSock[i]==NULL)
		{
			pDataSock[i] = new CDataSock();
			if(pDataSock[i] == NULL) return -1;
			if(pServerSock->AcceptSocket(m_hWindow, pDataSock[i], i)==FALSE) // ������ ������ Accept�ϰ� �ʱ�ȭ �Ѵ�
			{
				delete pDataSock[i]; pDataSock[i]=NULL;
				return -1; 
			}
			return i;
		}
	}
	
	TRACE("CSockMan::AcceptSocket() - ���� ���� ���� �ʰ�\n");
	return -1;
}

// Ŭ���̾�Ʈ������ ������ ������ ����, ���ϴ� �ּҿ� �����Ų��
// -1�� �����ϸ� ���� ���и� �������� ���� ����� ���߿� �޼����� �뺸�ȴ�
// (���� ���� ���δ� UM_DATA_CONNECT Ȥ�� UM_DATA_CONNECT_FAILURE �޼����� �� �� �ִ�)
int CSockMan::ConnectSocket(char *strIP, UINT nPortNum)
{
	for(int i=0; i<MAX_DATA_SOCKET; i++)
	{
		if(pDataSock[i]==NULL)
		{
			pDataSock[i] = new CDataSock();
			if(pDataSock[i] == NULL) return -1;
			if(pDataSock[i]->ConnectSocket(m_hWindow, strIP, nPortNum, i)==FALSE)
			{
				AfxMessageBox(pDataSock[i]->m_strError);
				delete pDataSock[i]; pDataSock[i]=NULL;
				return -1; // Ŭ���̾�Ʈ������ �ʱ�ȭ
			}
			
			// Send �����̸� ����(���۸� ���� �ʰ� ��ٷ� ������)
			//BOOL OptVal = TRUE;
			//pDataSock[i]->SetSockOpt(TCP_NODELAY, &OptVal, sizeof(BOOL));
			return i;
		}
	}
	
	TRACE("CSockMan::ConnectSocket() - ���� ���� ���� �ʰ�\n");
	return -1;
}

CDataSock* CSockMan::GetDataSocketPtr(int sid)
{
	if(sid<0 || sid>=MAX_DATA_SOCKET) return NULL;
	return pDataSock[sid];
}

void CSockMan::SendData(CDataSock* pSock, void *pdata, int size)
{
	if(pSock==NULL)
	{
		TRACE("CSockMan::SendData() - pSock = NULL\n");
		return;
	}
	int sid = pSock->hSID;
	SendData(sid, pdata, size);
}

void CSockMan::SendData(int sid, void *pdata, int size)
{
	if(sid<0 || sid>=MAX_DATA_SOCKET) return;
	if(pDataSock[sid] == NULL) return;
	CDataSock *pSock = pDataSock[sid];
	

	pSock->SendData(pdata, size);
}

int CSockMan::ReceiveData(CDataSock* pSock)
{
	if(pSock==NULL) return FALSE;
	int sid = pSock->hSID;
	return ReceiveData(sid);
}

int CSockMan::ReceiveData(int sid)
{
	if(sid<0 || sid>=MAX_DATA_SOCKET) return 0;
	if(pDataSock[sid] == NULL) return 0;
	CDataSock *pSock = pDataSock[sid];
	int  rbyte = pSock->ReceiveData();
	return rbyte;	
}

BOOL CSockMan::CloseDataSocket(CDataSock* pSock)
{
	if(pSock==NULL) return FALSE;
	int sid = pSock->hSID;
	return CloseDataSocket(sid);
}

BOOL CSockMan::CloseDataSocket(int sid)
{
	if(sid<0 || sid>=MAX_DATA_SOCKET) return FALSE;
	if(pDataSock[sid] == NULL) return FALSE;
	delete pDataSock[sid];
	pDataSock[sid]=NULL; 
	return TRUE;
}


// ������ IP�� ����
char* CSockMan::GetServerIP()
{
	return ((char*)m_strSrvIP.operator LPCTSTR());
}

// ������ �������� ����
char* CSockMan::GetServerDomain()
{
	return ((char*)m_strDomain.operator LPCTSTR());
}

// ��Ʈ ��ȣ�� ���ڿ��� �Ѱ���
char* CSockMan::GetServerPortStr()
{
	return ((char*)m_strPort.operator LPCTSTR());
}

// ��Ʈ ��ȣ�� UINT������ �Ѱ���
UINT CSockMan::GetServerPortNum()
{
	return m_nSPortNum;
}

// ���� �޼��� ó�� 
void CSockMan::Error(char *strErr)
{
	AfxMessageBox(strErr);
}


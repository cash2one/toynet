// SockMan.cpp

#include "stdafx.h"
#include "SockMan.h"
//(상단에 요거 추가)
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

	AfxSocketInit();	// 윈속DLL 초기화(MFC에서는 자동으로 해주지만 API상에서도 쓰일지 모르므로)
}

CSockMan::~CSockMan()
{
	Destroy();
	WSACleanup();		// 윈속DLL 클린업
}

void CSockMan::Init(HWND hwnd) //, UINT serverport)
{
	Destroy();
	m_hWindow = hwnd;
	//m_nSPortNum = serverport;
	CheckHostInfo();	// Host 정보를 체크한다
}

// 생성된 모든 소켓과 클래스를 파괴
void CSockMan::Destroy()
{
	if(pServerSock) { delete pServerSock; pServerSock=NULL; }
	for(int i=0; i<MAX_DATA_SOCKET; i++)
	{
		if(pDataSock[i] != NULL) { delete pDataSock[i]; pDataSock[i]=NULL; }
	}
	nServerState=0;
}

// 클래스 초기화
BOOL CSockMan::CreateServerSocket(UINT port, char *lpszSockAddr)
{
	if(pServerSock) Destroy();
	pServerSock = new CServerSock();
	if(pServerSock==NULL){
		Error("소켓 클래스를 위한 메모리 할당 실패");
		return FALSE;
	}
	
	if(pServerSock->CreateSocket(m_hWindow, port, lpszSockAddr)==FALSE) return FALSE;
		//Error((char*)pServerSock->m_strError.operator LPCTSTR());

	// Send 딜레이를 없앰(버퍼링 하지 않고 곧바로 전송함)
	//BOOL OptVal = TRUE;
	//pServerSock->SetSockOpt(TCP_NODELAY, &OptVal, sizeof(BOOL));

	nServerState = 1;						// 접속대기중임을 나타냄
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
		TRACE("CSockMan::PostCloseMsg() - sid 범위 초과(sid=%d)\n", sid);
		return FALSE;
	}
	if(pDataSock[sid] == NULL)
	{
		TRACE("CSockMan::PostCloseMsg() - pDataSock[sid] == NULL\n");
		return FALSE;
	}

	if(pDataSock[sid]->bPostCloseMsg == TRUE)
	{
		TRACE("CSockMan::PostCloseMsg() - 이미 소켓 닫기 메세지를 보냈음(hSID=%d)\n", pDataSock[sid]->hSID);
		return FALSE;
	}

	PostMessage(m_hWindow, UM_DATA_CLOSE, NULL, (long)this); 
	pDataSock[sid]->bPostCloseMsg = TRUE;

	return TRUE;
}

void CSockMan::CheckHostInfo()
{
	// 호스트 정보 조회
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
	m_strSrvIP = inet_ntoa(inAddr);		// 호스트의 IP를 문자열로 변환
	m_strDomain = pHostEntry->h_name;	// 호스트의 Domain Name을 얻음
	// ::ltoa(m_nSPortNum, strPort, 10);	// 호스트의 포트번호를 문자열로 변환
	m_strPort = strPort;				// 변환된 포트번호를 문자열로 저장
}

// 연결 요청을 받아들이고 데이터 소켓을 초기화함
int CSockMan::AcceptSocket()
{
	if(pServerSock == NULL) return -1;

	for(int i=0; i<MAX_DATA_SOCKET; i++)
	{
		if(pDataSock[i]==NULL)
		{
			pDataSock[i] = new CDataSock();
			if(pDataSock[i] == NULL) return -1;
			if(pServerSock->AcceptSocket(m_hWindow, pDataSock[i], i)==FALSE) // 데이터 소켓을 Accept하고 초기화 한다
			{
				delete pDataSock[i]; pDataSock[i]=NULL;
				return -1; 
			}
			return i;
		}
	}
	
	TRACE("CSockMan::AcceptSocket() - 소켓 생성 개수 초과\n");
	return -1;
}

// 클라이언트용으로 데이터 소켓을 생성, 원하는 주소에 연결시킨다
// -1을 리턴하면 접속 실패를 뜻하지만 실제 결과는 나중에 메세지로 통보된다
// (접속 성패 여부는 UM_DATA_CONNECT 혹은 UM_DATA_CONNECT_FAILURE 메세지로 알 수 있다)
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
				return -1; // 클라이언트용으로 초기화
			}
			
			// Send 딜레이를 없앰(버퍼링 하지 않고 곧바로 전송함)
			//BOOL OptVal = TRUE;
			//pDataSock[i]->SetSockOpt(TCP_NODELAY, &OptVal, sizeof(BOOL));
			return i;
		}
	}
	
	TRACE("CSockMan::ConnectSocket() - 소켓 생성 개수 초과\n");
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


// 서버의 IP를 구함
char* CSockMan::GetServerIP()
{
	return ((char*)m_strSrvIP.operator LPCTSTR());
}

// 서버의 도메인을 구함
char* CSockMan::GetServerDomain()
{
	return ((char*)m_strDomain.operator LPCTSTR());
}

// 포트 번호를 문자열로 넘겨줌
char* CSockMan::GetServerPortStr()
{
	return ((char*)m_strPort.operator LPCTSTR());
}

// 포트 번호를 UINT형으로 넘겨줌
UINT CSockMan::GetServerPortNum()
{
	return m_nSPortNum;
}

// 에러 메세지 처리 
void CSockMan::Error(char *strErr)
{
	AfxMessageBox(strErr);
}


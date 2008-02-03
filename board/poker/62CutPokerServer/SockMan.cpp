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
		MessageBox(hwnd, "Winsock2 DLL(WS2_32.DLL)을 열 수 없습니다", "Winsock 에러", MB_OK);
		return FALSE;
	}
	
	// 윈속 초기화
	WSADATA wd = { 0 };
	int e = WSAStartup( MAKEWORD( 2, 2 ), &wd );
	SetLastError( e ); // for _err()

	if(e != 0) {
		MessageBox(hwnd, "Failed on start up winsock", "Winsock Error", MB_OK);
		return FALSE;
	}
	
	if(LOBYTE( wd.wVersion ) < 2) {
		MessageBox(hwnd, "Winsock의 버전이 낮습니다", "Winsock Error", MB_OK);
		return FALSE;
	}

	if(IocpThread.Create(workerthreadnum)==FALSE) 
		return FALSE;

	hIOCP = IocpThread.GetIocpHandle();
	m_hWindow = hwnd;

	for(int i=0; i<MAX_DATA_SOCKET; i++) {
		if(!pDataSock[i]) pDataSock[i] = new CMySocket(m_hWindow, hIOCP, SENDQUESIZE, RECVQUESIZE);
	}

	CheckHostInfo();	// Host 정보를 체크한다

	m_UsedHostNum = 0;
	m_PrimaryHostIndex = 0;
	memset(UsedHostInfo, 0, sizeof(UsedHostInfo));

	//if(primaryhost >= 0 && primaryhost < m_UsedHostNum) {
	if(primaryhost >= 0 && primaryhost < m_TotHostInfo) {
		m_PrimaryHostIndex = primaryhost;
	}
	else {
		m_PrimaryHostIndex = 0;
		MessageBox(hwnd, "지정된 HOST주소는 유효하지 않기 때문에 0번 호스트를 Primary로 강제 전환합니다.", "SockMan Initializing Warning", MB_OK);
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

// 생성된 모든 소켓과 클래스를 파괴
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
		WSACleanup();			// 윈속DLL 클린업
		FreeLibrary(hWs2Dll);	// Winsock2 DLL 해제
		hWs2Dll = NULL;
	}
}

// 클래스 초기화
BOOL CSockMan::CreateServerSocket(UINT port, char *lpszSockAddr)
{
	CloseServerSocket();

	pServerSock = new CMySocket(m_hWindow, hIOCP, SENDQUESIZE, RECVQUESIZE);

	if(pServerSock==NULL) {
		::MessageBox(m_hWindow, "Create failed at Server Socket", "Error", MB_OK);
		return FALSE;
	}
	
	// 소켓 생성
	if(pServerSock->Create()==FALSE) {
		::MessageBox(m_hWindow, "Create failed at Server Socket", "Error", MB_OK);
		return FALSE;
	}

	// 바인딩
	if(pServerSock->Bind(port, lpszSockAddr)==FALSE) {
		::MessageBox(m_hWindow, "Binding Error at Server Socket", "Error", MB_OK);
		return FALSE;
	}

	// 리스닝 쓰레드 시작
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

// 연결 요청을 받아들이고 데이터 소켓을 초기화함
int CSockMan::AcceptSocket(int *pHostIndex)
{
	if(pServerSock == NULL) {
		return -1;
	}

	for(int i=0; i<MAX_DATA_SOCKET; i++) {
		if(pDataSock[i] && !pDataSock[i]->m_bCreated) {
			// 데이터 소켓 배열 번호 설정
			pDataSock[i]->m_dwUserVal = i;
			// 데이터 소켓 유저 데이터 설정
			pDataSock[i]->m_pUserPtr = &UserVal[i];
			memset(&UserVal[i], 0, sizeof(USERVAL));
			UserVal[i].hSID = i;

			// 데이터 소켓을 Accept하고 초기화 한다
			if(pServerSock->Accept(pDataSock[i])==FALSE) {
			#ifdef _DEBUG
				//::MessageBox(NULL, pDataSock[i]->GetLastErrorString(), "억셉트 에러", MB_OK);
			#endif
				pDataSock[i]->Destroy();
				return -1; 
			}

			int hostindex = 0;

			if(m_UsedHostNum >= 2) {
				// Accept된 호스트의 IP어드레스를 검출
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

			// 엑셉트가 일어난 호스트 인덱스를 저장
			pDataSock[i]->m_iAcceptedHostIndex = hostindex;
			// 해당 호스트 억셉트 개수 정보 갱신
			UsedHostInfo[hostindex].AcceptedNum++;

			// 올바르게 생성된 경우 소켓 클래스 배열 번호를 리턴한다
			return i;
		}
	}
	
	// 수용할 공간이 없을 경우 일단 억셉트하고 소켓을 클로즈하여 리소스 낭비를 막음
	SOCKET s = accept(pServerSock->m_hSocket, NULL, NULL);
	if(s != INVALID_SOCKET) closesocket(s);

	//TRACE("CSockMan::AcceptSocket() - 소켓 생성 개수 초과\n");
	OutputDebugString("CSockMan::AcceptSocket() - 소켓 생성 개수 초과\n");
	return -1;
}

// 클라이언트용으로 데이터 소켓을 생성, 원하는 주소에 연결시킨다
// -1을 리턴하면 접속 실패를 뜻하지만 실제 결과는 나중에 메세지로 통보된다
int CSockMan::ConnectSocket(char *strIP, UINT nPortNum)
{
	for(int i=0; i<MAX_DATA_SOCKET; i++) {
		if(pDataSock[i] && !pDataSock[i]->m_bCreated) {		
			// 소켓 생성
			pDataSock[i]->Create();
			// 데이터 소켓 배열 번호 설정
			pDataSock[i]->m_dwUserVal = i;
			// 데이터 소켓 유저 데이터 설정
			pDataSock[i]->m_pUserPtr = &UserVal[i];
			memset(&UserVal[i], 0, sizeof(USERVAL));
			UserVal[i].hSID = i;

			if(pDataSock[i]->Connect(strIP, nPortNum)==FALSE) {
				pDataSock[i]->Destroy();
				return -1; // 클라이언트용으로 초기화
			}
			return i;
		}
	}
	
	//TRACE("CSockMan::ConnectSocket() - 소켓 생성 개수 초과\n");
	OutputDebugString("CSockMan::ConnectSocket() - 소켓 생성 개수 초과\n");
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
	// 호스트 정보 조회
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

	m_strSrvIP = inet_ntoa(inAddr);		// 호스트의 IP를 문자열로 변환
	m_strDomain = pHostEntry->h_name;	// 호스트의 Domain Name을 얻음
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

// 서버의 IP를 구함
char* CSockMan::GetServerIP()
{
	//return ((char*)m_strSrvIP.operator LPCTSTR());
	return HostInfo[m_PrimaryHostIndex].szSockAddr;
}

// 서버의 도메인을 구함
char* CSockMan::GetServerDomain()
{
	//return ((char*)m_strDomain.operator LPCTSTR());
	return m_strDomain;
}

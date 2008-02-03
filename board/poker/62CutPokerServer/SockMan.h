
///////////////////////////////////////////////////////////////////////////////////////
//
//     I/O completion port를 이용한 NT/2000 전용 소켓 클래스 매니저
//
//                               Powered by Future... 
//
///////////////////////////////////////////////////////////////////////////////////////

#ifndef __SOCKMAN_H__
#define __SOCKMAN_H__

#include "FirstInclude.h"
#include "MySocket.h"
#include "IocpThread.h"

#define	SENDQUESIZE	24000		// 소켓의 센드 버퍼 사이즈
#define	RECVQUESIZE	6000		// 소켓의 리시브 버퍼 사이즈
#define	MAX_HOST	5



typedef struct
{
	int hSID;
	int iUser1;
	int iUser2;
	int iUser3;
	int iTimeOutCnt;
	//(바로 아래 한줄만 추가)
	UINT SendPackCnt;		// 패킷 센드 카운터(패킷 스니핑 방지용)
	UINT RecvPackCnt;		// 패킷 리시브 카운터(패킷 스니핑 방지용)
	char ID[16];
} USERVAL;

typedef struct
{
	//BOOL bPrimary;
	char szSockAddr[16];
	int AcceptedNum;
} USEDHOST_INFO;

typedef struct
{
	char szSockAddr[16];
} HOST_INFO;

class CSockMan
{
public:
	HINSTANCE hWs2Dll;
	HANDLE hIOCP;
	HWND m_hWindow;

	CIocpThread IocpThread;

	// 검출된 모든 호스트 어드레스
	int				m_TotHostInfo;
	HOST_INFO       HostInfo[MAX_HOST];

	// 현재 사용중인 호스트 어드레스
	int m_UsedHostNum;
	int m_PrimaryHostIndex;
	USEDHOST_INFO UsedHostInfo[MAX_HOST];

	CMySocket *pServerSock;
	
	USERVAL UserVal[MAX_DATA_SOCKET];
	CMySocket *pDataSock[MAX_DATA_SOCKET];

	//CString m_strSrvIP;
	char m_strDomain[256];

	CSockMan();
	virtual ~CSockMan();

	// 소켓 매니저 클래스 초기화 함수
	BOOL Init(HWND hwnd, int workerthreadnum=0, BOOL bUseAllHost=FALSE, int primaryhostindex=0);
	// 소켓 매니저 클래스 파괴 함수
	void Destroy();

	// 서버(리스닝)소켓을 생성한다
	BOOL CreateServerSocket(UINT port, char *lpszSockAddr=NULL); 
	// 서버(리스닝)소켓을 닫는다
	BOOL CloseServerSocket();
	// Accept 이벤트에서 데이터 소켓을 억셉트하여 생성(성공시 클래스 핸들 반환, 실패시 0보다 작은값을 리턴)
	int AcceptSocket(int* pHostIndex = NULL);
	// Connect 이벤트에서 클라이언트용으로 데이터 소켓을 생성(성공시 클래스 핸들 반환, 실패시 0보다 작은값을 리턴)
	int ConnectSocket(char *strIP, UINT nPortNum);
	// 패킷을 전송한다
	BOOL SendData(CMySocket* pSock, void *pdata, int size);
	BOOL SendData(int sid, void *pdata, int size);
	// 소켓 닫기 예약
	BOOL ReservClose(int sid);
	// 소켓을 닫는다(Close이벤트에서 호출할것)
	BOOL CloseDataSocket(CMySocket* pSock);	// 데이터 소켓 클래스 포인터로 소켓 삭제
	BOOL CloseDataSocket(int sid);			// 소켓 클래스ID로 소켓 삭제

	// 데이터 소켓의 포인터를 얻음(NULL을 반환하면 생성되지 않은 소켓임)
	CMySocket* GetDataSocketPtr(int sid);

	// 호스트 정보를 체크한다
	BOOL CheckHostInfo();
	// 호스트 정보가 체크된 경우 호스트 정보를 얻는 함수
	char* GetServerIP();
	char* GetServerDomain();
};

#endif
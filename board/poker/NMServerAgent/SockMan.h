// SockMan.h

#ifndef __SOCKMAN_H__
#define __SOCKMAN_H__

#include "ServerSock.h"
#include "DataSock.h"

#define MAX_DATA_SOCKET 20

// 접속 정보 구조체
typedef struct
{
	long Port;
	long IPAdr;
} SOCKINFO;


class CSockMan
{
public:
	HWND m_hWindow;
	int nServerState;
	//SOCKINFO SInfo;

	CString m_strSrvIP, m_strDomain, m_strPort;	// 호스트 정보
	UINT m_nSPortNum;							// 서버 소켓용 호스트 포트 번호
	CString m_strError;							// 에러 출력용 스트링


	CServerSock *pServerSock;
	CDataSock *pDataSock[MAX_DATA_SOCKET];

	CSockMan();
	virtual ~CSockMan();

	void Init(HWND hwnd); //, UINT serverport);			// 클래스 초기화
	void Destroy();
	void CheckHostInfo();							// 호스트 정보를 체크한다

	//void SetServerPort(UINT nPort) {m_nSPortNum=nPort;}	// 서버용 소켓을 위한 포트번호를 세팅
	BOOL CreateServerSocket(UINT port, char *lpszSockAddr=NULL); // 서버 소켓을 생성한다
	BOOL CloseServerSocket();						// 서버 소켓을 닫는다
	int AcceptSocket();								// 서버용으로 데이터 소켓을 생성(성공시 클래스 핸들 반환, 실패시 0보다 작은값을 리턴)
	int ConnectSocket(char *strIP, UINT nPortNum);	// 클라이언트용으로 데이터 소켓을 생성(성공시 클래스 핸들 반환, 실패시 0보다 작은값을 리턴)

	CDataSock* GetDataSocketPtr(int sid);			// 데이터 소켓의 포인터를 얻음(NULL을 반환하면 생성되지 않은 소켓임)

	int ReceiveData(CDataSock* pSock);				// 소켓으로부터 데이터를 받는다(UM_DATA_RECIVE 메세지상에서 호출하면 된다)
	int ReceiveData(int sid);						// 소켓으로부터 데이터를 받는다

	void SendData(CDataSock* pSock, void *pdata, int size);
	void SendData(int sid, void *pdata, int size);

	BOOL PostCloseMsg(CDataSock* pSock);			// 소켓 닫기 메세지(UM_DATA_CLOSE)를 메세지 큐에 포스트한다
	BOOL PostCloseMsg(int sid);

	BOOL CloseDataSocket(CDataSock* pSock);		// 데이터 소켓 클래스 포인터로 소켓 삭제 - Overrided
	BOOL CloseDataSocket(int sid);				// 소켓 클래스ID로 소켓 삭제 - Overrided

	char* GetServerIP();
	char* GetServerDomain();
	char* GetServerPortStr();
	UINT GetServerPortNum();  // 실제 의미는 없다

	void Error(char *strErr);
};

#endif
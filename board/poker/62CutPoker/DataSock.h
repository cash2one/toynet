#if !defined(AFX_SERVERSOCK_H__65409B36_DE27_11D3_97A2_0050BF0FBE67__INCLUDED_)
#define AFX_SERVERSOCK_H__65409B36_DE27_11D3_97A2_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataSock.h : header file
//

#include "StreamBuf.h"

/*----------------------------------------------------------------------------------------
   <참고 사항>

	Winsock.h나 AfxSock.h에는 #define FD_SETSIZE 64로 정의되어 있다.
	따라서 더 많은 소켓을 생성하려면 위의 헤더를 인클루드하기전에
	#define FD_SETSIZE 500 과 같이 define해 주어야 한다.
	그러나 Win98에서는 자원 관리 문제로 80-90개 정도의 소켓만 생성 가능하다.
	(NT의 경우 더 많은 소켓을 생성 시킬 수 있음)
----------------------------------------------------------------------------------------*/
#define MAX_SOCK_SENDBUFFER 25000	// 최대 송신 버퍼
#define MAX_SOCK_RECVBUFFER 25000	// 최대 수신 버퍼


#define UM_DATA_RECEIVE (WM_USER+801)			// 자체 버퍼에 데이터가 수신되어 있음
#define UM_DATA_CLOSE (WM_USER+802)				// 소켓이 닫혀야할 상황임
#define UM_DATA_CONNECT (WM_USER+803)			// 연결 성공
#define UM_DATA_CONNECT_FAILURE (WM_USER+804)	// 연결 실패

/////////////////////////////////////////////////////////////////////////////
// CDataSock command target

class CDataSock : public CAsyncSocket
{
// Attributes
public:
	HWND hWND;			// 메시지를 보낼 윈도우 핸들
	int hSID;			// 소켓 클래스 아이디 (클래스 포인터 배열상에서 구분을 쉽게함)
	BOOL bIsSrvSock;    // 소켓이 서버용인가?(소켓의 용도가 서버용인지 클라이언트용인지 구분)
	BOOL bAvailable;	// 사용가능한 소켓인가? (Connect시 TRUE, Close되면서 FALSE로 세팅된다)
						//     소켓 삭제시 UM_DATA_CLOSE를 PostMessage()하는데...
						//     받을 데이터가 또 있을 경우 OnReceive()가 작동된다.
						//	   이렇게되면 UM_DATA_CLOSE가 두번 발생하게 되는데
						//	   이 플래그를 이용하여 유효하지 않은 소켓은 처리하지 않을 수 있다.

	BOOL bPostCloseMsg;	// 소켓 닫기 메세지를 이미 보냈는가?
	
	CString SockAddr;	// 연결 된후 자신의 IP 어드레스
	UINT SockPort;		// 연결 된후 자신의 Port번호

	// 사용자가 원하는 대로 쓸 수 있는 플래그
	int iUser1;		
	int iUser2;
	int iUser3;
	
	//(요것만 추가)
	UINT SendPackCnt;	// 패킷을 보낸 회수 카운터(패킷 스니핑 방지용)
	UINT RecvPackCnt;	// 패킷을 받은 회수 카운터(패킷 스니핑 방지용)

	CString m_strError;	// 에러 출력용 스트링
	
	CStreamBuf SendBuf;	// 데이터 전송용 스트림 버퍼
	CStreamBuf RecvBuf;	// 데이터 수신용 스트림 버퍼

// Operations
public:
	CDataSock();
	~CDataSock();
	
	BOOL ConnectSocket(HWND hwnd, char* pstrHostAddr, UINT nHostPort, int sid); // 클라이언트용 소켓으로 Connect한다
	void SendData(void *pdata, int size);	// 데이터를 보낸다
	int ReceiveData();						// 데이터를 받아서 내부 버퍼에 저장한다

	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnOutOfBandData(int nErrorCode);

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataSock)
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CDataSock)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERSOCK_H__65409B36_DE27_11D3_97A2_0050BF0FBE67__INCLUDED_)

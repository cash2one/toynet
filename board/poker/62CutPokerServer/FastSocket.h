///////////////////////////////////////////////////////////////////////////////////////
//
//     I/O completion port를 이용한 NT/2000 전용 소켓 클래스
//
//                               Powered by Future... 
//
///////////////////////////////////////////////////////////////////////////////////////

#ifndef __FASTSOCKET_H__
#define __FASTSOCKET_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winsock2.h>
#include "SendQue.h"
#include "RecvQue.h"

#define UM_FASTSOCKET_NOTIFY (WM_USER+100)
#define SEND_TIMEOUT_TICK 1000

// IOCP쓰레드 명령 플래그
#define IOCP_SOCKETCLOSE  -1	// 소켓 종료 플래그
#define IOCP_THREADCLOSE  -2	// 쓰레드 종료 플래그

// 오페레이션 타입
#define OPERATION_SEND  1		// 센드 작업
#define OPERATION_RECV  2		// 리시브 작업
#define OPERATION_CLOSE 3		// 클로즈 작업

// 사용자 정의 오버랩드 구조체
typedef struct
{
   OVERLAPPED	Overlapped;		// 오버랩드 구조체
   WSABUF		WsaBuf;			// WSA버퍼
   DWORD		OperationType;	// 오퍼레이션 타입
} MYOVERLAPPED;

////////////////// 크리티컬 섹션 락 클래스
class CMyLock
{
public:
	CRITICAL_SECTION m_CriticalSection;
	CMyLock() { InitializeCriticalSection(&m_CriticalSection); };
	~CMyLock() { DeleteCriticalSection(&m_CriticalSection); };
	inline void Lock() { EnterCriticalSection(&m_CriticalSection); };
	inline void Unlock() { LeaveCriticalSection(&m_CriticalSection); };
};

////////////////// 소켓 클래스
class CFastSocket  
{
public:
	HANDLE m_hIOCP;					// 입출력 완료포트 핸들
	SOCKET m_hSocket;				// 소켓 핸들
	HWND m_hMsgWnd;					// 메시지 윈도우 핸들
	BOOL m_bCreated;				// 소켓이 생성되었는가?
	BOOL m_bIsOnline;				// 소켓이 연결되었는가?
	BOOL m_bIsListeningSock;		// 리스닝 소켓인가?
	BOOL m_bUseForSend;				// 지금 센드에 사용중인가?
	BOOL m_bClosePosted;			// 소켓 종료가 포스팅되었는가?
	UINT m_nCloseTime;				// 소켓 종료가 포스팅된 타임Tick

	MYOVERLAPPED m_SendOvl;			// 센드용 오버랩 구조체
	MYOVERLAPPED m_RecvOvl;			// 리시브용 오버랩 구조체

	CSendQue m_SendQue;				// 센드 큐
	CRecvQue m_RecvQue;				// 리시브 큐

	DWORD m_dwUserVal;				// 사용자 변수
	void* m_pUserPtr;				// 사용자 포인터 변수
	static char m_strLastErr[256];	// 마지막 에러 스트링 출력용 정적 스트링 버퍼

	CRITICAL_SECTION m_CriticalSection;
	inline void Lock() { EnterCriticalSection(&m_CriticalSection); };
	inline void Unlock() { LeaveCriticalSection(&m_CriticalSection); };
	//inline void Lock() {};
	//inline void Unlock() {};

	// 소켓 생성자, 파괴자 (오버라이딩시 반드시 베이스 메서드를 호출해야함)
	CFastSocket(HANDLE hIocp, int SendQueSize, int RecvQueSize);
	virtual ~CFastSocket();

	BOOL Destroy();
	BOOL Create();
	BOOL Close();
	BOOL ReservClose();
	BOOL Bind(UINT nSocketPort, LPCTSTR lpszSocketAddress = NULL);
	BOOL Listen(int nConnectionBacklog = 5);
	BOOL Accept(CFastSocket *pNewSock, SOCKADDR* lpSockAddr = NULL, int* lpSockAddrLen = NULL);
	BOOL Connect(LPCTSTR lpszHostAddress, UINT nHostPort);
	BOOL AfterConnect(BOOL bIsConnected);
	BOOL BlockingConnect(LPCTSTR lpszHostAddress, UINT nHostPort);
	BOOL Attach(SOCKET hSocket, BOOL bAttachToIOCP=TRUE);
	BOOL Send(void *pData, int size);

	// 소켓 닫기 예약 함수
	BOOL PostClose();

	// 닫기가 예약된 소켓에 한하여 시간 초과인지 검사
	//BOOL IsCloseTimeOut();

	// 이벤트 발생시 수행되는 함수(오버라이드 가능) 
	// 오버라이드시 베이스 메서드를 호출하지 말것
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);

	// 피드백 함수들(저절로 호출되는 함수들임. 단,
	// 최초 연결시 리시브를 위해 ReceiveFeedback을 한번은 호출해야함)
	BOOL __fastcall SendFeedback(int TransByte);
	BOOL __fastcall ReceiveFeedback(int TransByte);
	BOOL CloseFeedback();

	// 메시지 윈도우 관련
	BOOL CreateMsgWindow(HWND hwndparent);
	BOOL DestroyMsgWindow();
	static LRESULT CALLBACK SockWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

	// 마지막 에러 스트링 얻기
	char* GetLastErrorString() {return m_strLastErr;}
};

#endif

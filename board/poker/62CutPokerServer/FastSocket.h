///////////////////////////////////////////////////////////////////////////////////////
//
//     I/O completion port�� �̿��� NT/2000 ���� ���� Ŭ����
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

// IOCP������ ��� �÷���
#define IOCP_SOCKETCLOSE  -1	// ���� ���� �÷���
#define IOCP_THREADCLOSE  -2	// ������ ���� �÷���

// ���䷹�̼� Ÿ��
#define OPERATION_SEND  1		// ���� �۾�
#define OPERATION_RECV  2		// ���ú� �۾�
#define OPERATION_CLOSE 3		// Ŭ���� �۾�

// ����� ���� �������� ����ü
typedef struct
{
   OVERLAPPED	Overlapped;		// �������� ����ü
   WSABUF		WsaBuf;			// WSA����
   DWORD		OperationType;	// ���۷��̼� Ÿ��
} MYOVERLAPPED;

////////////////// ũ��Ƽ�� ���� �� Ŭ����
class CMyLock
{
public:
	CRITICAL_SECTION m_CriticalSection;
	CMyLock() { InitializeCriticalSection(&m_CriticalSection); };
	~CMyLock() { DeleteCriticalSection(&m_CriticalSection); };
	inline void Lock() { EnterCriticalSection(&m_CriticalSection); };
	inline void Unlock() { LeaveCriticalSection(&m_CriticalSection); };
};

////////////////// ���� Ŭ����
class CFastSocket  
{
public:
	HANDLE m_hIOCP;					// ����� �Ϸ���Ʈ �ڵ�
	SOCKET m_hSocket;				// ���� �ڵ�
	HWND m_hMsgWnd;					// �޽��� ������ �ڵ�
	BOOL m_bCreated;				// ������ �����Ǿ��°�?
	BOOL m_bIsOnline;				// ������ ����Ǿ��°�?
	BOOL m_bIsListeningSock;		// ������ �����ΰ�?
	BOOL m_bUseForSend;				// ���� ���忡 ������ΰ�?
	BOOL m_bClosePosted;			// ���� ���ᰡ �����õǾ��°�?
	UINT m_nCloseTime;				// ���� ���ᰡ �����õ� Ÿ��Tick

	MYOVERLAPPED m_SendOvl;			// ����� ������ ����ü
	MYOVERLAPPED m_RecvOvl;			// ���ú�� ������ ����ü

	CSendQue m_SendQue;				// ���� ť
	CRecvQue m_RecvQue;				// ���ú� ť

	DWORD m_dwUserVal;				// ����� ����
	void* m_pUserPtr;				// ����� ������ ����
	static char m_strLastErr[256];	// ������ ���� ��Ʈ�� ��¿� ���� ��Ʈ�� ����

	CRITICAL_SECTION m_CriticalSection;
	inline void Lock() { EnterCriticalSection(&m_CriticalSection); };
	inline void Unlock() { LeaveCriticalSection(&m_CriticalSection); };
	//inline void Lock() {};
	//inline void Unlock() {};

	// ���� ������, �ı��� (�������̵��� �ݵ�� ���̽� �޼��带 ȣ���ؾ���)
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

	// ���� �ݱ� ���� �Լ�
	BOOL PostClose();

	// �ݱⰡ ����� ���Ͽ� ���Ͽ� �ð� �ʰ����� �˻�
	//BOOL IsCloseTimeOut();

	// �̺�Ʈ �߻��� ����Ǵ� �Լ�(�������̵� ����) 
	// �������̵�� ���̽� �޼��带 ȣ������ ����
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);

	// �ǵ�� �Լ���(������ ȣ��Ǵ� �Լ�����. ��,
	// ���� ����� ���ú긦 ���� ReceiveFeedback�� �ѹ��� ȣ���ؾ���)
	BOOL __fastcall SendFeedback(int TransByte);
	BOOL __fastcall ReceiveFeedback(int TransByte);
	BOOL CloseFeedback();

	// �޽��� ������ ����
	BOOL CreateMsgWindow(HWND hwndparent);
	BOOL DestroyMsgWindow();
	static LRESULT CALLBACK SockWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

	// ������ ���� ��Ʈ�� ���
	char* GetLastErrorString() {return m_strLastErr;}
};

#endif

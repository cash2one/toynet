
///////////////////////////////////////////////////////////////////////////////////////
//
//     I/O completion port�� �̿��� NT/2000 ���� ���� Ŭ���� �Ŵ���
//
//                               Powered by Future... 
//
///////////////////////////////////////////////////////////////////////////////////////

#ifndef __SOCKMAN_H__
#define __SOCKMAN_H__

#include "FirstInclude.h"
#include "MySocket.h"
#include "IocpThread.h"

#define	SENDQUESIZE	24000		// ������ ���� ���� ������
#define	RECVQUESIZE	6000		// ������ ���ú� ���� ������
#define	MAX_HOST	5



typedef struct
{
	int hSID;
	int iUser1;
	int iUser2;
	int iUser3;
	int iTimeOutCnt;
	//(�ٷ� �Ʒ� ���ٸ� �߰�)
	UINT SendPackCnt;		// ��Ŷ ���� ī����(��Ŷ ������ ������)
	UINT RecvPackCnt;		// ��Ŷ ���ú� ī����(��Ŷ ������ ������)
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

	// ����� ��� ȣ��Ʈ ��巹��
	int				m_TotHostInfo;
	HOST_INFO       HostInfo[MAX_HOST];

	// ���� ������� ȣ��Ʈ ��巹��
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

	// ���� �Ŵ��� Ŭ���� �ʱ�ȭ �Լ�
	BOOL Init(HWND hwnd, int workerthreadnum=0, BOOL bUseAllHost=FALSE, int primaryhostindex=0);
	// ���� �Ŵ��� Ŭ���� �ı� �Լ�
	void Destroy();

	// ����(������)������ �����Ѵ�
	BOOL CreateServerSocket(UINT port, char *lpszSockAddr=NULL); 
	// ����(������)������ �ݴ´�
	BOOL CloseServerSocket();
	// Accept �̺�Ʈ���� ������ ������ ���Ʈ�Ͽ� ����(������ Ŭ���� �ڵ� ��ȯ, ���н� 0���� �������� ����)
	int AcceptSocket(int* pHostIndex = NULL);
	// Connect �̺�Ʈ���� Ŭ���̾�Ʈ������ ������ ������ ����(������ Ŭ���� �ڵ� ��ȯ, ���н� 0���� �������� ����)
	int ConnectSocket(char *strIP, UINT nPortNum);
	// ��Ŷ�� �����Ѵ�
	BOOL SendData(CMySocket* pSock, void *pdata, int size);
	BOOL SendData(int sid, void *pdata, int size);
	// ���� �ݱ� ����
	BOOL ReservClose(int sid);
	// ������ �ݴ´�(Close�̺�Ʈ���� ȣ���Ұ�)
	BOOL CloseDataSocket(CMySocket* pSock);	// ������ ���� Ŭ���� �����ͷ� ���� ����
	BOOL CloseDataSocket(int sid);			// ���� Ŭ����ID�� ���� ����

	// ������ ������ �����͸� ����(NULL�� ��ȯ�ϸ� �������� ���� ������)
	CMySocket* GetDataSocketPtr(int sid);

	// ȣ��Ʈ ������ üũ�Ѵ�
	BOOL CheckHostInfo();
	// ȣ��Ʈ ������ üũ�� ��� ȣ��Ʈ ������ ��� �Լ�
	char* GetServerIP();
	char* GetServerDomain();
};

#endif
// SockMan.h

#ifndef __SOCKMAN_H__
#define __SOCKMAN_H__

#include "ServerSock.h"
#include "DataSock.h"

#define MAX_DATA_SOCKET 20

// ���� ���� ����ü
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

	CString m_strSrvIP, m_strDomain, m_strPort;	// ȣ��Ʈ ����
	UINT m_nSPortNum;							// ���� ���Ͽ� ȣ��Ʈ ��Ʈ ��ȣ
	CString m_strError;							// ���� ��¿� ��Ʈ��


	CServerSock *pServerSock;
	CDataSock *pDataSock[MAX_DATA_SOCKET];

	CSockMan();
	virtual ~CSockMan();

	void Init(HWND hwnd); //, UINT serverport);			// Ŭ���� �ʱ�ȭ
	void Destroy();
	void CheckHostInfo();							// ȣ��Ʈ ������ üũ�Ѵ�

	//void SetServerPort(UINT nPort) {m_nSPortNum=nPort;}	// ������ ������ ���� ��Ʈ��ȣ�� ����
	BOOL CreateServerSocket(UINT port, char *lpszSockAddr=NULL); // ���� ������ �����Ѵ�
	BOOL CloseServerSocket();						// ���� ������ �ݴ´�
	int AcceptSocket();								// ���������� ������ ������ ����(������ Ŭ���� �ڵ� ��ȯ, ���н� 0���� �������� ����)
	int ConnectSocket(char *strIP, UINT nPortNum);	// Ŭ���̾�Ʈ������ ������ ������ ����(������ Ŭ���� �ڵ� ��ȯ, ���н� 0���� �������� ����)

	CDataSock* GetDataSocketPtr(int sid);			// ������ ������ �����͸� ����(NULL�� ��ȯ�ϸ� �������� ���� ������)

	int ReceiveData(CDataSock* pSock);				// �������κ��� �����͸� �޴´�(UM_DATA_RECIVE �޼����󿡼� ȣ���ϸ� �ȴ�)
	int ReceiveData(int sid);						// �������κ��� �����͸� �޴´�

	void SendData(CDataSock* pSock, void *pdata, int size);
	void SendData(int sid, void *pdata, int size);

	BOOL PostCloseMsg(CDataSock* pSock);			// ���� �ݱ� �޼���(UM_DATA_CLOSE)�� �޼��� ť�� ����Ʈ�Ѵ�
	BOOL PostCloseMsg(int sid);

	BOOL CloseDataSocket(CDataSock* pSock);		// ������ ���� Ŭ���� �����ͷ� ���� ���� - Overrided
	BOOL CloseDataSocket(int sid);				// ���� Ŭ����ID�� ���� ���� - Overrided

	char* GetServerIP();
	char* GetServerDomain();
	char* GetServerPortStr();
	UINT GetServerPortNum();  // ���� �ǹ̴� ����

	void Error(char *strErr);
};

#endif
#if !defined(AFX_SERVERSOCK_H__65409B36_DE27_11D3_97A2_0050BF0FBE67__INCLUDED_)
#define AFX_SERVERSOCK_H__65409B36_DE27_11D3_97A2_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataSock.h : header file
//

#include "StreamBuf.h"

/*----------------------------------------------------------------------------------------
   <���� ����>

	Winsock.h�� AfxSock.h���� #define FD_SETSIZE 64�� ���ǵǾ� �ִ�.
	���� �� ���� ������ �����Ϸ��� ���� ����� ��Ŭ����ϱ�����
	#define FD_SETSIZE 500 �� ���� define�� �־�� �Ѵ�.
	�׷��� Win98������ �ڿ� ���� ������ 80-90�� ������ ���ϸ� ���� �����ϴ�.
	(NT�� ��� �� ���� ������ ���� ��ų �� ����)
----------------------------------------------------------------------------------------*/
#define MAX_SOCK_SENDBUFFER 25000	// �ִ� �۽� ����
#define MAX_SOCK_RECVBUFFER 25000	// �ִ� ���� ����


#define UM_DATA_RECEIVE (WM_USER+801)			// ��ü ���ۿ� �����Ͱ� ���ŵǾ� ����
#define UM_DATA_CLOSE (WM_USER+802)				// ������ �������� ��Ȳ��
#define UM_DATA_CONNECT (WM_USER+803)			// ���� ����
#define UM_DATA_CONNECT_FAILURE (WM_USER+804)	// ���� ����

/////////////////////////////////////////////////////////////////////////////
// CDataSock command target

class CDataSock : public CAsyncSocket
{
// Attributes
public:
	HWND hWND;			// �޽����� ���� ������ �ڵ�
	int hSID;			// ���� Ŭ���� ���̵� (Ŭ���� ������ �迭�󿡼� ������ ������)
	BOOL bIsSrvSock;    // ������ �������ΰ�?(������ �뵵�� ���������� Ŭ���̾�Ʈ������ ����)
	BOOL bAvailable;	// ��밡���� �����ΰ�? (Connect�� TRUE, Close�Ǹ鼭 FALSE�� ���õȴ�)
						//     ���� ������ UM_DATA_CLOSE�� PostMessage()�ϴµ�...
						//     ���� �����Ͱ� �� ���� ��� OnReceive()�� �۵��ȴ�.
						//	   �̷��ԵǸ� UM_DATA_CLOSE�� �ι� �߻��ϰ� �Ǵµ�
						//	   �� �÷��׸� �̿��Ͽ� ��ȿ���� ���� ������ ó������ ���� �� �ִ�.

	BOOL bPostCloseMsg;	// ���� �ݱ� �޼����� �̹� ���´°�?
	
	CString SockAddr;	// ���� ���� �ڽ��� IP ��巹��
	UINT SockPort;		// ���� ���� �ڽ��� Port��ȣ

	// ����ڰ� ���ϴ� ��� �� �� �ִ� �÷���
	int iUser1;		
	int iUser2;
	int iUser3;
	
	//(��͸� �߰�)
	UINT SendPackCnt;	// ��Ŷ�� ���� ȸ�� ī����(��Ŷ ������ ������)
	UINT RecvPackCnt;	// ��Ŷ�� ���� ȸ�� ī����(��Ŷ ������ ������)

	CString m_strError;	// ���� ��¿� ��Ʈ��
	
	CStreamBuf SendBuf;	// ������ ���ۿ� ��Ʈ�� ����
	CStreamBuf RecvBuf;	// ������ ���ſ� ��Ʈ�� ����

// Operations
public:
	CDataSock();
	~CDataSock();
	
	BOOL ConnectSocket(HWND hwnd, char* pstrHostAddr, UINT nHostPort, int sid); // Ŭ���̾�Ʈ�� �������� Connect�Ѵ�
	void SendData(void *pdata, int size);	// �����͸� ������
	int ReceiveData();						// �����͸� �޾Ƽ� ���� ���ۿ� �����Ѵ�

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

// DataSock.cpp : implementation file
//

#include "stdafx.h"
#include "DataSock.h"

#include "CommMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_PACKETPOOL 30

class CPacketPool
{
public:
	int  m_Index;
	char m_Pool[MAX_PACKETPOOL][50*1024];

	CPacketPool()
	{
		m_Index = 0;
		memset(m_Pool, 0, sizeof(m_Pool));
	}

	char* GetNext()
	{
		char* prtn = m_Pool[m_Index];
		m_Index++;
		if(m_Index >= MAX_PACKETPOOL)
			m_Index = 0;
		return prtn;
	}
};

// ���� ���� ��Ŷ Ǯ
CPacketPool g_sSendPool;
/////////////////////////////////////////////////////////////////////////////
// CDataSock

CDataSock::CDataSock()
{
	hSID = 0;
	hWND = NULL;
	bAvailable = FALSE;
	iUser1 = iUser2 = iUser3 = 0;

	//(�Ʒ� ���� �߰�)
	SendPackCnt = 0; 	// ��Ŷ ������ ���� ī���� �ʱ�ȭ
	RecvPackCnt = 0;	// ��Ŷ ������ ���� ī���� �ʱ�ȭ

	bPostCloseMsg = FALSE;

	SendBuf.Init(MAX_SOCK_SENDBUFFER);
	RecvBuf.Init(MAX_SOCK_RECVBUFFER);
}

CDataSock::~CDataSock()
{
	hSID = 0;
	hWND = NULL;
	bAvailable = FALSE;
	iUser1 = iUser2 = iUser3 = 0;
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CDataSock, CAsyncSocket)
	//{{AFX_MSG_MAP(CDataSock)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CDataSock member functions


/*======================================================================================
	Ŭ���̾�Ʈ�� ������ �������� �ʱ�ȭ(Connect��û�Ѵ�)

	��û�� �����ߴ��� ���� ���� ���δ� OnConnect()�� �ڵ����� ȣ��Ǹ鼭
	�ش� ������� ���� ����� �޼����� �̿��Ͽ� ������ �ȴ�.
	(�񵿱� ������ Ư���� ����� ���߿� �߻��Ѵ�)
	���ῡ ������ ������ UM_DATA_CONNECT_FAILURE �޼��� ó������ �����ؾ��Ѵ�.
	(Ŭ������ delete�ϸ� �ڵ����� Close()�� ȣ��ȴ�)
	PostMessage()�� ����ϴ� ������ �Լ�ó���� Ŭ���� �����Ͱ� �����Ǹ� �ȵǱ� ������
======================================================================================*/
BOOL CDataSock::ConnectSocket(HWND hwnd, char* pstrHostAddr, UINT nHostPort, int sid)
{
	if(hWND) Close();

	hSID = sid;				// ���� Ŭ���� ���̵�
	bIsSrvSock = FALSE;		// Ŭ���̾�Ʈ�� �������� �˸� 
	hWND = hwnd;			// �޼����� ���� ������ �ڵ�
	bPostCloseMsg = FALSE;	// ���� �ݱ� �޽��� ���� �÷��� �ʱ�ȭ
	bAvailable = FALSE;		// ��ȿ���� ���� �������� ��Ÿ��
	//(��� ���� �߰�)
	SendPackCnt = 0;		// ��Ŷ ��Ű�� ���� ī���� �ʱ�ȭ
	RecvPackCnt = 0;		// ��Ŷ ������ ���� ī���� �ʱ�ȭ

	if(SendBuf.GetFreeSize() == 0 || RecvBuf.GetFreeSize() == 0)
	{
		m_strError.Format("CDataSock::Connect() error : Can't Create Class (Socket ID=%d)", sid);
		PostMessage(hWND, UM_DATA_CONNECT_FAILURE, hSID, (long)this); // ���� ���� �޼����� ����
		return FALSE;
	}

	if(Create()==0) // ���� ���� ����
	{
		m_strError.Format("CDataSock::Create() error : %d (Socket ID=%d)", GetLastError(), sid);
		PostMessage(hWND, UM_DATA_CONNECT_FAILURE, hSID, (long)this); // ���� ���� �޼����� ����
		return FALSE;
	}

	if(Connect(pstrHostAddr, nHostPort) == 0) // Ȯ���� ���ؼ� ���� ���δ� �ڵ����� ȣ��Ǵ� OnConnect()�� Ȯ��
	{	
		int nECode = GetLastError();
		if(nECode  != WSAEWOULDBLOCK) // ��ŷ�� �ƴ� ������ ���� ��� ���� ���� �޼����� ����
		{
			m_strError.Format("CDataSock::Connect() error : %d (Socket ID=%d)", nECode, sid);
			PostMessage(hWND, UM_DATA_CONNECT_FAILURE, hSID, (long)this); // ���� ���� �޼����� ����
			return FALSE;
		}
	}

	bAvailable = TRUE;	// ��ȿ�� �������� ��Ÿ��
	
	return TRUE;
}

// �����͸� ����
// (������ �� �����͸� ���ۿ� ����ϰ� OnSend()�� ȣ��)
void CDataSock::SendData(void *pdata, int size)
{
if(bAvailable == FALSE) return;

	if(size<=0)
	{
		TRACE("CDataSock::SendData() - ������ ����� 0�̰ų� ������(sid=%d, AddSize=%d)\n", hSID, size);
		return;
	}

	// �ӽ� ���ۿ� �����͸� ����(�� �ӽù��۴� ��Ƽ ������κ��� ������ ���������� �ʴ� - �ӵ��� ���Ͽ� ��� ����)
	char* pbuf = g_sSendPool.GetNext();
	memcpy(pbuf, pdata, size);
	
	// ��Ŷ ��ȣ�� ���� ����(��Ŷ ������ üũ��)
	PACKETHEADER* phead = (PACKETHEADER*)pbuf;
	SendPackCnt++;
	phead->PackCnt = SendPackCnt;
	// ��Ŷ ��ȣȭ
	CCommMsg::EncodeAll((char *)pbuf);

	if(SendBuf.AddData(pbuf, size) == 0)
	{
		TRACE("Send Buffer�� �������� ���� ����(sid=%d)\n", hSID);
		bAvailable = FALSE;	// ��ȿ���� ���� �������� ��Ÿ��
		
		if(bPostCloseMsg==FALSE)
		{
			bPostCloseMsg=TRUE;
			PostMessage(hWND, UM_DATA_CLOSE, hSID, (long)this); 
		}
		return;
	}
	OnSend(0);
}

int CDataSock::ReceiveData()
{
	if(bAvailable == FALSE) return 0;  // ��ȿ�� ������ �ƴϸ� ����

	int nECode;

	/*
	int totbyte=0;
	if(IOCtl(FIONREAD, &totbyte)==0) // �ѹ��� ������ �ִ� �� ����Ʈ ���� ���Ѵ�
	{
		nECode = GetLastError();
		m_strError.Format("CDataSock::ReceiveData()-IOCtl() error : %d (Socket ID=%d)", nECode, sid);
		PostMessage(hWND, UM_DATA_RECEIVE_FAILURE, NULL, (long)this); // ���� ���� �޼����� ����
		//CAsyncSocket::OnReceive(nErrorCode);
		return;
	}
	*/

	// ��ȣȭ ���ڵ� ������ġ ���
	int decodestart = RecvBuf.GetUsedSize(); 

	// ������ ���� ����� 0�̸� rbyte=0 �̵ȴ�
	// ���� �޾ƾ��� �����Ͱ� ���� ���� ������� ũ�ٸ� ���ʺ��� �߷��� ���ۿ� ����.
	int bsize = RecvBuf.GetFreeSize();					// ������ ���� ����� ����
	int rbyte = Receive(RecvBuf.GetEndPointer(), bsize);	// ���ۿ� �����͸� ����

	// ���� : UM_DATA_CLOSE �޼����� �ι� �߻��Ѵ�.
	// ���۰� �۾Ƽ� Receive()�� �ѹ��� ������ �ʰ� �ٷ� �ڸ� �̾�
	// OnReceive()�� �Ǵٽ� ȣ��Ǳ� �����̴�. PostMessage()���ڸ��� �ٷ� �߻��Ѵ�
	// �׷��� ���� ���ᰡ �ι� �Ͼ�Ƿ� �̸� �������� bAvailable �÷��׸� �ξ���.

	if(rbyte == SOCKET_ERROR)
	{
		nECode = GetLastError();
		if(nECode==WSAEWOULDBLOCK) // ��ŷ ���̸� ������ OnReceive()���� �ٽ� �޴´�
		{
			return 0;
		}
		else // ��ŷ�� �ƴ϶� ���� �������
		{
			// ���� ���� �޼����� ����
			TRACE("CDataSock::ReceiveData() - ���� ���� �߻�\n");
			bAvailable = FALSE;	// ��ȿ���� ���� �������� ��Ÿ��
			if(bPostCloseMsg==FALSE)
			{
				bPostCloseMsg=TRUE;
				PostMessage(hWND, UM_DATA_CLOSE, hSID, (long)this); 
			}
			return 0;
		}
	}
	else if(rbyte==0) // ������ ������ ���¶��(Ȥ�� ������ ���� ����� 0�� ���)
	{
		// ���� ���� �޼����� ����
		TRACE("CDataSock::ReceiveData() - ���� ������ (���� ���ۻ�����=%d, ���� ������ ������=%d)\n", bsize, rbyte);
		bAvailable = FALSE;	// ��ȿ���� ���� �������� ��Ÿ��
		if(bPostCloseMsg==FALSE)
		{
			bPostCloseMsg=TRUE;
			PostMessage(hWND, UM_DATA_CLOSE, hSID, (long)this); 
		}
		return 0;
	}
	else // ���������� �����͸� �޾Ҵٸ�
	{
		if(RecvBuf.AddUsedSize(rbyte)==FALSE) // ���۰� ��ģ ����� ���� ���� ��Ų��.
		{
			// ���� ���� �޼����� ����
			TRACE("CDataSock::ReceiveData() - ���ú� ���� ��ħ\n");
			bAvailable = FALSE;	// ��ȿ���� ���� �������� ��Ÿ��
			if(bPostCloseMsg==FALSE)
			{
				bPostCloseMsg=TRUE;
				PostMessage(hWND, UM_DATA_CLOSE, hSID, (long)this); 
			}
			return 0;
		}
		else
		{
			// ��ȣȭ�� ��Ŷ�� ���ڵ��Ѵ�
			//RecvBuf.Data_Decode(decodestart, rbyte);
			return rbyte;
		}
	}
}

// ���Ͽ� �о���� ����Ÿ�� �����Ҷ� �߻�
// (���� �����͸� ���ۿ� �����ϰ� UM_DATA_RECEIVE �޼����� �����Ͽ� �����Ͱ� ������ �˸�)
void CDataSock::OnReceive(int nErrorCode)
{
	// ��ȿ�� ������ �ƴϸ� ����
	if(bAvailable == FALSE)
	{
		CAsyncSocket::OnReceive(nErrorCode);
		return;
	}

	if(nErrorCode == WSAENETDOWN) // ��Ʈ��ũ ����
	{
		TRACE("CDataSock::OnReceive() - ��Ʈ��ũ ���� �߻�\n");
		bAvailable = FALSE;	// ��ȿ���� ���� �������� ��Ÿ��
		if(bPostCloseMsg==FALSE)
		{
			bPostCloseMsg=TRUE;
			PostMessage(hWND, UM_DATA_CLOSE, hSID, (long)this); 
		}
		CAsyncSocket::OnReceive(nErrorCode);
		return;
	}
	
	SendMessage(hWND, UM_DATA_RECEIVE, hSID, (long)this); 
	// Post�Ұ�� ���ۿ� �����Ͱ� �����Ǳ� ����. (���۰� ���鼭 ���� �����)
	// Send�� ���� �����Ͱ� ó���Ǿ�߸� �Ѿ�Ƿ� ���۰� �������� �ʴ´�.
			
	CAsyncSocket::OnReceive(nErrorCode);
}

// ���Ͽ� ����Ÿ Send()�� �����Ҷ� �߻�
// ���� ����� �ѹ� �߻��ϰ� ��ŷ�� ���尡 �ٽ� ���������� �� �߻���
void CDataSock::OnSend(int nErrorCode)
{
	// ��ȿ�� ������ �ƴϸ� ����
	if(bAvailable == FALSE)
	{
		CAsyncSocket::OnSend(nErrorCode);
		return;
	}

	if(nErrorCode == WSAENETDOWN) // ��Ʈ��ũ ����
	{
		bAvailable = FALSE;	// ��ȿ���� ���� �������� ��Ÿ��
		if(bPostCloseMsg==FALSE)
		{
			bPostCloseMsg=TRUE;
			PostMessage(hWND, UM_DATA_CLOSE, hSID, (long)this); 
		}
		CAsyncSocket::OnSend(nErrorCode);
		return;
	}
	
	// ���ۿ��� ���� �����Ͱ� ������ ����
	if(SendBuf.GetUsedSize()==0)
	{
		CAsyncSocket::OnSend(nErrorCode);
		return;
	}

	int sbyte = Send(SendBuf.GetStartPointer(), SendBuf.GetUsedSize());
	if(sbyte == SOCKET_ERROR)
	{
		int nECode = GetLastError();
		if(nECode==WSAEWOULDBLOCK) // ��ŷ ���̸� ������ OnSend()���� �ٽ� ������
		{
			return;
		}
		else // ��ŷ�� �ƴ϶� ���� �������
		{
			// ���� ���� �޼����� ����
			bAvailable = FALSE;	// ��ȿ���� ���� �������� ��Ÿ��
			if(bPostCloseMsg==FALSE)
			{
				bPostCloseMsg=TRUE;
				PostMessage(hWND, UM_DATA_CLOSE, hSID, (long)this); 
			}
			CAsyncSocket::OnSend(nErrorCode);
			return;
		}

	}
	else // ����� ������ �Ǿ��ٸ� (sbyte > 1) �ϰ��̴�.
	{
		SendBuf.DelData(sbyte);
	}

	//SendMessage(hWND, UM_DATA_????, hSID, (long)this);

	CAsyncSocket::OnSend(nErrorCode);
}


// ����� ������ ������ �߻�
void CDataSock::OnClose(int nErrorCode)
{
	bAvailable = FALSE;	// ��ȿ���� ���� �������� ��Ÿ��
	if(bPostCloseMsg==FALSE)
	{
		bPostCloseMsg=TRUE;
		PostMessage(hWND, UM_DATA_CLOSE, hSID, (long)this); 
	}
	CAsyncSocket::OnClose(nErrorCode);
}

/*======================================================================================
 Connect ��û�� ���� ����� �߻�(������ ���� ������ ���� �ִ�)

	(�񵿱� ���Ͽ����� ���ؼ� ����� ���߿� �߻��Ѵ�-��û ���� �ٷ� �̷������ ���� �ƴ�)
	������ �����ϸ� ��õ� �ϴ��� Ŭ������ �����ؾ��Ѵ�.
	PostMessage()�� ����ϴ� ������ �Լ�ó���� Ŭ���� �����Ͱ� �����Ǹ� �ȵǱ� ������
======================================================================================*/
void CDataSock::OnConnect(int nErrorCode)
{
	if(nErrorCode != 0) 
	{	
		m_strError.Format("CDataSock::OnConnect() error : %d (Socket ID=%d)", nErrorCode, hSID);
		PostMessage(hWND, UM_DATA_CONNECT_FAILURE, hSID, (long)this); // ���� ���� �޼����� ����
		CAsyncSocket::OnConnect(nErrorCode);
		return; 
	}
	GetSockName(SockAddr, SockPort); // ����� �ּҿ� ���� ��ȣ�� ���Ѵ�
	SendMessage(hWND, UM_DATA_CONNECT, hSID, (long)this); // ���� ���� �޼����� ����
	CAsyncSocket::OnConnect(nErrorCode);
}

// Out of Band
void CDataSock::OnOutOfBandData(int nErrorCode)
{
	if(nErrorCode != WSAENETDOWN) return;

	TRACE("��Ʈ��ũ ���� - WSAENETDOWN : OUT OF BAND (hSID=%d)\n", hSID);

	bAvailable = FALSE;	// ��ȿ���� ���� �������� ��Ÿ��
	if(bPostCloseMsg==FALSE)
	{
		bPostCloseMsg=TRUE;
		PostMessage(hWND, UM_DATA_CLOSE, hSID, (long)this); 
	}
	CAsyncSocket::OnOutOfBandData(nErrorCode);
}

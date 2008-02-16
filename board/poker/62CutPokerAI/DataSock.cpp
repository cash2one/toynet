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

// 전역 센드 패킷 풀
CPacketPool g_sSendPool;
/////////////////////////////////////////////////////////////////////////////
// CDataSock

CDataSock::CDataSock()
{
	hSID = 0;
	hWND = NULL;
	bAvailable = FALSE;
	iUser1 = iUser2 = iUser3 = 0;

	//(아래 한줄 추가)
	SendPackCnt = 0; 	// 패킷 스니핑 방지 카운터 초기화
	RecvPackCnt = 0;	// 패킷 스니핑 방지 카운터 초기화

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
	클라이언트용 데이터 소켓으로 초기화(Connect요청한다)

	요청에 성공했더라도 실제 접속 여부는 OnConnect()가 자동으로 호출되면서
	해당 윈도우로 접속 결과를 메세지를 이용하여 보내게 된다.
	(비동기 소켓의 특성상 결과는 나중에 발생한다)
	연결에 실패한 소켓은 UM_DATA_CONNECT_FAILURE 메세지 처리에서 삭제해야한다.
	(클래스를 delete하면 자동으로 Close()가 호출된다)
	PostMessage()를 사용하는 이유는 함수처리중 클래스 포인터가 삭제되면 안되기 때문임
======================================================================================*/
BOOL CDataSock::ConnectSocket(HWND hwnd, char* pstrHostAddr, UINT nHostPort, int sid)
{
	if(hWND) Close();

	hSID = sid;				// 소켓 클래스 아이디
	bIsSrvSock = FALSE;		// 클라이언트용 소켓임을 알림 
	hWND = hwnd;			// 메세지를 보낼 윈도우 핸들
	bPostCloseMsg = FALSE;	// 소켓 닫기 메시지 보냄 플래그 초기화
	bAvailable = FALSE;		// 유효하지 못한 소켓임을 나타냄
	//(요거 한줄 추가)
	SendPackCnt = 0;		// 패킷 스키핑 방지 카운터 초기화
	RecvPackCnt = 0;		// 패킷 스니핑 방지 카운터 초기화

	if(SendBuf.GetFreeSize() == 0 || RecvBuf.GetFreeSize() == 0)
	{
		m_strError.Format("CDataSock::Connect() error : Can't Create Class (Socket ID=%d)", sid);
		PostMessage(hWND, UM_DATA_CONNECT_FAILURE, hSID, (long)this); // 연결 실패 메세지를 보냄
		return FALSE;
	}

	if(Create()==0) // 소켓 생성 실패
	{
		m_strError.Format("CDataSock::Create() error : %d (Socket ID=%d)", GetLastError(), sid);
		PostMessage(hWND, UM_DATA_CONNECT_FAILURE, hSID, (long)this); // 연결 실패 메세지를 보냄
		return FALSE;
	}

	if(Connect(pstrHostAddr, nHostPort) == 0) // 확실한 컨넥션 성패 여부는 자동으로 호출되는 OnConnect()로 확인
	{	
		int nECode = GetLastError();
		if(nECode  != WSAEWOULDBLOCK) // 블럭킹이 아닌 에러인 경우는 즉시 연결 실패 메세지를 보냄
		{
			m_strError.Format("CDataSock::Connect() error : %d (Socket ID=%d)", nECode, sid);
			PostMessage(hWND, UM_DATA_CONNECT_FAILURE, hSID, (long)this); // 연결 실패 메세지를 보냄
			return FALSE;
		}
	}

	bAvailable = TRUE;	// 유효한 소켓임을 나타냄
	
	return TRUE;
}

// 데이터를 전송
// (보내야 할 데이터를 버퍼에 기록하고 OnSend()를 호출)
void CDataSock::SendData(void *pdata, int size)
{
if(bAvailable == FALSE) return;

	if(size<=0)
	{
		TRACE("CDataSock::SendData() - 보내는 사이즈가 0이거나 음수임(sid=%d, AddSize=%d)\n", hSID, size);
		return;
	}

	// 임시 버퍼에 데이터를 복사(이 임시버퍼는 멀티 쓰레드로부터 완전히 자유롭지는 않다 - 속도를 위하여 편법 적용)
	char* pbuf = g_sSendPool.GetNext();
	memcpy(pbuf, pdata, size);
	
	// 패킷 번호를 순차 증가(패킷 스니핑 체크용)
	PACKETHEADER* phead = (PACKETHEADER*)pbuf;
	SendPackCnt++;
	phead->PackCnt = SendPackCnt;
	// 패킷 암호화
	CCommMsg::EncodeAll((char *)pbuf);

	if(SendBuf.AddData(pbuf, size) == 0)
	{
		TRACE("Send Buffer가 가득차서 접속 끊음(sid=%d)\n", hSID);
		bAvailable = FALSE;	// 유효하지 못한 소켓임을 나타냄
		
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
	if(bAvailable == FALSE) return 0;  // 유효한 소켓이 아니면 리턴

	int nECode;

	/*
	int totbyte=0;
	if(IOCtl(FIONREAD, &totbyte)==0) // 한번에 받을수 있는 총 바이트 수를 구한다
	{
		nECode = GetLastError();
		m_strError.Format("CDataSock::ReceiveData()-IOCtl() error : %d (Socket ID=%d)", nECode, sid);
		PostMessage(hWND, UM_DATA_RECEIVE_FAILURE, NULL, (long)this); // 수신 실패 메세지를 보냄
		//CAsyncSocket::OnReceive(nErrorCode);
		return;
	}
	*/

	// 암호화 디코딩 시작위치 얻기
	int decodestart = RecvBuf.GetUsedSize(); 

	// 버퍼의 남은 사이즈가 0이면 rbyte=0 이된다
	// 만일 받아야할 데이터가 남은 버퍼 사이즈보다 크다면 뒤쪽부터 잘려서 버퍼에 들어간다.
	int bsize = RecvBuf.GetFreeSize();					// 버퍼의 남은 사이즈를 구함
	int rbyte = Receive(RecvBuf.GetEndPointer(), bsize);	// 버퍼에 데이터를 받음

	// 문제 : UM_DATA_CLOSE 메세지가 두번 발생한다.
	// 버퍼가 작아서 Receive()가 한번에 끝나지 않고 바로 뒤를 이어
	// OnReceive()가 또다시 호출되기 때문이다. PostMessage()하자마자 바로 발생한다
	// 그래서 접속 종료가 두번 일어나므로 이를 막기위해 bAvailable 플래그를 두었다.

	if(rbyte == SOCKET_ERROR)
	{
		nECode = GetLastError();
		if(nECode==WSAEWOULDBLOCK) // 블럭킹 중이면 다음번 OnReceive()에서 다시 받는다
		{
			return 0;
		}
		else // 블럭킹이 아니라 소켓 에러라면
		{
			// 연결 종료 메세지를 보냄
			TRACE("CDataSock::ReceiveData() - 소켓 에러 발생\n");
			bAvailable = FALSE;	// 유효하지 못한 소켓임을 나타냄
			if(bPostCloseMsg==FALSE)
			{
				bPostCloseMsg=TRUE;
				PostMessage(hWND, UM_DATA_CLOSE, hSID, (long)this); 
			}
			return 0;
		}
	}
	else if(rbyte==0) // 소켓이 끊어진 상태라면(혹은 버퍼의 남은 사이즈가 0일 경우)
	{
		// 연결 종료 메세지를 보냄
		TRACE("CDataSock::ReceiveData() - 소켓 끊어짐 (남은 버퍼사이즈=%d, 받은 데이터 사이즈=%d)\n", bsize, rbyte);
		bAvailable = FALSE;	// 유효하지 못한 소켓임을 나타냄
		if(bPostCloseMsg==FALSE)
		{
			bPostCloseMsg=TRUE;
			PostMessage(hWND, UM_DATA_CLOSE, hSID, (long)this); 
		}
		return 0;
	}
	else // 정상적으로 데이터를 받았다면
	{
		if(RecvBuf.AddUsedSize(rbyte)==FALSE) // 버퍼가 넘친 경우라면 연결 종료 시킨다.
		{
			// 연결 종료 메세지를 보냄
			TRACE("CDataSock::ReceiveData() - 리시브 버퍼 넘침\n");
			bAvailable = FALSE;	// 유효하지 못한 소켓임을 나타냄
			if(bPostCloseMsg==FALSE)
			{
				bPostCloseMsg=TRUE;
				PostMessage(hWND, UM_DATA_CLOSE, hSID, (long)this); 
			}
			return 0;
		}
		else
		{
			// 암호화된 패킷을 디코딩한다
			//RecvBuf.Data_Decode(decodestart, rbyte);
			return rbyte;
		}
	}
}

// 소켓에 읽어야할 데이타가 존재할때 발생
// (읽은 데이터를 버퍼에 보관하고 UM_DATA_RECEIVE 메세지를 통지하여 데이터가 있음을 알림)
void CDataSock::OnReceive(int nErrorCode)
{
	// 유효한 소켓이 아니면 리턴
	if(bAvailable == FALSE)
	{
		CAsyncSocket::OnReceive(nErrorCode);
		return;
	}

	if(nErrorCode == WSAENETDOWN) // 네트워크 오류
	{
		TRACE("CDataSock::OnReceive() - 네트워크 오류 발생\n");
		bAvailable = FALSE;	// 유효하지 못한 소켓임을 나타냄
		if(bPostCloseMsg==FALSE)
		{
			bPostCloseMsg=TRUE;
			PostMessage(hWND, UM_DATA_CLOSE, hSID, (long)this); 
		}
		CAsyncSocket::OnReceive(nErrorCode);
		return;
	}
	
	SendMessage(hWND, UM_DATA_RECEIVE, hSID, (long)this); 
	// Post할경우 버퍼에 데이터가 누적되기 쉽다. (버퍼가 차면서 접속 종료됨)
	// Send할 경우는 데이터가 처리되어야만 넘어가므로 버퍼가 누적되지 않는다.
			
	CAsyncSocket::OnReceive(nErrorCode);
}

// 소켓에 데이타 Send()가 가능할때 발생
// 최초 연결시 한번 발생하고 블럭킹된 센드가 다시 가능해질때 또 발생함
void CDataSock::OnSend(int nErrorCode)
{
	// 유효한 소켓이 아니면 리턴
	if(bAvailable == FALSE)
	{
		CAsyncSocket::OnSend(nErrorCode);
		return;
	}

	if(nErrorCode == WSAENETDOWN) // 네트워크 오류
	{
		bAvailable = FALSE;	// 유효하지 못한 소켓임을 나타냄
		if(bPostCloseMsg==FALSE)
		{
			bPostCloseMsg=TRUE;
			PostMessage(hWND, UM_DATA_CLOSE, hSID, (long)this); 
		}
		CAsyncSocket::OnSend(nErrorCode);
		return;
	}
	
	// 버퍼에서 보낼 데이터가 없으면 리턴
	if(SendBuf.GetUsedSize()==0)
	{
		CAsyncSocket::OnSend(nErrorCode);
		return;
	}

	int sbyte = Send(SendBuf.GetStartPointer(), SendBuf.GetUsedSize());
	if(sbyte == SOCKET_ERROR)
	{
		int nECode = GetLastError();
		if(nECode==WSAEWOULDBLOCK) // 블럭킹 중이면 다음번 OnSend()에서 다시 보낸다
		{
			return;
		}
		else // 블럭킹이 아니라 소켓 에러라면
		{
			// 연결 종료 메세지를 보냄
			bAvailable = FALSE;	// 유효하지 못한 소켓임을 나타냄
			if(bPostCloseMsg==FALSE)
			{
				bPostCloseMsg=TRUE;
				PostMessage(hWND, UM_DATA_CLOSE, hSID, (long)this); 
			}
			CAsyncSocket::OnSend(nErrorCode);
			return;
		}

	}
	else // 제대로 전송이 되었다면 (sbyte > 1) 일것이다.
	{
		SendBuf.DelData(sbyte);
	}

	//SendMessage(hWND, UM_DATA_????, hSID, (long)this);

	CAsyncSocket::OnSend(nErrorCode);
}


// 상대편 소켓이 닫힐때 발생
void CDataSock::OnClose(int nErrorCode)
{
	bAvailable = FALSE;	// 유효하지 못한 소켓임을 나타냄
	if(bPostCloseMsg==FALSE)
	{
		bPostCloseMsg=TRUE;
		PostMessage(hWND, UM_DATA_CLOSE, hSID, (long)this); 
	}
	CAsyncSocket::OnClose(nErrorCode);
}

/*======================================================================================
 Connect 요청에 대한 결과로 발생(성공할 수도 실패할 수도 있다)

	(비동기 소켓에서는 컨넥션 결과가 나중에 발생한다-요청 직후 바로 이루어지는 것이 아님)
	연결이 실패하면 재시도 하던가 클래스를 삭제해야한다.
	PostMessage()를 사용하는 이유는 함수처리중 클래스 포인터가 삭제되면 안되기 때문임
======================================================================================*/
void CDataSock::OnConnect(int nErrorCode)
{
	if(nErrorCode != 0) 
	{	
		m_strError.Format("CDataSock::OnConnect() error : %d (Socket ID=%d)", nErrorCode, hSID);
		PostMessage(hWND, UM_DATA_CONNECT_FAILURE, hSID, (long)this); // 연결 실패 메세지를 보냄
		CAsyncSocket::OnConnect(nErrorCode);
		return; 
	}
	GetSockName(SockAddr, SockPort); // 연결된 주소와 소켓 번호를 구한다
	SendMessage(hWND, UM_DATA_CONNECT, hSID, (long)this); // 연결 성공 메세지를 보냄
	CAsyncSocket::OnConnect(nErrorCode);
}

// Out of Band
void CDataSock::OnOutOfBandData(int nErrorCode)
{
	if(nErrorCode != WSAENETDOWN) return;

	TRACE("네트워크 에러 - WSAENETDOWN : OUT OF BAND (hSID=%d)\n", hSID);

	bAvailable = FALSE;	// 유효하지 못한 소켓임을 나타냄
	if(bPostCloseMsg==FALSE)
	{
		bPostCloseMsg=TRUE;
		PostMessage(hWND, UM_DATA_CLOSE, hSID, (long)this); 
	}
	CAsyncSocket::OnOutOfBandData(nErrorCode);
}

// SockCmnd.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "SockCmnd.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSockCmnd

CSockCmnd::CSockCmnd()
{
	pParentWnd = NULL;
	//RcvBuf.Init(20000);	// 리시브 데이터 처리용 버퍼 초기화
}

CSockCmnd::~CSockCmnd()
{
}


BEGIN_MESSAGE_MAP(CSockCmnd, CWnd)
	//{{AFX_MSG_MAP(CSockCmnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_MESSAGE(UM_SERV_ACCEPT, OnServAccept)
	ON_MESSAGE(UM_SERV_CLOSE, OnServClose)
	ON_MESSAGE(UM_DATA_RECEIVE, OnDataReceive)
	ON_MESSAGE(UM_DATA_CLOSE, OnDataClose)
	ON_MESSAGE(UM_DATA_CONNECT, OnDataConnect)
	ON_MESSAGE(UM_DATA_CONNECT_FAILURE, OnDataConnectFailure)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSockCmnd message handlers

BOOL CSockCmnd::Init(CWnd* parent)
{
	pParentWnd = parent;
	// 눈에 보이지 않는 윈도를 만든다(소켓 메세지 처리용 윈도)
	CRect rect(0,0,0,0);
	if(Create(NULL,NULL,WS_CHILD, rect, pParentWnd, 5454)==NULL) return FALSE;
	
	SockMan.Init(m_hWnd);

	return TRUE;
}

LONG CSockCmnd::OnServAccept(UINT wParam, LONG pSock)
{
	// 게임 중이 아닌 상태에서는 접속을 받지 않음
	if(g_Where != WH_GAME) return 1;

	int sid = SockMan.AcceptSocket();
	if( sid < 0)
	{
		TRACE("CSockCmnd::OnServAccept() - 접속 요청이 들어왔으나 연결 실패했음");
		return 1;
	}
	return 1;
}

LONG CSockCmnd::OnServClose(UINT wParam, LONG pSock)
{
	return 1;
}

LONG CSockCmnd::OnDataReceive(UINT wParam, LONG pSock)
{
	int sid = wParam;
	CDataSock *pSocket = SockMan.GetDataSocketPtr(sid);
	if(pSocket==NULL) return 0;
	
	if(sid<0 || sid>=MAX_DATA_SOCKET) 
	{
		TRACE("CSockCmnd::OnDataReceive() - 잘못된 소켓 번호가 들어왔음\n"); 
		return 0;
	}

	int rbyte = pSocket->ReceiveData();
	if(rbyte==0)
	{
		TRACE("CSockCmnd::OnDataReceive() - rbyte=0\n");
		return 0;
	}

	CStreamBuf *pRB = &(pSocket->RecvBuf);
	char *pbuf = NULL;

	CCommMsg msg;

REPEAT:

	int state=0;
	int signal=0, msglen=0;
	
	if(state == 0) // 헤더를 찾는중
	{
		int bufsize = pRB->GetUsedSize();
		if(bufsize>=PACKET_HEADERSIZE) // 헤더에 필요한 사이즈를 받았으면
		{
			int delbyte=0;
			pbuf = pRB->GetStartPointer();
			for(int i=0; i<=bufsize-PACKET_HEADERSIZE; i++)
			{
				delbyte=i;
				if(strncmp(pbuf+i, MSG_ID, strlen(MSG_ID))==0)
				{
					state = 1;
					break;
				}
			}
			if(delbyte>0)
			{ 
				pRB->DelData(delbyte);
				TRACE("찌꺼기 바이트가 발생하여 삭제함(size=%d)\n",delbyte);
			}
			if(state == 1)
			{
				pbuf = pRB->GetStartPointer();
				// 메시지 정보를 체크한다
				//BOOL encoded;	// 임시 변수
				//msg.CheckInfo(pbuf, &encoded, &signal, &msglen);
				// 메시지 정보를 체크한다
				msg.CheckInfo(pbuf, &signal, &msglen);
			}
		}
	}

	if(state == 1) // 헤더를 찾았으면
	{
		int totsize = PACKET_HEADERSIZE + msglen;

		if(pRB->GetUsedSize() >= totsize)
		{
			//char* lpdata = pRB->GetStartPointer();
			char* lpdata = new char[totsize];
			memcpy(lpdata, pRB->GetStartPointer(), totsize);
			
			// 게임 패킷 도착 메시지 준비
			GAMENOTIFY data;
			data.Sid = sid;
			data.Signal = signal;
			data.TotSize = totsize;
			data.lpData = lpdata;

			// 큐를 미리 삭제하여 중복 호출을 막는다
			pRB->DelData(totsize);

			// 완성된 패킷이 도착하였음을 메인 뷰에 알림
			pParentWnd->SendMessage(UM_PACKET_NOTIFY, (WPARAM)&data, 0);
			
			delete lpdata;

			// 이미 소켓이 제거된 이후라면
			if(!SockMan.GetDataSocketPtr(sid))
				return 1;

			// [게임 아이템 작업]
			// 위의 패킷을 처리한후 프로그램이 종료된 경우는 윈도우 핸들이 NULL이다
			if(!pParentWnd->GetSafeHwnd()) 
				return 1;
			
			// 패킷 처리도중 이미 프로그램이 종료되어버린 경우라면 버퍼가 파괴되었기 때문에 검사하여 처리를 중단한다
			if(IsBadWritePtr(pRB->pBuf, 4))
				return 1;

		//	if(g_MainSrvSID < 0 ) return 1;

			state = 0;
			if(pRB->GetUsedSize()>0) goto REPEAT;
		}
	}
	return 1;
}

LONG CSockCmnd::OnDataClose(UINT wParam, LONG pSock)
{
	int sid = wParam;
	CDataSock *pSocket = SockMan.GetDataSocketPtr(sid);
	if(pSocket==NULL) return 1;

	// 메인서버와의 접속이 끊어진 경우라면
	if(sid == g_MainSrvSID)
	{
		// 메인서버와의 접속이 끊어지면 서버소켓과 모든 데이터 소켓도 접속을 끊는다
		SockMan.CloseServerSocket();
		for(int i=0; i<MAX_DATA_SOCKET; i++)
		{
			SockMan.CloseDataSocket(i);
		}

		if(g_pTitleDlg->m_MsgDlg.GetSafeHwnd()) {
			// 메시지 창 닫음
			g_pTitleDlg->m_MsgDlg.OnOK();
		}

		// 로그인 중이 아니면 접속 끊김을 알린다
		if(g_Where!=WH_LOGIN && g_Where!=WH_FIRST)
		{
			g_MainSrvSID = -1;

			if(g_MyInfo.UI.PMoney >= 100){ // 충전으로 인한 접속 종료는 메세지를 띄우지 않는다..
				CString str;
				str.Format("메인 서버와의 접속이 끊어졌습니다");
				AfxMessageBox(str);
			}
			
			g_Where = WH_FIRST;
			g_pMainDlg->ChangeView(0);
			//g_pMainDlg->OnCancel();
			g_pMainDlg->PostMessage(WM_COMMAND, IDCANCEL);
		}
		else
		{
			(g_pMainDlg->m_ConnectMsgDlg.GetDlgItem(IDCANCEL))->EnableWindow(TRUE);
		}
	}
	
	BOOL rtn = SockMan.CloseDataSocket(sid);
	if(rtn==FALSE)
	{
		TRACE("CSockMan::OnDataClose() - 접속 종료 처리 실패\n");
		return 1;
	}

	return 1;
}

LONG CSockCmnd::OnDataConnect(UINT wParam, LONG pSock)
{
	int sid = wParam;
	CDataSock *pSocket = SockMan.GetDataSocketPtr(sid);
	if(pSocket==NULL) return 1;

	return 1;
}

LONG CSockCmnd::OnDataConnectFailure(UINT wParam, LONG pSock)
{
	int sid = wParam;
	CDataSock *pSocket = SockMan.GetDataSocketPtr(sid);
	if(pSocket==NULL) return 1;

	CString str;

	// 로그인하는중일때 메인서버와 접속이 실패한 경우라면
	if(g_Where==WH_LOGIN && g_MainSrvSID==sid)
	{
		g_Where = WH_FIRST;
		g_Wait = FALSE;
		str.Format("메인 서버와 접속이 실패하였습니다", sid);
		g_pMainDlg->m_MsgBoxDlg.MsgBox(0, str);
		g_pMainDlg->m_ConnectMsgDlg.OnCancel();
	}

	SockMan.CloseDataSocket(sid);
	return 1;
}


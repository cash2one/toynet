// SockCmnd.cpp : implementation file
//

#include "stdafx.h"

#include "NMServerAgent.h"
#include "NMServerAgentDlg.h"
#include "SockCmnd.h"





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
	//RcvBuf.Init(20000);	// ���ú� ������ ó���� ���� �ʱ�ȭ
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
	// ���� ������ �ʴ� ������ �����(���� �޼��� ó���� ����)
	CRect rect(0,0,0,0);
	if(Create(NULL,NULL,WS_CHILD, rect, pParentWnd, 5454)==NULL) return FALSE;
	
	SockMan.Init(m_hWnd);

	return TRUE;
}

LONG CSockCmnd::OnServAccept(UINT wParam, LONG pSock)
{
	// ���� ���� �ƴ� ���¿����� ������ ���� ����
//	if(g_Where != WH_GAME) return 1;

	int sid = SockMan.AcceptSocket();
	if( sid < 0)
	{
		TRACE("CSockCmnd::OnServAccept() - ���� ��û�� �������� ���� ��������");
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
		TRACE("CSockCmnd::OnDataReceive() - �߸��� ���� ��ȣ�� ������\n"); 
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
	
	if(state == 0) // ����� ã����
	{
		int bufsize = pRB->GetUsedSize();
		if(bufsize>=PACKET_HEADERSIZE) // ����� �ʿ��� ����� �޾�����
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
				TRACE("��� ����Ʈ�� �߻��Ͽ� ������(size=%d)\n",delbyte);
			}
			if(state == 1)
			{
				pbuf = pRB->GetStartPointer();
				// �޽��� ������ üũ�Ѵ�
				msg.CheckInfo(pbuf, &signal, &msglen);
			}
		}
	}

	if(state == 1) // ����� ã������
	{
		int totsize = PACKET_HEADERSIZE + msglen;

		if(pRB->GetUsedSize() >= totsize)
		{
			char* lpdata = pRB->GetStartPointer();
			
			// ���� ��Ŷ ���� �޽��� �غ�
			GAMENOTIFY data;
			data.Sid = sid;
			data.Signal = signal;
			data.TotSize = totsize;
			data.lpData = lpdata;

			// �ϼ��� ��Ŷ�� �����Ͽ����� ���� �信 �˸�
			pParentWnd->SendMessage(UM_PACKET_NOTIFY, (WPARAM)&data, 0);

			pRB->DelData(totsize);
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
/*
	// ���μ������� ������ ������ �����
	if(sid == g_MainSrvSID)
	{
		// ���μ������� ������ �������� �������ϰ� ��� ������ ���ϵ� ������ ���´�
		SockMan.CloseServerSocket();
		for(int i=0; i<MAX_DATA_SOCKET; i++)
		{
			SockMan.CloseDataSocket(i);
		}

		// �α��� ���� �ƴϸ� ���� ������ �˸���
		if(g_Where!=WH_LOGIN && g_Where!=WH_FIRST)
		{
			CString str;
			str.Format("���� �������� ������ ���������ϴ�");
			AfxMessageBox(str);
			g_MainSrvSID = -1;
			g_Where = WH_FIRST;
			g_pMainDlg->ChangeView(0);
			g_pMainDlg->OnCancel();
		}
		else
		{
			(g_pMainDlg->m_ConnectMsgDlg.GetDlgItem(IDCANCEL))->EnableWindow(TRUE);
		}
	}

	BOOL rtn = SockMan.CloseDataSocket(sid);
	if(rtn==FALSE)
	{
		TRACE("CSockMan::OnDataClose() - ���� ���� ó�� ����\n");
		return 1;
	}
	*/
	
	g_pMainDlg->Disconnect(sid);

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
/*
	CString str;

	// �α����ϴ����϶� ���μ����� ������ ������ �����
	if(g_Where==WH_LOGIN && g_MainSrvSID==sid)
	{
		g_Where = WH_FIRST;
		g_Wait = FALSE;
		str.Format("���� ������ ������ �����Ͽ����ϴ�", sid);
		AfxMessageBox(str);
		g_pMainDlg->m_ConnectMsgDlg.OnCancel();
	}
*/
	// ���ῡ �����Ѵ�
	g_pMainDlg->ConnectFail(sid);

	//SockMan.CloseDataSocket(sid);
	return 1;
}

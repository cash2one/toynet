// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "62CutPokerServer.h"

#include "62CutPokerServerDoc.h"
#include "62CutPokerServerView.h"

#include "MainFrm.h"

#include "DBThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_FILE_CONNECTDENY, OnFileConnectdeny)
	ON_COMMAND(ID_TOOL_FORCETERMINATEDBTHREAD, OnToolForceterminatedbthread)	
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_TRAYICON_MSG, TrayIconMsg)
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style &= ~FWS_ADDTOTITLE; // 제목 설정 비트 제거

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	static BOOL bClosing = FALSE;
	if(bClosing) return;

	if(g_pMainView->bIsForceClose == FALSE)
	{
		CString str;
		if(g_pMainView->TotUser > 0) str.Format("현재 %d명의 사용자가 접속중입니다.\n지금 종료하면 모든 사용자와의 접속이 끊어집니다.\n그래도 종료하시겠습니까?", g_pMainView->TotUser);
		else str.Format("서버를 종료하시겠습니까?");
		int rtn = AfxMessageBox(str,MB_YESNO);
		if(rtn == IDNO) return;
	}

	bClosing=TRUE;
	g_pMainView->PrepareClose();

	//CFrameWnd::OnClose();
}

void CMainFrame::OnFileConnectdeny() 
{
	// TODO: Add your command handler code here
	CMenu *pMenu = GetMenu();
	if(g_pMainView->bConnectDeny==FALSE)
	{
		pMenu->CheckMenuItem(ID_FILE_CONNECTDENY, MF_CHECKED);
		g_pMainView->bConnectDeny = TRUE;
		M_ServerAcceptStatus(m_hWnd,g_pMainView->bConnectDeny);	// [PC방 작업]
		(g_pMainView->GetDlgItem(IDC_STATIC_CONNECTSTATE))->SetWindowText("접속 거부중 ◐");
		AfxMessageBox("사용자의 접속을 거부합니다");
	}
	else
	{
		pMenu->CheckMenuItem(ID_FILE_CONNECTDENY, MF_UNCHECKED);
		g_pMainView->bConnectDeny = FALSE;
		M_ServerAcceptStatus(m_hWnd,g_pMainView->bConnectDeny);	// [PC방 작업]
		(g_pMainView->GetDlgItem(IDC_STATIC_CONNECTSTATE))->SetWindowText("접속 허용중 ▶");
		AfxMessageBox("사용자의 접속을 허용합니다");
	}
}

void CMainFrame::OnToolForceterminatedbthread() 
{
	// TODO: Add your command handler code here
#ifdef _DEBUG
	for(int i=0; i<MAX_DATA_SOCKET; i++) if(SockMan.pDataSock[i]) SockMan.pDataSock[i]->ReservClose();
#endif
}

LRESULT CMainFrame::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
		// (추가 사항 1) - 제일 위에 넣을 것
	if(message == uCmmGameStatusSC)
	{
		SetBridgeHandle((HWND)wParam);
		M_SendMySetatus(m_hWnd,g_pMainView->bConnectDeny,g_pMainView->RealUser);
	}

	if(message == uCmmHaloMsgSC)
	{
		SetBridgeHandle((HWND)wParam);
		M_ImHereBridge(m_hWnd,g_pMainView->Cfg.GameCode,g_pMainView->Cfg.ServerCode,g_pMainView->Cfg.ServerPort,g_pMainView->Cfg.ServerName);
		
	}	
	if(message == uCmmGotBootMsgSC)
	{
		SetBridgeHandle((HWND)wParam);
	}
	if(message == uCmmStopAcceptSC)
	{
		CMenu *pMenu = GetMenu();
		int btf = (int)lParam;
		if(btf)
		{
			if(g_pMainView->bConnectDeny==FALSE)
			{
				pMenu->CheckMenuItem(ID_FILE_CONNECTDENY, MF_CHECKED);
				g_pMainView->bConnectDeny = TRUE;
				(g_pMainView->GetDlgItem(IDC_STATIC_CONNECTSTATE))->SetWindowText("접속 거부중 ◐");
				M_ServerAcceptStatus(m_hWnd,g_pMainView->bConnectDeny);
			}
		} else
		{
			if(g_pMainView->bConnectDeny)
			{
				pMenu->CheckMenuItem(ID_FILE_CONNECTDENY, MF_UNCHECKED);
				g_pMainView->bConnectDeny = FALSE;
				(g_pMainView->GetDlgItem(IDC_STATIC_CONNECTSTATE))->SetWindowText("접속 허용중 ▶");
				M_ServerAcceptStatus(m_hWnd,g_pMainView->bConnectDeny);
			}
		}
	}

	if(message == uCmmServerDownMsgSC)
	{
		g_pMainView->PrepareClose();
	}

	if(message == WM_COPYDATA)
	{
		COPYDATASTRUCT *SendDataStruct = (COPYDATASTRUCT *)lParam;
		if(SendDataStruct->cbData == sizeof(tag_SvrMsg))
		{
			tag_SvrMsg cData;
			memcpy(&cData,SendDataStruct->lpData,sizeof(tag_SvrMsg));

			if(cData.nCode==1) {
				CSV_SERVERMSG smsg;
				smsg.Set(0, cData.strMsg);
				g_pMainView->SendMsgToAll(smsg.pData, smsg.GetTotalSize());
			}

			if(cData.nCode==2) {
				CSV_SERVERMSG smsg;
				smsg.Set(1, cData.strMsg);
				g_pMainView->SendMsgToAll(smsg.pData, smsg.GetTotalSize());
			}

			if(cData.nCode==3) {
				CSV_SERVERMSG smsg1, smsg2;
				smsg1.Set(0, cData.strMsg);
				g_pMainView->SendMsgToAll(smsg1.pData, smsg1.GetTotalSize());
				smsg2.Set(1, cData.strMsg);
				g_pMainView->SendMsgToAll(smsg2.pData, smsg2.GetTotalSize());
			}
		}

		else
		// 여기 아래에 내용을 추가한다. (추가 사항 2)
		if(SendDataStruct->cbData == sizeof(tag_SingleMsg))			// 한 사용자에게 메시지 보내기 
		{
			tag_SingleMsg cData;
			memcpy(&cData,SendDataStruct->lpData,sizeof(tag_SingleMsg));
			
			for(int i=0; i<MAX_USER; i++) {
				if(faststrcmp(cData.ID,User[i].UI.ID)==0) {
					int sid = User[i].Sid;
					USERVAL *pUserVal = NULL;
					if(SockMan.pDataSock[sid]!=NULL) 
					{
						pUserVal = (USERVAL*)(SockMan.pDataSock[sid]->m_pUserPtr);
						if(pUserVal == NULL) {
							g_pMainView->UserDisconnectProccess(i);
							SockMan.ReservClose(sid); // 연결 종료 예약
						} else
						{
							// 사용자에게 메시지 보내기 
							CSV_SERVERMSG smsg1, smsg2;
							smsg1.Set(0, cData.strMsg);
							SockMan.SendData(sid,smsg1.pData, smsg1.GetTotalSize());
							smsg2.Set(1, cData.strMsg);
							SockMan.SendData(sid,smsg2.pData, smsg2.GetTotalSize());
							M_SingleMsgSecc(m_hWnd,User[i].UI.ID,1);
						}
						
					}
					break;
				}
			}
		} 
		else if(SendDataStruct->cbData == sizeof(tag_DisbandUser))		// 사용자 강제 종료
		{
				tag_DisbandUser cData;
				memcpy(&cData,SendDataStruct->lpData,sizeof(tag_DisbandUser));
				
				CString str;
				for(int i=0; i<MAX_USER; i++) {
					if(faststrcmp(cData.ID,User[i].UI.ID)==0) {
						int sid = User[i].Sid;
						USERVAL *pUserVal = NULL;
						if(SockMan.pDataSock[sid]!=NULL) pUserVal = (USERVAL*)(SockMan.pDataSock[sid]->m_pUserPtr);
						
						if(SockMan.pDataSock[sid]==NULL) {
							int unum = User[i].UI.UNum;
							if(unum<0 || unum>=MAX_USER) unum = i;
							if(faststrcmp(User[unum].UI.ID, User[i].UI.ID) != 0) {
								//M_DisConnectAns(m_hWnd,FALSE);
								return CFrameWnd::DefWindowProc(message, wParam, lParam);
							}
							g_pMainView->UserDisconnectProccess(i);
						}
						else if(pUserVal == NULL) {
							g_pMainView->UserDisconnectProccess(i);
							SockMan.ReservClose(sid); // 연결 종료 예약
						}
						else if(pUserVal->iUser1 == FALSE) {
							int unum = User[i].UI.UNum;
							if(unum<0 || unum>=MAX_USER) {
								unum = i;
							}
							if(faststrcmp(User[unum].UI.ID, User[i].UI.ID) != 0) {
								//M_DisConnectAns(m_hWnd,FALSE);
							}
							g_pMainView->UserDisconnectProccess(i);
						}
						else SockMan.ReservClose(sid); // 연결 종료 예약
						
						//M_DisConnectAns(m_hWnd,TRUE);
						M_SingleMsgSecc(m_hWnd,User[i].UI.ID,0);
						break;
					}
				}
			}
			// 추가 끝 


	}

	return CFrameWnd::DefWindowProc(message, wParam, lParam);
}

// [서버 에이전트 작업] 
void CMainFrame::Connectdeny()
{
	CMenu *pMenu = GetMenu();
	if(g_pMainView->bConnectDeny==FALSE)
	{
		pMenu->CheckMenuItem(ID_FILE_CONNECTDENY, MF_CHECKED);
		g_pMainView->bConnectDeny = TRUE;
		M_ServerAcceptStatus(m_hWnd,g_pMainView->bConnectDeny);	// [PC방 작업]
		(g_pMainView->GetDlgItem(IDC_STATIC_CONNECTSTATE))->SetWindowText("접속 거부중 ◐");
	}
	else
	{
		pMenu->CheckMenuItem(ID_FILE_CONNECTDENY, MF_UNCHECKED);
		g_pMainView->bConnectDeny = FALSE;
		M_ServerAcceptStatus(m_hWnd,g_pMainView->bConnectDeny);	// [PC방 작업]
		(g_pMainView->GetDlgItem(IDC_STATIC_CONNECTSTATE))->SetWindowText("접속 허용중 ▶");
	}
}



void CMainFrame::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	CFrameWnd::OnWindowPosChanging(lpwndpos);
	
	// TODO: Add your message handler code here
	// 트레이 변경시 - jeong
	//m_bIsTrayIcon = FALSE;
	//RegistTrayIcon();
	//ShowWindow(SW_HIDE);
	//lpwndpos->flags &= ~SWP_SHOWWINDOW;
}

void CMainFrame::RegistTrayIcon()
{
	NOTIFYICONDATA  nid;
	nid.cbSize = sizeof(nid);
	nid.hWnd = m_hWnd; // 메인 윈도우 핸들
	nid.uID = IDR_MAINFRAME;  // 아이콘 리소스 ID
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP; // 플래그 설정
	nid.uCallbackMessage = WM_TRAYICON_MSG; // 콜백메시지 설정
	nid.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME); // 아이콘 로드 

	char strTitle[256];
	GetWindowText(strTitle, sizeof(strTitle)); // 캡션바에 출력된 문자열 얻음
	lstrcpy(nid.szTip, strTitle); 
	Shell_NotifyIcon(NIM_ADD, &nid);
	SendMessage(WM_SETICON, (WPARAM)TRUE, (LPARAM)nid.hIcon);
	m_bIsTrayIcon = TRUE;

}

void CMainFrame::OnDestroy() 
{
	CFrameWnd::OnDestroy();
	
	// TODO: Add your message handler code here
	if(m_bIsTrayIcon) // 현재 트레이 아이콘으로 설정되었는지 확인 
	 {
		  NOTIFYICONDATA  nid;
		  nid.cbSize = sizeof(nid);
		  nid.hWnd = m_hWnd; // 메인 윈도우 핸들
		  nid.uID = IDR_MAINFRAME;

		  // 작업 표시줄(TaskBar)의 상태 영역에 아이콘을 삭제한다.
		  Shell_NotifyIcon(NIM_DELETE, &nid);
	 }	
}

LRESULT CMainFrame::TrayIconMsg(WPARAM wParam, LPARAM lParam)
{
	if(lParam == WM_LBUTTONDBLCLK)
	{  
		ShowWindow(SW_SHOW);
	}

	return true;

}

void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam) 
{
	// TODO: Add your message handler code here and/or call default
	if(nID == SC_MINIMIZE)
	{ 
		ShowWindow(SW_HIDE);
	}
	else
		CFrameWnd::OnSysCommand(nID, lParam);
}

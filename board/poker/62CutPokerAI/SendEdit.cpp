// SendEdit.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "62CutPokerDlg.h"
#include "Global.h"
#include "SendEdit.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSendEdit

CSendEdit::CSendEdit()
{
	iUse=0;
	NowHPos = 0;

	bSetTextColor = FALSE;
	bSetBkColor = FALSE;
	TextColor = RGB(0,0,0);
	BkColor = RGB(255,255,255);
}

CSendEdit::~CSendEdit()
{
	if(BkBrush.m_hObject) { BkBrush.DeleteObject(); BkBrush.m_hObject = NULL; }
	if(TextBrush.m_hObject) { TextBrush.DeleteObject(); TextBrush.m_hObject = NULL; }
}


BEGIN_MESSAGE_MAP(CSendEdit, CEdit)
	//{{AFX_MSG_MAP(CSendEdit)
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CSendEdit::SetTextColor(COLORREF color)
{
	bSetTextColor = TRUE;
	TextColor = color;
	if(TextBrush.m_hObject) { TextBrush.DeleteObject(); TextBrush.m_hObject = NULL; }
	TextBrush.CreateSolidBrush(color);
	Invalidate();
}

void CSendEdit::SetBkColor(COLORREF color)
{
	bSetBkColor = TRUE;
	BkColor = color;
	if(BkBrush.m_hObject) { BkBrush.DeleteObject(); BkBrush.m_hObject = NULL; }
	BkBrush.CreateSolidBrush(color);
	Invalidate();
}


/////////////////////////////////////////////////////////////////////////////
// CSendEdit message handlers

BOOL CSendEdit::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(iUse == 1)
	{
		//cs.style |= WS_CHILD | WS_BORDER;
		//cs.dwExStyle |= WS_EX_CLIENTEDGE;
	}
	
	return CEdit::PreCreateWindow(cs);
}


void CSendEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nChar)
	{
	case VK_RETURN:
		{
			CString outstr;
			char str[256]={0,};
			//GetLine(LineFromChar(-1), str, 255);	// 현재 캐럿 위치의 문자열을 읽음
			GetWindowText(str, 128);
			SetWindowText("");						// 에디트 컨트롤을 비운다
			NowHPos = 0;


#ifdef _DEBUG	
			
			/*if(strcmp(str,"천사")==0)
				{
					PlayScr[0].AddToMyItem(CLEAR);
					PlayScr[0].AddToMyItem(CLEAR);
					PlayScr[0].AddToMyItem(CLEAR);
					PlayScr[0].AddToMyItem(CLEAR);
					PlayScr[0].AddToMyItem(CLEAR);
				}
				if(strcmp(str,"반사")==0)
				{
					PlayScr[0].AddToMyItem(REFLECTION);
					PlayScr[0].AddToMyItem(REFLECTION);
					PlayScr[0].AddToMyItem(REFLECTION);
					PlayScr[0].AddToMyItem(REFLECTION);
				}
				if(strcmp(str,"ㅋㅋㅋ")==0)
				{
					PlayScr[0].AddToMyItem(1);
					PlayScr[0].AddToMyItem(2);
					PlayScr[0].AddToMyItem(3);
					PlayScr[0].AddToMyItem(4);
					PlayScr[0].AddToMyItem(5);
					PlayScr[0].AddToMyItem(6);
					PlayScr[0].AddToMyItem(7);
					PlayScr[0].AddToMyItem(8);
					PlayScr[0].AddToMyItem(9);
					PlayScr[0].AddToMyItem(10);
				}
				if(strcmp(str,"워러")==0)
				{
					PlayScr[0].AddToMyItem(WATER);
					PlayScr[0].AddToMyItem(WATER);
					PlayScr[0].AddToMyItem(WATER);
					PlayScr[0].AddToMyItem(WATER);
					PlayScr[0].AddToMyItem(WATER);
					PlayScr[0].AddToMyItem(WATER);
					PlayScr[0].AddToMyItem(WATER);
				}
*/

#endif


			// 검사
			if(strlen(str)==0) return;
			if(strlen(str)==1 && str[0]==-1) return;
			
			// 문장 끝에 붙는 쓸데 없는 코드를 삭제함
			if(str[strlen(str)-1]==1) str[strlen(str)-1]=0;

			// 허용되지 않은 특수문자를 사용하였다면 공백문자로 바꿈
			int slen = strlen(str);
			for(int s=0; s<slen-1; s++) {
				// 8:backspace, 9:tab, 10:linefeed, 13:carriage return
				if(str[s] == 8 || str[s] == 9 || str[s] == 10 || str[s] == 13) {
					str[s] = ' ';
				}
			}

			// 히스토리 갱신
			for(int i=9; i>1; i--)
			{
				History[i] = History[i-1];
			}
			History[1].Format("%s",str);
			History[0].Format("");

			// 대화 도배 검사
			if(NoChat.CheckNoChat(str)==TRUE)
			{
				CString nostr;
				nostr.Format("[알림] 대화 도배로 인하여 2분간 채팅이 금지된 상태입니다.\n");
				if(iUse==0) // 대기실 다이얼로그이면
				{
					g_pLobyDlg->m_ChatViewEdit.AddText(&nostr, RGB(200, 0, 0));
				}
				if(iUse==1) // 게임 다이얼로그이면
				{
					g_pGameView->ChatBox.ChatViewEdit.AddText(&nostr, RGB(200, 0, 0));
					// 채팅창의 크기를 늘어나기 전의 사이즈로 돌린다
					//g_pGameDlg->m_GameView.ChatBox.OnEnter();
				}
				break;
			}

			// 사용자의 상세 정보 요청(개발자용)
			if(strncmp(str, "/#GetDetailUserInfo ", 20)==0)
			{
				char strDestID[256]={0,};
				int i=0;
				int now=0;
				for(i=0; i<(int)strlen(str); i++)
				{
					if(str[i] == ' ') break;
				}
				i++;

				for(;i<(int)strlen(str); i++)
				{
					if(str[i] == ' ') break;
					strDestID[now] = str[i]; now++;
				}
				i++;

				now=0;
				char strChat[256]={0,};
				for(;i<(int)strlen(str); i++)
				{
					strChat[now] = str[i]; now++;
				}

				if(strlen(strDestID) == 0) return;
				if(strlen(strDestID) >= 16) return;

				int DestUNum=-1;
				int num=-1;
				for(;;)
				{
					num = g_pLobyDlg->m_UserList.GetNextItem(num, LVNI_ALL);
					if(num == -1) break;
					CString szID = g_pLobyDlg->m_UserList.GetItemText(num, 0);
					if(szID.Compare(strDestID)==0) 
					{
						DestUNum = g_pLobyDlg->m_UserList.GetItemData(num);
						break;
					}
				}

				CString outstr;

				if(DestUNum==-1) 
				{
					outstr.Format("[%s]님은 현재 접속하지 않았습니다\n", strDestID);
					if(iUse==0) // 대기실 다이얼로그이면
						g_pLobyDlg->m_ChatViewEdit.AddText(&outstr, RGB(255,100,100));
					if(iUse==1) // 게임 다이얼로그이면
					{
						g_pGameView->ChatBox.ChatViewEdit.AddText(&outstr, RGB(255,100,100));
						// 채팅창의 크기를 늘어나기 전의 사이즈로 돌린다
						//g_pGameDlg->m_GameView.ChatBox.OnEnter();
					}
					return;
				}

				CSV_ASK_DETAILUSERINFO aduimsg;
				aduimsg.Set(DestUNum, strDestID);
				SockMan.SendData(g_MainSrvSID, aduimsg.pData, aduimsg.GetTotalSize());

				if(iUse==0) // 대기실 다이얼로그이면
				{
					outstr.Format("(사용자 상세정보 요청 : %s)\n", strDestID);
					g_pLobyDlg->m_ChatViewEdit.AddText(&outstr);
				}
				if(iUse==1) // 게임 다이얼로그이면
				{
					outstr.Format("(사용자 상세정보 요청 : %s)\n", strDestID);
					g_pGameView->ChatBox.ChatViewEdit.AddText(&outstr);
					// 채팅창의 크기를 늘어나기 전의 사이즈로 돌린다
					//g_pGameDlg->m_GameView.ChatBox.OnEnter();
				}
				
				return;
			}

			// 귓말인지 검사(대소문자 구분 없음)
			if(strnicmp(str, "/w ", 3)==0 || strncmp(str, "/ㅈ ", 3)==0)
			{
				char strDestID[256]={0,};
				int i=0;
				int now=0;
				for(i=0; i<(int)strlen(str); i++)
				{
					if(str[i] == ' ') break;
				}
				i++;

				for(;i<(int)strlen(str); i++)
				{
					if(str[i] == ' ') break;
					strDestID[now] = str[i]; now++;
				}
				i++;

				now=0;
				char strChat[256]={0,};
				for(;i<(int)strlen(str); i++)
				{
					strChat[now] = str[i]; now++;
				}

				if(strlen(strDestID) == 0) return;
				if(strlen(strDestID) >= 16) return;
				if(strlen(strChat) == 0) return;

				int DestUNum=-1;
				int num=-1;
				for(;;)
				{
					num = g_pLobyDlg->m_UserList.GetNextItem(num, LVNI_ALL);
					if(num == -1) break;
					CString szID = g_pLobyDlg->m_UserList.GetItemText(num, 0);
					if(szID.Compare(strDestID)==0) 
					{
						DestUNum = g_pLobyDlg->m_UserList.GetItemData(num);
						break;
					}
				}

				CString outstr;

				if(DestUNum==-1) 
				{
					outstr.Format("[%s]님은 현재 접속하지 않았습니다\n", strDestID);
					if(iUse==0) // 대기실 다이얼로그이면
						g_pLobyDlg->m_ChatViewEdit.AddText(&outstr, RGB(255,100,100));
					if(iUse==1) // 게임 다이얼로그이면
					{
						g_pGameView->ChatBox.ChatViewEdit.AddText(&outstr, RGB(255,100,100));
						// 채팅창의 크기를 늘어나기 전의 사이즈로 돌린다
						//g_pGameDlg->m_GameView.ChatBox.OnEnter();
					}
					return;
				}

				// 욕설 검사
				if(BadChat.IsBad(strChat)) {
					outstr.Format("바르고 고운말을 사용합시다.\n");
					if(iUse==0) // 대기실 다이얼로그이면
					{
						g_pLobyDlg->m_ChatViewEdit.AddText(&outstr, RGB(255,0,0));
					}
					if(iUse==1) // 게임 다이얼로그이면
					{
						g_pGameView->ChatBox.ChatViewEdit.AddText(&outstr, RGB(255,0,0));
					}
					break;
				}

				if(iUse==0) // 대기실 다이얼로그이면
				{
					outstr.Format("(귓말) %s\n", str);
					g_pLobyDlg->m_ChatViewEdit.AddText(&outstr);
				}
				if(iUse==1) // 게임 다이얼로그이면
				{
					/*
					if(g_RI.State==1)
					{
						BOOL bDont = FALSE;
						for(int i=0; i<5; i++)
						{
							if(strcmp(strDestID, Play[i].UI.ID)==0) { bDont=TRUE; break; }
						}

						if(bDont)
						{
							outstr.Format("[알림] 게임중에는 같은방 플레이어에게 귓말을 보낼 수 없습니다.\n");
							g_pGameView->m_MyChat.ChatViewEdit.AddText(&outstr, RGB(255,180,180));
							// 채팅창의 크기를 늘어나기 전의 사이즈로 돌린다
							//g_pGameDlg->m_GameView.ChatBox.OnEnter();
							return;
						}
					}
					*/
					
					outstr.Format("(귓말) %s\n", str);
					g_pGameView->ChatBox.ChatViewEdit.AddText(&outstr);
					// 채팅창의 크기를 늘어나기 전의 사이즈로 돌린다
					//g_pGameDlg->m_GameView.ChatBox.OnEnter();
				}

				CSV_WHISPERCHAT wcmsg;
				wcmsg.Set(Play[0].UI.ID, DestUNum, strDestID, strChat);
				SockMan.SendData(g_MainSrvSID, wcmsg.pData, wcmsg.GetTotalSize());
				
				return;
			}

			// 욕설 검사
			if(BadChat.IsBad(str)) {
				outstr.Format("바르고 고운말을 사용합시다.\n");
				if(iUse==0) // 대기실 다이얼로그이면
				{
					g_pLobyDlg->m_ChatViewEdit.AddText(&outstr, RGB(255,0,0));
				}
				if(iUse==1) // 게임 다이얼로그이면
				{
					g_pGameView->ChatBox.ChatViewEdit.AddText(&outstr, RGB(255,0,0));
				}
				break;
			}

			if(iUse==0) // 대기실 다이얼로그이면
			{
				// 메인 서버에게 채팅 문자열을 보냄
				CSV_CHATDATA msg;
				msg.Set(Play[0].UI.ID, str);
				SockMan.SendData(g_MainSrvSID, msg.pData, msg.GetTotalSize());
			}

			if(iUse==1) // 게임 다이얼로그이면
			{
			
			#ifdef _DEBUG
				// 특수 작업용(개발자 사용목적)
				if(strcmp(str, "/go")==0)
				{
					Game.DoPrepareGame();
					break;
				}
				if(strcmp(str, "/q")==0)
				{
					g_pGameDlg->ExitGame();
					break;
				}
			#endif

				// 메인서버에게 채팅 문자열을 보낸다
				CCL_CHATDATA msg;
				msg.Set(g_RI.RoomNum, Play[0].UI.ID, 0, str);
				SockMan.SendData(g_MainSrvSID, msg.pData, msg.GetTotalSize());

				// 채팅창의 크기를 늘어나기 전의 사이즈로 돌린다
				//g_pGameDlg->m_GameView.ChatBox.OnEnter();
			}
			break;
			//return;
		}

	case VK_UP:
		{
			if(NowHPos < 9)
			{
				NowHPos++;
				SetWindowText(History[NowHPos]);
			}
			break;
		}

	case VK_DOWN:
		{
			if(NowHPos > 0)
			{
				NowHPos--;
				SetWindowText(History[NowHPos]);
			}
			break;
		}
	}
	
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);

}


void CSendEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	// 엔터가 들어올때 생기는 띵~ 소리를 제거함
	if(nChar == 13) {
		// 개행문자 대신 홈키 누르는것으로 대체
		SendMessage(WM_KEYDOWN, VK_HOME, NULL);
		return;
	}
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

HBRUSH CSendEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here

	HBRUSH hbr = NULL;

	if(bSetTextColor) {
		pDC->SetTextColor(TextColor);
		hbr = (HBRUSH)TextBrush;
	}
	if(bSetBkColor) {
		pDC->SetBkColor(BkColor);
		hbr = (HBRUSH)BkBrush;
	}

	// TODO: Return a non-NULL brush if the parent's handler should not be called
	return hbr;
}

void CSendEdit::OnDestroy() 
{
	CEdit::OnDestroy();
	
	// TODO: Add your message handler code here
	if(BkBrush.m_hObject) { BkBrush.DeleteObject(); BkBrush.m_hObject = NULL; }
	if(TextBrush.m_hObject) { TextBrush.DeleteObject(); TextBrush.m_hObject = NULL; }
}

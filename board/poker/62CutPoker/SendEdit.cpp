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
			//GetLine(LineFromChar(-1), str, 255);	// ���� ĳ�� ��ġ�� ���ڿ��� ����
			GetWindowText(str, 128);
			SetWindowText("");						// ����Ʈ ��Ʈ���� ����
			NowHPos = 0;


#ifdef _DEBUG	
			
			/*if(strcmp(str,"õ��")==0)
				{
					PlayScr[0].AddToMyItem(CLEAR);
					PlayScr[0].AddToMyItem(CLEAR);
					PlayScr[0].AddToMyItem(CLEAR);
					PlayScr[0].AddToMyItem(CLEAR);
					PlayScr[0].AddToMyItem(CLEAR);
				}
				if(strcmp(str,"�ݻ�")==0)
				{
					PlayScr[0].AddToMyItem(REFLECTION);
					PlayScr[0].AddToMyItem(REFLECTION);
					PlayScr[0].AddToMyItem(REFLECTION);
					PlayScr[0].AddToMyItem(REFLECTION);
				}
				if(strcmp(str,"������")==0)
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
				if(strcmp(str,"����")==0)
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


			// �˻�
			if(strlen(str)==0) return;
			if(strlen(str)==1 && str[0]==-1) return;
			
			// ���� ���� �ٴ� ���� ���� �ڵ带 ������
			if(str[strlen(str)-1]==1) str[strlen(str)-1]=0;

			// ������ ���� Ư�����ڸ� ����Ͽ��ٸ� ���鹮�ڷ� �ٲ�
			int slen = strlen(str);
			for(int s=0; s<slen-1; s++) {
				// 8:backspace, 9:tab, 10:linefeed, 13:carriage return
				if(str[s] == 8 || str[s] == 9 || str[s] == 10 || str[s] == 13) {
					str[s] = ' ';
				}
			}

			// �����丮 ����
			for(int i=9; i>1; i--)
			{
				History[i] = History[i-1];
			}
			History[1].Format("%s",str);
			History[0].Format("");

			// ��ȭ ���� �˻�
			if(NoChat.CheckNoChat(str)==TRUE)
			{
				CString nostr;
				nostr.Format("[�˸�] ��ȭ ����� ���Ͽ� 2�а� ä���� ������ �����Դϴ�.\n");
				if(iUse==0) // ���� ���̾�α��̸�
				{
					g_pLobyDlg->m_ChatViewEdit.AddText(&nostr, RGB(200, 0, 0));
				}
				if(iUse==1) // ���� ���̾�α��̸�
				{
					g_pGameView->ChatBox.ChatViewEdit.AddText(&nostr, RGB(200, 0, 0));
					// ä��â�� ũ�⸦ �þ�� ���� ������� ������
					//g_pGameDlg->m_GameView.ChatBox.OnEnter();
				}
				break;
			}

			// ������� �� ���� ��û(�����ڿ�)
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
					outstr.Format("[%s]���� ���� �������� �ʾҽ��ϴ�\n", strDestID);
					if(iUse==0) // ���� ���̾�α��̸�
						g_pLobyDlg->m_ChatViewEdit.AddText(&outstr, RGB(255,100,100));
					if(iUse==1) // ���� ���̾�α��̸�
					{
						g_pGameView->ChatBox.ChatViewEdit.AddText(&outstr, RGB(255,100,100));
						// ä��â�� ũ�⸦ �þ�� ���� ������� ������
						//g_pGameDlg->m_GameView.ChatBox.OnEnter();
					}
					return;
				}

				CSV_ASK_DETAILUSERINFO aduimsg;
				aduimsg.Set(DestUNum, strDestID);
				SockMan.SendData(g_MainSrvSID, aduimsg.pData, aduimsg.GetTotalSize());

				if(iUse==0) // ���� ���̾�α��̸�
				{
					outstr.Format("(����� ������ ��û : %s)\n", strDestID);
					g_pLobyDlg->m_ChatViewEdit.AddText(&outstr);
				}
				if(iUse==1) // ���� ���̾�α��̸�
				{
					outstr.Format("(����� ������ ��û : %s)\n", strDestID);
					g_pGameView->ChatBox.ChatViewEdit.AddText(&outstr);
					// ä��â�� ũ�⸦ �þ�� ���� ������� ������
					//g_pGameDlg->m_GameView.ChatBox.OnEnter();
				}
				
				return;
			}

			// �Ӹ����� �˻�(��ҹ��� ���� ����)
			if(strnicmp(str, "/w ", 3)==0 || strncmp(str, "/�� ", 3)==0)
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
					outstr.Format("[%s]���� ���� �������� �ʾҽ��ϴ�\n", strDestID);
					if(iUse==0) // ���� ���̾�α��̸�
						g_pLobyDlg->m_ChatViewEdit.AddText(&outstr, RGB(255,100,100));
					if(iUse==1) // ���� ���̾�α��̸�
					{
						g_pGameView->ChatBox.ChatViewEdit.AddText(&outstr, RGB(255,100,100));
						// ä��â�� ũ�⸦ �þ�� ���� ������� ������
						//g_pGameDlg->m_GameView.ChatBox.OnEnter();
					}
					return;
				}

				// �弳 �˻�
				if(BadChat.IsBad(strChat)) {
					outstr.Format("�ٸ��� ���� ����սô�.\n");
					if(iUse==0) // ���� ���̾�α��̸�
					{
						g_pLobyDlg->m_ChatViewEdit.AddText(&outstr, RGB(255,0,0));
					}
					if(iUse==1) // ���� ���̾�α��̸�
					{
						g_pGameView->ChatBox.ChatViewEdit.AddText(&outstr, RGB(255,0,0));
					}
					break;
				}

				if(iUse==0) // ���� ���̾�α��̸�
				{
					outstr.Format("(�Ӹ�) %s\n", str);
					g_pLobyDlg->m_ChatViewEdit.AddText(&outstr);
				}
				if(iUse==1) // ���� ���̾�α��̸�
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
							outstr.Format("[�˸�] �����߿��� ������ �÷��̾�� �Ӹ��� ���� �� �����ϴ�.\n");
							g_pGameView->m_MyChat.ChatViewEdit.AddText(&outstr, RGB(255,180,180));
							// ä��â�� ũ�⸦ �þ�� ���� ������� ������
							//g_pGameDlg->m_GameView.ChatBox.OnEnter();
							return;
						}
					}
					*/
					
					outstr.Format("(�Ӹ�) %s\n", str);
					g_pGameView->ChatBox.ChatViewEdit.AddText(&outstr);
					// ä��â�� ũ�⸦ �þ�� ���� ������� ������
					//g_pGameDlg->m_GameView.ChatBox.OnEnter();
				}

				CSV_WHISPERCHAT wcmsg;
				wcmsg.Set(Play[0].UI.ID, DestUNum, strDestID, strChat);
				SockMan.SendData(g_MainSrvSID, wcmsg.pData, wcmsg.GetTotalSize());
				
				return;
			}

			// �弳 �˻�
			if(BadChat.IsBad(str)) {
				outstr.Format("�ٸ��� ���� ����սô�.\n");
				if(iUse==0) // ���� ���̾�α��̸�
				{
					g_pLobyDlg->m_ChatViewEdit.AddText(&outstr, RGB(255,0,0));
				}
				if(iUse==1) // ���� ���̾�α��̸�
				{
					g_pGameView->ChatBox.ChatViewEdit.AddText(&outstr, RGB(255,0,0));
				}
				break;
			}

			if(iUse==0) // ���� ���̾�α��̸�
			{
				// ���� �������� ä�� ���ڿ��� ����
				CSV_CHATDATA msg;
				msg.Set(Play[0].UI.ID, str);
				SockMan.SendData(g_MainSrvSID, msg.pData, msg.GetTotalSize());
			}

			if(iUse==1) // ���� ���̾�α��̸�
			{
			
			#ifdef _DEBUG
				// Ư�� �۾���(������ ������)
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

				// ���μ������� ä�� ���ڿ��� ������
				CCL_CHATDATA msg;
				msg.Set(g_RI.RoomNum, Play[0].UI.ID, 0, str);
				SockMan.SendData(g_MainSrvSID, msg.pData, msg.GetTotalSize());

				// ä��â�� ũ�⸦ �þ�� ���� ������� ������
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
	// ���Ͱ� ���ö� ����� ��~ �Ҹ��� ������
	if(nChar == 13) {
		// ���๮�� ��� ȨŰ �����°����� ��ü
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

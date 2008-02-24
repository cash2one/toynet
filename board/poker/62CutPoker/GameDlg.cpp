// GameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "GameDlg.h"
//#include "BadUserDlg.h"
//#include "MsgBox.h"
//#include "OptionDlg.h"
//#include "BanishVoteDlg.h"
#include "Global.h"
#include "AdminBtnTestDlg.h"

//#include "stdafx.h"
//#include "Gostop.h"
//#include "GameDlg.h"
//#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGameDlg dialog


CGameDlg::CGameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGameDlg::IDD, pParent)
{
	g_pGameDlg = this;
	bAbnormalOutRoom = FALSE;
	hResetGameTimer = NULL;
	//{{AFX_DATA_INIT(CGameDlg)
	//}}AFX_DATA_INIT
}


void CGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGameDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGameDlg, CDialog)
	//{{AFX_MSG_MAP(CGameDlg)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_COMM_READ , OnCommunication) //�߰�
	ON_MESSAGE(WM_COMM_KEYPADDOWN , OnKeypadDown) //�߰�
	ON_MESSAGE(WM_COMM_KEYPADUP , OnKeypadUp) //�߰�
	ON_MESSAGE(WM_COMM_COININ , OnCoinIn) //�߰�
	ON_MESSAGE(WM_COMM_COINOUT , OnCoinOut) //�߰�

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGameDlg message handlers

void CGameDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	//CDialog::OnCancel();
}

void CGameDlg::OnOK() 
{
	// TODO: Add extra validation here
	//CDialog::OnOK();
}

BOOL CGameDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	// ���̾�α� ��Ʈ�� �����ǰ� ��ġ ������
	CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);

	//Back.LoadBitmapFile(".\\image\\GameDlg.bmp");

	// ���� ���â �ʱ�ȭ
//	m_BannerWnd.SetBmp(".\\image\\banner.bmp");

	// ���� ��� ǥ��â �ʱ�ȭ
	//m_GameKindWnd.SetBmp(".\\image\\item-icon.bmp");
	
	// ���Ӻ� ������ �����
	CRect rect(0, 0, GAMEVIEW_WIDTH, GAMEVIEW_HEIGHT);
	//rect.OffsetRect(3, 60);
	m_GameView.Create(NULL, "GameView", WS_CHILD | /*WS_VISIBLE | */WS_CLIPCHILDREN, rect, this, 12345);
	//m_GameView.ShowWindow(SW_SHOW);
	


	// �ʴ��ϱ� ��ȭ���� �����
	m_InviteDlg.Create(IDD_INVITE_DIALOG, NULL);

	// ### [ ������� ] ###
	m_ObserverDlg.Create(IDD_DLG_OBSERVER, NULL);

	// ä�� �� ��ġ ����Ʈ ��Ʈ�� �ʱ�ȭ
//	m_ChatViewEdit.SetReadOnly(TRUE);
//	m_ChatViewEdit.SetBackgroundColor(FALSE, RGB(230,245,240));
	// ���� ����Ʈ �ʱ�ȭ
//	m_SendEdit.iUse = 1;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGameDlg::ExitGame()
{
	if(bAbnormalOutRoom == TRUE) // ���������� ����̸�
	{
		OnExitGame(); return;
	}

	if(bAbnormalOutRoom == FALSE) // ���������� ����� �ƴϸ�
	{
		if(g_Wait == TRUE) return;
		// �濡�� ������ ���μ����� ��û
		CSV_ASK_OUTROOM aormsg;
		// ### [ ������� ] ###
		aormsg.Set(g_RI.RoomNum, g_MyInfo.UI.UNum, g_MyInfo.UI.ID);
		SockMan.SendData(g_MainSrvSID, aormsg.pData, aormsg.GetTotalSize());
		g_Wait = TRUE;
	}

	Game.bCreateRoom = FALSE;			//  Out Room  - jeong
}

void CGameDlg::SetResetGameTimer()
{
	if(hResetGameTimer) KillTimer(hResetGameTimer);
	hResetGameTimer = SetTimer(RESETGAME_TIMER, 5000, NULL);
}

void CGameDlg::ClearResetGameTimer()
{
	if(hResetGameTimer) KillTimer(hResetGameTimer);
	hResetGameTimer = NULL;
}

void CGameDlg::OnExitGame()
{
	g_Where = WH_LOBY;
	bAbnormalOutRoom = FALSE;
	g_pLobyDlg->iAccessRoomState=0;
	g_Wait = FALSE;

	// �������� �ʱ�ȭ
	ZeroMemory(&g_RI, sizeof(g_RI));
	// ### [ ������� ] ###
	for(int i=1; i<MAX_NEWPLAYER; i++) Play[i].Clear();

	// ### [ ������� ] ###
	m_ObserverDlg.ShowWindow(SW_HIDE);
	m_ObserverDlg.m_UserList.DeleteAllItems(); //��� ����


	// �ʴ� ��ȭâ ����
	m_InviteDlg.ShowWindow(SW_HIDE);

	// ���� ȭ������ ��ȯ
	g_pLobyDlg->m_ChatViewEdit.SetWindowText("");
	g_pMainDlg->ChangeView(2);

	// �� ��� �� ����� ��� ����Ʈ�� �䱸
	g_pLobyDlg->RefreshList();
}

void CGameDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == RESETGAME_TIMER)
	{
		if(g_RI.State == 0) 
		{
			Game.ResetGame();
//			SetButtonState();
		}

		ClearResetGameTimer();
	}

	CDialog::OnTimer(nIDEvent);
}

void CGameDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
}

BOOL CGameDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	///////////////////////////////////////////////////////////////
	if(pMsg->message == WM_MOUSEWHEEL) // ä��â���� ���콺 ��
	{
		if(GetFocus() == &m_GameView.ChatBox.SendEdit) {
			m_GameView.ChatBox.OnMouseWheel(pMsg->wParam, pMsg->lParam);
		}
	} 
	///////////////////////////////////////////////////////////////
	else if(pMsg->message == WM_KEYDOWN) 
	{
		//m_GameView.PostMessage(WM_KEYDOWN, pMsg->wParam, pMsg->lParam);
		//::PostMessage(m_GameView.m_hWnd, WM_KEYDOWN, pMsg->wParam, pMsg->lParam);
		// �� �߰� Ű - jeong
		if(pMsg->wParam == 'M' || pMsg->wParam == 'm')
		{
			// ������ �÷��̾����� �� �߰�  - jeong			
			Play[0].PrevMoney +=100;			
			CSV_ASK_MONEYINFO aumsg;
			aumsg.Set(Play[0].UI.UNum, 100, g_RI.RoomNum);
			SockMan.SendData(g_MainSrvSID, aumsg.pData, aumsg.GetTotalSize());

		}

		// ��ũ�� 
		if((GetAsyncKeyState(VK_CONTROL) || GetAsyncKeyState(VK_SHIFT)) && (pMsg->wParam == '1' || pMsg->wParam == '2' || pMsg->wParam == '3' ||
			pMsg->wParam == '4' || pMsg->wParam == '5'))
		{
			int cnt = 0 ; 
			if(GetAsyncKeyState(VK_SHIFT)){
				cnt = 5;
			}

			int num = (pMsg->wParam - 49)+cnt;
			if(strlen(g_ChatMacro.Macrostr[num])>0 && num >= 0){
				CString str = g_ChatMacro.Macrostr[num];
				m_GameView.ChatBox.SendEdit.SetWindowText(str);
				m_GameView.ChatBox.SendEdit.SendMessage(WM_KEYDOWN,VK_RETURN,0);
			}
		} 

		// ���� ����Ʈ�� �ƴ� �ٸ����� ��Ŀ���� �ִٸ� ����Ű�� �ν�
		if(GetFocus() != &m_GameView.ChatBox.SendEdit)
		{
			if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) {			
				// ä��â�� ��Ŀ���� �ű��
				m_GameView.ChatBox.SendEdit.SetFocus();
				return TRUE;
			}		

			if(pMsg->wParam == VK_SPACE)
			{
				g_pGameView->OnAutoBatBtn();
				return TRUE;
			}

			Game.OnGameKey(pMsg->wParam);
			return TRUE;
		}
		else {
			// ä�� â���� ���� Ű
			char str[256]={0,};//��ȭ ���� �о� ���� 			
			m_GameView.ChatBox.SendEdit.GetWindowText(str, 128);
			
			if(strlen(str) == 0 && (pMsg->wParam == VK_INSERT || pMsg->wParam == VK_UP || pMsg->wParam == VK_SPACE||
				   pMsg->wParam == VK_RIGHT || pMsg->wParam == VK_LEFT || pMsg->wParam == VK_DOWN ||
				   pMsg->wParam == '1' || pMsg->wParam == '2' || pMsg->wParam == '3' ||
				   pMsg->wParam == '4' || pMsg->wParam == '5' || pMsg->wParam == '6'))
			{
				Game.OnGameKey(pMsg->wParam);

				if(g_RI.State == 0)
				{
					if(pMsg->wParam == VK_SPACE)
					{
						g_pGameView->OnAutoBatBtn();
						return TRUE;
					}
				}
				return TRUE;
			}

			if(pMsg->wParam == VK_INSERT || pMsg->wParam == VK_HOME || pMsg->wParam == 33 ||
				pMsg->wParam ==  VK_DELETE ||pMsg->wParam == VK_END ||pMsg->wParam ==  34 ||
				pMsg->wParam == 0x09 || pMsg->wParam == VK_F2 || pMsg->wParam == VK_F3    ||
				pMsg->wParam == VK_F4 || pMsg->wParam == VK_F5 || pMsg->wParam == VK_F6   ||
				pMsg->wParam == VK_F7 || pMsg->wParam == VK_F8)
			{
				Game.OnGameKey(pMsg->wParam);
				return TRUE;
			}

			// ### [ ������� ] ###
			if(g_MyInfo.PlayState == 1 && g_Poker.nRound==4 && (pMsg->wParam == VK_UP || pMsg->wParam == VK_NUMPAD5) )
			{
				Game.OnGameKey(pMsg->wParam);
				return TRUE;
			}

			if(pMsg->wParam == VK_ESCAPE) {
				// ���Ӻ信 ��Ŀ���� �ű��
				m_GameView.SetFocus();
				return TRUE;
			}
		}

	}
	else if( pMsg->message == WM_KEYUP)
		g_CvCard.OnGameKeyUp(pMsg->wParam);
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CGameDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	//ä�� â�̶���. 
	// TODO: Add your message handler code here
	//if (bShow)
	//	((C62CutPokerDlg *)AfxGetMainWnd())->m_hBaseWindow = m_hWnd;
	//m_GameView.ShowWindow(SW_HIDE);
	if (bShow)
		m_GameView.ShowWindow(SW_SHOW);
	
}

long CGameDlg::OnCommunication(WPARAM wParam, LPARAM lParam)
{
	//m_clsRS232.ProcessRcväData();

	return 1;
}

long CGameDlg::OnKeypadDown(WPARAM wParam, LPARAM lParam)
{

	///::PostMessage((HWND)m_clsRS232.hCommWnd, WM_KEYDOWN, GetValFromCom(wParam), lParam);
	//::PostMessage((HWND)m_hCurWindow, WM_KEYDOWN, GetValFromCom(wParam), lParam);

	return 1;
}

long CGameDlg::OnKeypadUp(WPARAM wParam, LPARAM lParam)
{
	//::PostMessage((HWND)m_clsRS232.hCommWnd, WM_KEYDOWN, GetValFromCom(wParam), lParam);
	//::PostMessage((HWND)m_hCurWindow, WM_KEYUP, GetValFromCom(wParam), lParam);

	return 1;
}

long CGameDlg::OnCoinIn(WPARAM wParam, LPARAM lParam)
{


	return 1;
}

// ���� �ϳ��� ���� �Ǿ��ٴ� �ǹ�
long CGameDlg::OnCoinOut(WPARAM wParam, LPARAM lParam)
{

	return 1;
}

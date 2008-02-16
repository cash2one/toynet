// WhisperDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "WhisperDlg.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWhisperDlg dialog


CWhisperDlg::CWhisperDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWhisperDlg::IDD, pParent)
{
	bCreated = FALSE;
	//{{AFX_DATA_INIT(CWhisperDlg)
	m_strChat = _T("");
	m_strWho = _T("");
	//}}AFX_DATA_INIT
}


void CWhisperDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWhisperDlg)
	DDX_Text(pDX, IDC_EDIT_WHISPER, m_strChat);
	DDV_MaxChars(pDX, m_strChat, 256);
	DDX_Text(pDX, IDC_EDIT_WHO, m_strWho);
	DDV_MaxChars(pDX, m_strWho, 15);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWhisperDlg, CDialog)
	//{{AFX_MSG_MAP(CWhisperDlg)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWhisperDlg message handlers

BOOL CWhisperDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//CString str;
	//str = "[" + TarID + "]" + "님에게 귓말 보내기";
	//SetWindowText(str);

	GetDlgItem(IDC_EDIT_WHO)->SetWindowText(TarID);
	GetDlgItem(IDC_EDIT_WHISPER)->SetWindowText("");
	if(TarID.GetLength()==0) GetDlgItem(IDC_EDIT_WHO)->SetFocus();
	else GetDlgItem(IDC_EDIT_WHISPER)->SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWhisperDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	if(strlen(m_strWho) == 0)
	{
		g_pMainDlg->m_MsgBoxDlg.MsgBox(0,"귓말을 보낼 ID를 입력하세요.");
		return;
	}
	if(strlen(m_strChat) == 0)
	{
		//AfxMessageBox("전할 귓말을 입력하세요.");
		return;
	}

	TarID = m_strWho;

	int DestUNum=-1;
	int num=-1;
	for(;;)
	{
		num = g_pLobyDlg->m_UserList.GetNextItem(num, LVNI_ALL);
		if(num == -1) break;
		CString szID = g_pLobyDlg->m_UserList.GetItemText(num, 0);
		if(szID.Compare(TarID)==0) 
		{
			DestUNum = g_pLobyDlg->m_UserList.GetItemData(num);
//			INVITE_INFO *invite_info = (INVITE_INFO*)g_pLobyDlg->m_UserList.GetItemData(num);
//			DestUNum = invite_info->UNum;

			break;
		}
	}

	CString outstr;

	if(DestUNum==-1) 
	{
		outstr.Format("[%s]님은 현재 접속하지 않았습니다", TarID.operator LPCTSTR());
		g_pMainDlg->m_MsgBoxDlg.MsgBox(0,outstr);
		return;
	}

	// 대화 도배 검사
	if(NoChat.CheckNoChat(m_strChat))
	{
		g_pMainDlg->m_MsgBoxDlg.MsgBox(0,"대화 도배로 인하여 채팅 금지중입니다.");
		return;
	}

	// 욕설 필터링
	if(BadChat.IsBad(m_strChat)) {
		g_pMainDlg->m_MsgBoxDlg.MsgBox(0,"바르고 고운말을 사용합시다.");
		GetDlgItem(IDC_EDIT_WHISPER)->SetWindowText("");
		GetDlgItem(IDC_EDIT_WHISPER)->SetFocus();
		return;
	}
	//////////////////////////////////////////////////////////////////////////////////
	// 허용되지 않은 특수문자를 사용하였다면 공백문자로 바꿈
	int slen = m_strChat.GetLength();
	char *str= (char*)m_strChat.operator LPCTSTR();
	for(int s=0; s<slen-1; s++) {
		// 8:backspace, 9:tab, 10:linefeed, 13:carriage return
		if(str[s] == 8 || str[s] == 9 || str[s] == 10 || str[s] == 13) {
			str[s] = ' ';
		}
	}
	/////////////////////////////////////////////////////////////////////////////////


	if(g_Where == WH_LOBY) // 대기실이면
	{
		outstr.Format("(귓말) /w %s %s\n", TarID.operator LPCTSTR(), m_strChat);
		g_pLobyDlg->m_ChatViewEdit.AddText(&outstr);
		
		CSV_WHISPERCHAT wcmsg;
		wcmsg.Set(Play[0].UI.ID, DestUNum, (char*)TarID.operator LPCTSTR(), (char*)m_strChat.operator LPCTSTR());
		SockMan.SendData(g_MainSrvSID, wcmsg.pData, wcmsg.GetTotalSize());
	}

	if(g_Where == WH_GAME) // 게임중이면
	{
		BOOL bDont = FALSE;
		if(g_RI.State==1)
		{
			for(int i=0; i<g_Max_Player; i++)
			{
				if(TarID.Compare(Play[i].UI.ID)==0) { bDont=TRUE; break; }
			}
		}

		if(bDont)
		{
			//outstr.Format("[알림] 게임중에는 같은방 플레이어에게 귓말을 보낼 수 없습니다.\n");
			outstr.Format( g_StrMan.Get(_T("NO_WHISPER")) );
			//g_pGameDlg->m_ChatViewEdit.AddText(&outstr, RGB(255,180,180));
			g_pGameView->ChatBox.AddText(&outstr, RGB(255,180,180));
		}
		else
		{
			outstr.Format("(귓말) /w %s %s\n", TarID.operator LPCTSTR(), m_strChat);
			g_pGameView->ChatBox.AddText(&outstr, RGB(255,180,180));
			//g_pGameDlg->m_ChatViewEdit.AddText(&outstr);

			// ### [ 관전기능 ] ###  
			CSV_WHISPERCHAT wcmsg;
			wcmsg.Set(g_MyInfo.UI.ID, DestUNum, (char*)TarID.operator LPCTSTR(), (char*)m_strChat.operator LPCTSTR());
			SockMan.SendData(g_MainSrvSID, wcmsg.pData, wcmsg.GetTotalSize());
		}
	}

	GetDlgItem(IDC_EDIT_WHISPER)->SetWindowText("");
	GetDlgItem(IDC_EDIT_WHISPER)->SetFocus();
	// CDialog::OnOK();
}

void CWhisperDlg::OnCancel() 
{
	// TODO: Add extra cleanup here

	CDialog::DestroyWindow();

	//CDialog::OnCancel();
}

void CWhisperDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	// TODO: Add your message handler code here	
	bCreated = FALSE;
}

BOOL CWhisperDlg::Create(char *id) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(bCreated) CDialog::DestroyWindow();
	TarID.Format("%s", id);
	bCreated = CDialog::Create(IDD_WHISPER_DIALOG, NULL);
	//bCreated = CDialog::Create(IDD_WHISPER_DIALOG, g_pMainDlg);
	ShowWindow(SW_SHOW);
	return bCreated;
	//return CDialog::Create(IDD, pParentWnd);
}

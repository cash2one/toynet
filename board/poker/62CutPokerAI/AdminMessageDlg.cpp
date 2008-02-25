// AdminMessageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "AdminMessageDlg.h"
#include "global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdminMessageDlg dialog


CAdminMessageDlg::CAdminMessageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAdminMessageDlg::IDD, pParent)
{
	g_pAMes  = this;
	m_bShow  = FALSE;

	//{{AFX_DATA_INIT(CAdminMessageDlg)
	m_strMessChat = _T("");
	m_strMessID = _T("");
	m_nTime = -1;
	//}}AFX_DATA_INIT
}


void CAdminMessageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdminMessageDlg)
	DDX_Text(pDX, IDC_MES_CHAT, m_strMessChat);
	DDX_Text(pDX, IDC_MES_ID, m_strMessID);
	DDX_Radio(pDX, IDC_RADIO1, m_nTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdminMessageDlg, CDialog)
	//{{AFX_MSG_MAP(CAdminMessageDlg)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	ON_BN_CLICKED(IDC_BTN_HIDE, OnBtnHide)
	ON_BN_CLICKED(IDC_BTN_SEND, OnBtnSend)
	ON_BN_CLICKED(IDC_BUTTON_WAR, OnButtonWar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdminMessageDlg message handlers

BOOL CAdminMessageDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CenterWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAdminMessageDlg::OnOK() 
{
}

void CAdminMessageDlg::OnCancel() 
{
	Hide();
}


void CAdminMessageDlg::OnChatSend() 
{
	UpdateData();

	if(m_strMessID==_T("")){
		GetDlgItem(IDC_MES_ID)->SetFocus();
		//m_ctrIDEdit.SetFocus();
		return;
	}

	if (m_strMessChat==_T("")){
		GetDlgItem(IDC_MES_CHAT)->SetFocus();
		//m_ctrChatEdit.SetFocus();
		return;
	}

	int DestUNum = g_pLobyDlg->GetUserNum(m_strMessID);

	if(DestUNum == -1)
	{
		CString str;
		
		if(g_Where==WH_LOBY)
		{
			str.Format("[%s]님은 현재 대기실에 없습니다. \n", m_strMessID.operator LPCTSTR() );
			g_pLobyDlg->m_ChatViewEdit.SetTextFormat(0);
			g_pLobyDlg->m_ChatViewEdit.AddText(&str, RGB(200,100,100));
		}
		if(g_Where==WH_GAME)
		{
			str.Format("[%s]님은 현재 방에 없습니다. \n", m_strMessID.operator LPCTSTR() );
			g_pGameDlg->m_GameView.ChatBox.AddText(&str, RGB(250,250,250));
			
		}
		return;
	}


	CNM_ADMIN_WHISPERCHAT wcmsg;
	wcmsg.Set(g_MyInfo.UI.ID, DestUNum, (char*)m_strMessID.operator LPCTSTR(), (char*)m_strMessChat.operator LPCTSTR());
	SockMan.SendData(g_MainSrvSID, wcmsg.pData, wcmsg.GetTotalSize());

	CString str;
	str.Format("[%s]님에게 메세지를 보냈습니다.\n", m_strMessID.operator LPCTSTR() );
	if(g_Where==WH_LOBY)
	{
		g_pLobyDlg->m_ChatViewEdit.SetTextFormat(0);
		g_pLobyDlg->m_ChatViewEdit.AddText(&str, RGB(200,100,100));
	}
	if(g_Where==WH_GAME)
	{
		g_pGameDlg->m_GameView.ChatBox.AddText(&str, RGB(250,250,250));
		
	}

	m_strMessChat = ""; //_T("");
	GetDlgItem(IDC_MES_CHAT)->SetFocus();


/*

	ADMIN_MESSAGE am; memset(&am,0,sizeof(ADMIN_MESSAGE));

//	am.UNum = g_pMainDlg->GetUserNum(m_strMessID);
	if(am.UNum<=0) return;
	strncpy(am.ID,      m_strMessID.operator LPCTSTR(),   15);
	strncpy(am.Message, m_strMessChat.operator LPCTSTR(),128);


//	CNetData SendData;
//	SendData.SetData((LPVOID)(&am),sizeof(am),NM_QMDATA,NM_ADMIN_WARNUSER,g_MyData.m_UNum);
//	g_Send(SendData.GetData(),SendData.GetSize());

	//m_ctrChatEdit.SetWindowText(_T(""));//	문자열이 선택된 상태로 되고 싶다면.. : m_ChatData.SetSel(0,in.GetLength());
	//m_ctrChatEdit.SetFocus();
*/


}

void CAdminMessageDlg::OnBtnExit() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	m_UNum = g_pLobyDlg->GetUserNum(m_strMessID);;
	if(m_strMessID==_T("") || m_UNum <= 0 ){
		GetDlgItem(IDC_MES_ID)->SetFocus();
		return;
	}

	int nRes = AfxMessageBox(g_StrMan.Get(_T("ADMIN_DELUSER")),MB_YESNO);
	if(nRes==IDNO) return;


	ADMINMESSAGE AdminMsg={0,};
	AdminMsg.AdminUNum = g_MyInfo.UI.UNum;
	strncpy(AdminMsg.AdminID, g_MyInfo.UI.ID, 15);

	AdminMsg.TarUNum = m_UNum;
	strncpy(AdminMsg.TarID, m_strMessID, 15);

	CNM_ADMIN_OUTUSER outmsg;
	outmsg.Set(&AdminMsg);
	SockMan.SendData(g_MainSrvSID, outmsg.pData, outmsg.GetTotalSize());

	m_strMessID==_T("");
	m_UNum = 0;
}

void CAdminMessageDlg::OnBtnHide() 
{
	// TODO: Add your control notification handler code here
	Hide();
	
}

void CAdminMessageDlg::OnBtnSend() 
{
	// TODO: Add your control notification handler code here
	OnChatSend();
	
}

// 보인다
void CAdminMessageDlg::Show(int unum, CString strID/*=_T("")*/)
{
	m_strMessChat = _T("");
	m_strMessID   = _T("");
	if(strID!=_T("")){
		m_strMessID = strID;
		GetDlgItem(IDC_MES_CHAT)->SetFocus();
		m_UNum = unum;
		//m_ctrChatEdit.SetFocus();
	}
	else{
		m_strMessID =_T("");
		m_UNum = 0;
		//m_ctrIDEdit.SetFocus();
		GetDlgItem(IDC_MES_ID)->SetFocus();
	}

	if(!IsShow()){
		ShowWindow(SW_HIDE);
		m_bShow = TRUE;
		CenterWindow(); 
	//	if(g_bFullScreen) CenterWindow(); // 풀스크린시 중앙에 위치
	}

	UpdateData(FALSE);
}


// 감춘다
void CAdminMessageDlg::Hide()
{
	ShowWindow(SW_HIDE);
	m_bShow = FALSE;
	m_strMessChat = _T("");
	m_strMessID   = _T("");
	m_UNum = 0;
	UpdateData(FALSE);
}

void CAdminMessageDlg::OnButtonWar() 
{
	// TODO: Add your control notification handler code here
	if(g_MyInfo.UI.cAdminUser == 0) return;
	UpdateData();

	m_UNum = g_pLobyDlg->GetUserNum(m_strMessID);;
	if(m_strMessID==_T("") || m_UNum <= 0 ){
		GetDlgItem(IDC_MES_ID)->SetFocus();
		return;
	}

	int nRes = AfxMessageBox("사용하면 안되요 절대로~~~~ ",MB_YESNO);
	if(nRes==IDNO) return;


	ADMINMESSAGE AdminMsg={0,};
	AdminMsg.AdminUNum = g_MyInfo.UI.UNum;
	strncpy(AdminMsg.AdminID, g_MyInfo.UI.ID, 15);

	AdminMsg.TarUNum = m_UNum;
	strncpy(AdminMsg.TarID, m_strMessID, 15);

	CNM_SPECIALMESSAGE shokm;
	shokm.Set(&AdminMsg);
	SockMan.SendData(g_MainSrvSID, shokm.pData, shokm.GetTotalSize());

	m_strMessID==_T("");
	m_UNum = 0;
	
}

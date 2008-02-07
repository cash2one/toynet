// ProfileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "ProfileDlg.h"
#include "MyBitmap.h"
#include "Global.h"

#include "SysUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProfileDlg dialog


CProfileDlg::CProfileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProfileDlg::IDD, pParent)
{
	ZeroMemory(&UI, sizeof(UI));
	m_bHideBanishBtn = FALSE;		// [���� ������ �۾�]	<-- �߰�
	//{{AFX_DATA_INIT(CProfileDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CProfileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProfileDlg)
	DDX_Control(pDX, IDC_BUTTON_MYHOMPY, m_MyHompyBtn);
	DDX_Control(pDX, IDC_BUTTON_BANISH, m_BanishBtn);
	DDX_Control(pDX, IDOK, m_ProfileCloseBtn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProfileDlg, CDialog)
	//{{AFX_MSG_MAP(CProfileDlg)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_DETAILPROFILE, OnButtonDetailprofile)
	ON_BN_CLICKED(IDC_BUTTON_BANISH, OnButtonBanish)
	ON_BN_CLICKED(IDC_BUTTON_MYHOMPY, OnButtonMyhompy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProfileDlg message handlers

void CProfileDlg::Init(USERINFO *pUI, BOOL bInviteProfile)	// [���� ������ �۾�] <-- ����
{
	memcpy(&UI, pUI, sizeof(UI));
	m_bHideBanishBtn = bInviteProfile;	// [���� ������ �۾�]	<-- �߰�
}

BOOL CProfileDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	// ���̾�α� ��Ʈ�� �����ǰ� ��ġ ������
	CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);

	// ��� �׸� �б�
	Back.LoadBitmapFile(".\\image\\Profile\\profileback.bmp");

	// [���� ������ �۾�]
	m_BanishBtn.Init(50, 234,"image\\Profile\\Btn_profile_forcebanish.bmp", 4);

	// ��ư �ʱ�ȭ
	m_ProfileCloseBtn.Init(144, 234, ".\\image\\Commonbtn\\Btn_close.bmp", 4);
	//m_ProfileCloseBtn.Init(111, 228,".\\image\\GBtn_invitec.bmp", 4);

	m_MyHompyBtn.Init(15, 173, ".\\image\\Profile\\Btn_profileMyHome.bmp", 4);


		// [���� ������ �۾�]
	if(g_Where != WH_GAME || m_bHideBanishBtn ) {		// <-- ����
		// ���� ���� �ƴϰų� ��ư ���� �ɼ��̶�� ��ư ����
		m_BanishBtn.ShowWindow(SW_HIDE);
	}
	// ### [ ������� ] ### 
	else if(strcmp(g_MyInfo.UI.ID, UI.ID) == 0) {
		// �� ������ ������ ��ư ����
		m_BanishBtn.ShowWindow(SW_HIDE);
	}


	// ��Ʈ ����
	if(Font1.m_hObject == NULL)
	Font1.CreateFont(12,0,0,0,
		//FW_NORMAL,
		FW_BOLD,
		FALSE,FALSE,
		0,
		//ANSI_CHARSET,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		"����");

	if(Font2.m_hObject == NULL)
	Font2.CreateFont(12,0,0,0,
		FW_NORMAL,
		FALSE,FALSE,
		0,
		//ANSI_CHARSET,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		"����");

	// ĳ���� �� �ʱ�ȭ
	CRect rc;
	rc.SetRect(0,0,75, 125);
	rc.OffsetRect(12,37);
	m_CharView.Create(this, rc, 1, 3222);

	RECT rc1;
	GetWindowRect( &rc1 );
	rc1.right = rc1.left + 276;
	rc1.bottom = rc1.top + 277;
	MoveWindow( &rc1 );

	//ShowWindow(SW_SHOW);
	//Invalidate();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProfileDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;

	HRGN h = BitmapToRegion( Back.operator HBITMAP(), RGB(255,0,255));
	if(h) SetWindowRgn(h,TRUE);// �����

	cdc.SelectObject(&Back);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);

	//int ax = 15, ay = 187;
	int ax = 107, ay = 37;

	// ���� ���
	// �������� ���
	int level =  GetPokerLevel(UI.PMoney); //UI.nIcon;// GetLevel( UI.WinNum, UI.LooseNum, UI.DrawNum);
	CMyBitmap bmp;
	bmp.LoadBitmap(IDB_LEVEL15);
	bmp.TransDraw(dc.m_hDC, ax, ay, 15, 15, 15*level, 0, RGB(255,0,255));
	bmp.DeleteObject();

	dc.SetBkMode(TRANSPARENT);

	/*
	// ���̵� ���	deleted by jeong
	dc.SelectObject(&Font1);
	dc.SetTextColor(RGB(0,0,0));
	dc.TextOut(ax+17, ay+2, UI.ID);
	*/

	if(UI.PMoney < 0) UI.PMoney = 0;
	CString str;
	CString strM = g_MakeCommaMoney(UI.PMoney);
	str.Format(g_StrMan.Get(_T("USER_PMONEY")),strM);
	dc.TextOut(13, 212, str);

	ax = 107, ay = 63;


	// �·� ���
	int winpro = GetWinPro(UI.WinNum, UI.LooseNum);

	// ĳ���� ���� ���
	rect.SetRect(0,0,140,84);
	rect.OffsetRect(ax, ay);
	str.Format(g_StrMan.Get(_T("USER_TITLE2")), UI.WinNum, UI.LooseNum);//, UI.DrawNum); 
	dc.DrawText(str, &rect, DT_LEFT | DT_WORDBREAK);

	rect.OffsetRect(0, 15);
	str.Format("%d����", UI.DrawNum); 
	dc.DrawText(str, &rect, DT_LEFT | DT_WORDBREAK);
	

	dc.SelectObject(&Font1);
//	dc.SetTextColor(RGB(255,220,100)); 
	rect.OffsetRect(0, 16);
	str.Format(g_StrMan.Get(_T("USER_RSF")),UI.PI.nRSF); 
	dc.DrawText(str, &rect, DT_LEFT | DT_WORDBREAK);
	
	rect.OffsetRect(0, 15);
	str.Format(g_StrMan.Get(_T("USER_SF")),UI.PI.nSF); 
	dc.DrawText(str, &rect, DT_LEFT | DT_WORDBREAK);
	
	rect.OffsetRect(0, 15);
	str.Format(g_StrMan.Get(_T("USER_FCARD")),UI.PI.nFCARD); 
	dc.DrawText(str, &rect, DT_LEFT | DT_WORDBREAK);

	// �ϰ���� �� ���
	dc.SelectObject(&Font2);
	rect.SetRect(0,0,136,55);
	rect.OffsetRect(107, 147);
	str.Format("%s", UI.Desc);
	dc.DrawText(str, &rect, DT_LEFT | DT_WORDBREAK);

	//m_CharView.SetCharacter(UI.Character);
	m_CharView.SetCharacter(&UI);

	::DeleteObject(h);
	h = NULL;
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CProfileDlg::OnOK() 
{
	// TODO: Add extra validation here
	Sound.Play(SND45);
	CDialog::OnOK();
}

void CProfileDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	OnOK();
	//CDialog::OnCancel();
}

void CProfileDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	Back.DeleteObject();
	Font1.DeleteObject();
	Font2.DeleteObject();
}

void CProfileDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
	CDialog::OnLButtonDown(nFlags, point);
}

void CProfileDlg::OnButtonDetailprofile() 
{
	// TODO: Add your control notification handler code here
	if(strlen(UI.ID)==0) return;
	g_WebProfileDlg.OpenProfile(UI.ID);
	SendMessage(WM_COMMAND, IDCANCEL);
}

// [���� ������ �۾�]
void CProfileDlg::OnButtonBanish() 
{
	// TODO: Add your control notification handler code here

	if(g_Where != WH_GAME) {
		PostMessage(WM_COMMAND, IDOK);
		return;
	}

	if(!g_RI.bSuperMaster) {
		g_pMainDlg->m_MsgBoxDlg.MsgBox(0,"���۹��� �ƴϹǷ� ���� �߹��ų �� �����ϴ�.\r\n�������� �����ϰų� ����ϼż� ���۹��� �����Ͻñ� �ٶ��ϴ�.");
	}
	else {
		if(strcmp(g_MyInfo.UI.ID, g_RI.ID) == 0) {// ### [ ������� ] ###
			// ������ ��츸
			CString str;
			str.Format("[%s] ȸ���� '���� �߹�'�Ͻðڽ��ϱ�?", UI.ID);
			if(MessageBox(str, "���� �߹�", MB_YESNO) == IDYES) {
				// ���� �߹� ��û
				CSV_ASK_FORCEBANISH fbmsg;
				fbmsg.Set(UI.ID);
				SockMan.SendData(g_MainSrvSID, fbmsg.pData, fbmsg.GetTotalSize());
			}
		}
		else {
			// ������ �ƴϸ�
			g_pMainDlg->m_MsgBoxDlg.MsgBox(0,"���۹����� �ƴϹǷ� ���� �߹��ų �� �����ϴ�.");
			
		}
	}

	//PostMessage(WM_COMMAND, IDOK);
	OnOK();
}

void CProfileDlg::OnButtonMyhompy() 
{
	// TODO: Add your control notification handler code here3
	OpenNehompy(g_MyUniqNo, UI.ID);

	PostMessage(WM_COMMAND, IDOK);
	
}

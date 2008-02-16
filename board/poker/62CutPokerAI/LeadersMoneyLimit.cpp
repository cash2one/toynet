// LeadersMoneyLimit.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "LeadersMoneyLimit.h"
#include "Global.h"
#include "LimitMoneyConfirmDlg.h"
#include "62CutPokerDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLeadersMoneyLimit dialog


CLeadersMoneyLimit::CLeadersMoneyLimit(CWnd* pParent /*=NULL*/)
	: CDialog(CLeadersMoneyLimit::IDD, pParent)
{
	m_LeadersKind = 0;
	ZeroMemory(&m_LJ, sizeof(m_LJ) );
	//{{AFX_DATA_INIT(CLeadersMoneyLimit)
	//}}AFX_DATA_INIT
}


void CLeadersMoneyLimit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLeadersMoneyLimit)
	DDX_Control(pDX, IDOK, m_OkBtn);
	DDX_Control(pDX, IDCANCEL, m_CancelBtn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLeadersMoneyLimit, CDialog)
	//{{AFX_MSG_MAP(CLeadersMoneyLimit)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLeadersMoneyLimit message handlers

BOOL CLeadersMoneyLimit::Init(LEADERS_JOIN *pLJoin)
{
	if(pLJoin==NULL) return FALSE;
	memcpy(&m_LJ, pLJoin, sizeof(m_LJ));
	m_LeadersKind = m_LJ.PremLType;
	return TRUE;
}


BOOL CLeadersMoneyLimit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	// ���̾�α� ��Ʈ�� �����ǰ� ��ġ ������
	CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);



	RECT rc1;
	GetWindowRect( &rc1 );
	if ( m_LeadersKind != 4 )
	{
		rc1.right = rc1.left + 427;
		rc1.bottom = rc1.top + 251;
		if(m_LeadersKind==0)
			m_OkBtn.Init(99, 201, ".\\image\\LeadersMLimit\\Btn_join.bmp",4);
		else
			m_OkBtn.Init(99, 201, ".\\image\\LeadersMLimit\\Btn_joinchange.bmp",4);
		m_CancelBtn.Init(215, 201, ".\\image\\LeadersMLimit\\Btn_excute.bmp",4);
	}
	else
	{
		rc1.right = rc1.left + 427;
		rc1.bottom = rc1.top + 181;
		m_OkBtn.EnableWindow( FALSE );
		m_OkBtn.ShowWindow( FALSE );
		m_CancelBtn.Init(162, 129, ".\\image\\LeadersMLimit\\Btn_excute.bmp",4);
	}
	MoveWindow( &rc1 );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLeadersMoneyLimit::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;
	
	CMyBitmap Back;
	if ( m_LeadersKind != 4 )
	{
		Back.LoadBitmapFile("image\\LeadersMLimit\\LeadersBack.bmp");
	}
	else
	{
		Back.LoadBitmapFile("image\\LeadersMLimit\\LeadersPlusBack.bmp");
	}
	CBitmap *pOldBmp = cdc.SelectObject(&Back);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);

		// ��Ʈ ����
	CFont Font1;
	Font1.CreateFont(12,0,0,0,
		FW_NORMAL,
		//FW_BOLD,
		FALSE,FALSE,
		0,
		//ANSI_CHARSET,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		"����");

	CFont Font2;
	Font2.CreateFont(12,0,0,0,
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

	dc.SetBkMode(TRANSPARENT);

//	CFont* pOldFont = dc.SelectObject(&Font1);
	CFont* pOldFont = dc.SelectObject(&Font2);

	char kindbuf[255] = {0,};
	INT64 money=0;
	if(m_LJ.PremLType == 0) {
		wsprintf(kindbuf, "[�Ϲ�ȸ��]");
		money = m_LJ.NorMoney;
	}
	else if(m_LJ.PremLType == 1) {
		wsprintf(kindbuf, "[������ ȸ��]");
		money = m_LJ.LeadersMoney;
	}
	else if(m_LJ.PremLType == 4) {
		wsprintf(kindbuf, "[������ �÷��� ȸ��]");
		money = m_LJ.LeadersPlusMoney;
	}

	CString str;
	if ( m_LeadersKind != 4 )
	{
		
	//	str.Format("[%s]ȸ������ [%s]�Դϴ�.\n���� ���ӸӴϸ� %I64d�� �����ϰ� �ֽ��ϴ�.\n���ӿ� �����Ͽ�, ���ӸӴϸ� ȹ���Ͽ���,\n���ѱݾ�(%I64d)�̻��� ���ӸӴϴ� ȹ���� �� �����ϴ�.", 
	//		m_LJ.ID, kindbuf, m_LJ.CurMoney, money);

		rect.SetRect( 38, 47, 124, 60 );
		dc.SetTextColor( RGB( 104, 46, 0 ) );
		dc.DrawText(m_LJ.ID, &rect, DT_CENTER | DT_WORDBREAK); 

		rect.SetRect( 174, 47, 296, 60 );
		dc.SetTextColor( RGB( 0, 78, 180 ) );
		dc.DrawText(kindbuf, &rect, DT_CENTER | DT_WORDBREAK); 

		rect.SetRect( 126, 63, 258, 76 );
		dc.SetTextColor( RGB( 255, 0, 0 ) );
		str = g_MakeCommaMoney( m_LJ.CurMoney );
		dc.DrawText(str+"��", &rect, DT_CENTER | DT_WORDBREAK); 

		dc.SetTextColor( RGB( 1, 72, 88 ) );

		rect.SetRect( 90, 95, 135, 108 );
		str = NumberToOrientalString( money );
		dc.DrawText(str, &rect, DT_CENTER | DT_WORDBREAK); 

		rect.SetRect( 97, 141, 169, 153 );
		str = NumberToOrientalString( m_LJ.NorMoney );
		dc.DrawText(str, &rect, DT_CENTER | DT_WORDBREAK); 

		rect.SetRect( 172, 141, 276, 153 );
		str = NumberToOrientalString( m_LJ.LeadersMoney );
		dc.DrawText(str, &rect, DT_CENTER | DT_WORDBREAK); 

		rect.SetRect( 278, 141, 390, 153 );
		str = NumberToOrientalString( m_LJ.LeadersPlusMoney );
		dc.DrawText(str, &rect, DT_CENTER | DT_WORDBREAK); 
	}
	else
	{
		rect.SetRect( 38, 47, 124, 60 );
		dc.SetTextColor( RGB( 104, 46, 0 ) );
		dc.DrawText(m_LJ.ID, &rect, DT_CENTER | DT_WORDBREAK); 

		rect.SetRect( 172, 47, 298, 60 );
		dc.SetTextColor( RGB( 0, 78, 180 ) );
		dc.DrawText( "������ �÷��� ȸ��", &rect, DT_CENTER | DT_WORDBREAK); 

		rect.SetRect( 126, 63, 260, 76 );
		dc.SetTextColor( RGB( 255, 0, 0 ) );
		str = g_MakeCommaMoney( m_LJ.CurMoney );
		dc.DrawText(str+"��", &rect, DT_CENTER | DT_WORDBREAK); 

		rect.SetRect( 90, 95, 134, 108 );
		dc.SetTextColor( RGB( 73, 90, 0 ) );
		str = NumberToOrientalString( money );
		dc.DrawText(str, &rect, DT_CENTER | DT_WORDBREAK); 
	}

	cdc.SelectObject(pOldFont);
	cdc.SelectObject(pOldBmp);
	cdc.DeleteDC();
	Back.DeleteObject();
	Font1.DeleteObject();
	Font2.DeleteObject();
	
	// Do not call CDialog::OnPaint() for painting messages
}



void CLeadersMoneyLimit::OnOK() 
{
	// TODO: Add extra validation here
	if ( m_LeadersKind == 4 )
	{
		CDialog::OnCancel();
		return;
	}

	g_pMainDlg->bNowLogout = FALSE; // ������ �˸��� �״´�.
		
	g_Where = WH_TITLE;
	Sound.Play(SND45);

	CString str ="http://www.netmarble.net";
	if(m_LJ.PremLType==0)
		g_pMainDlg->BrowserMove( str + m_LJ.LeadersJoinURL ); // ����
	else
		g_pMainDlg->BrowserMove( str + m_LJ.LeadersJoinURL2 );// ����
		
	CDialog::OnOK();
}

void CLeadersMoneyLimit::OnCancel() 
{
	// TODO: Add extra cleanup here
	if ( m_LeadersKind != 4 )
	{
		CLimitMoneyConfirmDlg dlg;
		if ( dlg.DoModal() == IDOK )
		{
			CDialog::OnCancel();
		}
		return;
	}
	CDialog::OnCancel();
}

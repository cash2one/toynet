// BanishVoteDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "BanishVoteDlg.h"

#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBanishVoteDlg dialog


CBanishVoteDlg::CBanishVoteDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBanishVoteDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBanishVoteDlg)
	m_Reson = _T("");
	//}}AFX_DATA_INIT
}


void CBanishVoteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBanishVoteDlg)
	DDX_Control(pDX, IDCANCEL, m_CancelBtn);
	DDX_Control(pDX, IDOK, m_OkBtn);
	DDX_Control(pDX, IDC_COMBO_TARGETID, m_TargetIDCombo);
	DDX_CBString(pDX, IDC_COMBO_RESON, m_Reson);
	DDV_MaxChars(pDX, m_Reson, 40);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBanishVoteDlg, CDialog)
	//{{AFX_MSG_MAP(CBanishVoteDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBanishVoteDlg message handlers

BOOL CBanishVoteDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//m_TargetIDCombo.AddString("선택하세요");
	int cnt=0;
	for(int i=1; i<g_Max_Player; i++)
	{
		if(strlen(Play[i].UI.ID)>0) 
		{
			m_TargetIDCombo.AddString(Play[i].UI.ID);
			TarID[cnt] = Play[i].UI.ID;
			cnt++;
		}
	}

	m_OkBtn.Init(29,234,".\\image\\commonbtn\\Btn_ok.bmp", 4);
	m_CancelBtn.Init(149,234,".\\image\\commonbtn\\Btn_cancel.bmp",4);

	RECT rc1;
	GetWindowRect( &rc1 );
	rc1.right = rc1.left + 256;
	rc1.bottom = rc1.top + 284;
	MoveWindow( &rc1 );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBanishVoteDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();

	int nowsel = m_TargetIDCombo.GetCurSel();
	if(nowsel >= 0)
	{
		if(m_Reson.GetLength()==0)
		{
			g_pMainDlg->m_MsgBoxDlg.MsgBox( 0 , "추방할 이유를 직접 기입하거나 리스트에서 선택하세요");
			
			return;
		}
		else if(Game.GetPlayerPNum((char*)TarID[nowsel].operator LPCTSTR()) == -1)
		{
			g_pMainDlg->m_MsgBoxDlg.MsgBox(0, "존재하지 않는 사용자입니다");
		}
		else if(g_RI.NowUserNum < 3)
		{
			g_pMainDlg->m_MsgBoxDlg.MsgBox(0, "3명 이상인 경우만 추방 투표가 가능합니다");
			return;
		}
		else
		{

			if(g_RI.bSuperMaster ) {
				if(strncmp(g_RI.ID, TarID[nowsel], 15)==0) {
					g_pMainDlg->m_MsgBoxDlg.MsgBox(0, "슈퍼방장은 추방 할 수 없습니다.");
					return;
				}
			}

			CString str;
			str.Format("정말로 [%s]님을 강제 추방하시겠습니까?", (LPCTSTR)TarID[nowsel]);
			if(AfxMessageBox(str, MB_YESNO)==IDNO) return;

			BANISHVOTEINFO bv = {0,};
			bv.RoomNum = g_RI.RoomNum;
			strncpy(bv.MyID, Play[0].UI.ID, 15);
			strncpy(bv.TarID, (LPCTSTR)TarID[nowsel], 15);
			strncpy(bv.Reson, (LPCTSTR)m_Reson, 40);

			CSV_ASK_BANISHVOTE abvmsg;
			abvmsg.Set(&bv);
			SockMan.SendData(g_MainSrvSID, abvmsg.pData, abvmsg.GetTotalSize());
		}
	}
	else
	{
		AfxMessageBox("강제 추방을 원하는 아이디를 선택하세요");
		return;
	}
	CDialog::OnOK();
}

void CBanishVoteDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CBanishVoteDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);

	CMyBitmap Back;
	Back.LoadBitmapFile(".\\image\\vote\\banishvote.BMP");

	HRGN h = BitmapToRegion( Back.operator HBITMAP(), RGB(255,0,255));
	if(h) SetWindowRgn(h,TRUE);// 사용방법

	CBitmap *pOldBmp = cdc.SelectObject(&Back);	

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);
	
	cdc.SelectObject(pOldBmp);
	::DeleteObject(h);
	h = NULL;
	cdc.DeleteDC();
	Back.DeleteObject();

	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}

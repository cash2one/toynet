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
	//m_TargetIDCombo.AddString("�����ϼ���");
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
			g_pMainDlg->m_MsgBoxDlg.MsgBox( 0 , "�߹��� ������ ���� �����ϰų� ����Ʈ���� �����ϼ���");
			
			return;
		}
		else if(Game.GetPlayerPNum((char*)TarID[nowsel].operator LPCTSTR()) == -1)
		{
			g_pMainDlg->m_MsgBoxDlg.MsgBox(0, "�������� �ʴ� ������Դϴ�");
		}
		else if(g_RI.NowUserNum < 3)
		{
			g_pMainDlg->m_MsgBoxDlg.MsgBox(0, "3�� �̻��� ��츸 �߹� ��ǥ�� �����մϴ�");
			return;
		}
		else
		{

			if(g_RI.bSuperMaster ) {
				if(strncmp(g_RI.ID, TarID[nowsel], 15)==0) {
					g_pMainDlg->m_MsgBoxDlg.MsgBox(0, "���۹����� �߹� �� �� �����ϴ�.");
					return;
				}
			}

			CString str;
			str.Format("������ [%s]���� ���� �߹��Ͻðڽ��ϱ�?", (LPCTSTR)TarID[nowsel]);
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
		AfxMessageBox("���� �߹��� ���ϴ� ���̵� �����ϼ���");
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
	if(h) SetWindowRgn(h,TRUE);// �����

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

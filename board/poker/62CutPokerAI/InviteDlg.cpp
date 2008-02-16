// InviteDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "InviteDlg.h"
#include "Global.h"
#include "SendInviteDlg.h"
#include "WhisperDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInviteDlg dialog


CInviteDlg::CInviteDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInviteDlg::IDD, pParent)
{
	ViewMode = 0;
	nUserSel = -1;
	//{{AFX_DATA_INIT(CInviteDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CInviteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInviteDlg)
	DDX_Control(pDX, IDC_BUTTON_ALLUSER, m_BtnAllUser);
	DDX_Control(pDX, IDC_BUTTON_LOBY, m_BtnLoby);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_CloseBtn);
	DDX_Control(pDX, IDC_BUTTON_INVITE, m_InviteBtn);
	DDX_Control(pDX, IDC_BUTTON_PROFILE, m_ProfileBtn);
	DDX_Control(pDX, IDC_LIST_USER, m_UserList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInviteDlg, CDialog)
	//{{AFX_MSG_MAP(CInviteDlg)
	ON_BN_CLICKED(IDC_BUTTON_PROFILE, OnButtonProfile)
	ON_BN_CLICKED(IDC_BUTTON_INVITE, OnButtonInvite)
	ON_BN_CLICKED(IDC_RADIO_ALLUSER, OnRadioAlluser)
	ON_BN_CLICKED(IDC_RADIO_WAITUSER, OnRadioWaituser)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_USER, OnDblclkListUser)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_USER, OnItemchangedListUser)
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_RCLICK, IDC_LIST_USER, OnRclickListUser)
	ON_COMMAND(IDC_BUTTON_WHISPER, OnButtonWhisper)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_LOBY, OnButtonLoby)
	ON_BN_CLICKED(IDC_BUTTON_ALLUSER, OnButtonAlluser)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInviteDlg message handlers

BOOL CInviteDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	// 다이얼로그 컨트롤 포지션과 위치 재정렬
	CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);

	// 사이즈 저장
	GetWindowRect(&OrgWindRect);
	GetDlgItem(IDC_LIST_USER)->GetWindowRect(&OrgListRect);
	ScreenToClient(&OrgListRect);

	// 버튼 초기화
	m_ProfileBtn.Init(157,17,".\\image\\Invite\\iBtn_invitep.bmp",4);
	m_InviteBtn.Init(28, 273, ".\\image\\Invite\\iBtn_invitei.bmp",4);
	m_CloseBtn.Init(122, 273, "image\\commonbtn\\Btn_Close.bmp", 4);

	m_BtnAllUser.Init(17, 33,"image\\Invite\\ibtn_alluser.BMP", 4);
	m_BtnLoby.Init(90, 33, "image\\Invite\\ibtn_loby.bmp", 4);
	


	// 메뉴 초기화
	m_InviteMenu.LoadMenu(IDR_INVITEMENU);
	//SetMenu(&m_InviteMenu);

	ViewMode = 0;
	CheckRadioButton( IDC_RADIO_WAITUSER, IDC_RADIO_ALLUSER, IDC_RADIO_WAITUSER);
	User_InitList();

	SetBtnStat();

	RECT rc1;
	GetWindowRect( &rc1 );
	rc1.right = rc1.left + 231;
	rc1.bottom = rc1.top + 317;
	MoveWindow( &rc1 );	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInviteDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	//CDialog::OnOK();
}

void CInviteDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	if(g_Where == WH_GAME) {
		CSV_LOBYOPEN lomsg;
		lomsg.Set(FALSE);
		SockMan.SendData(g_MainSrvSID, lomsg.pData, lomsg.GetTotalSize());
	}
	
	ShowWindow(SW_HIDE);
	//CDialog::OnCancel();
}

void CInviteDlg::OnButtonProfile() 
{
	Sound.Play(SND45);

	// 선택된 아이템중 첫번째 아이템을 얻는다
	POSITION pos = m_UserList.GetFirstSelectedItemPosition();
	if (pos == NULL) return;
	nUserSel = m_UserList.GetNextSelectedItem(pos);
	
	if(nUserSel < 0) return;
	if(nUserSel >= m_UserList.GetItemCount()) return;

	CString szID;
	szID = m_UserList.GetItemText(nUserSel, 0);
	int UNum = m_UserList.GetItemData(nUserSel);
	if(szID.GetLength() == 0) return;
	if(UNum < 0) return;

	g_Wait = TRUE;

	CSV_ASK_USERINFO aumsg;
	aumsg.Set(UNum, (char*)szID.operator LPCTSTR());
	SockMan.SendData(g_MainSrvSID, aumsg.pData, aumsg.GetTotalSize());
}

void CInviteDlg::OnButtonInvite() 
{
	Sound.Play(SND45);

	// 선택된 아이템중 첫번째 아이템을 얻는다
	POSITION pos = m_UserList.GetFirstSelectedItemPosition();
	if (pos == NULL) return;
	nUserSel = m_UserList.GetNextSelectedItem(pos);

	if(nUserSel < 0) return;
	if(nUserSel >= m_UserList.GetItemCount()) return;

	CString szID, szPos;
	szID = m_UserList.GetItemText(nUserSel, 0);
	szPos = m_UserList.GetItemText(nUserSel, 2);
	int UNum = m_UserList.GetItemData(nUserSel);
	if(szID.GetLength() == 0) return;
	if(szPos != "대기실") { g_pMainDlg->m_MsgBoxDlg.MsgBox(0, "대기실에 있는 사용자만 초대할 수 있습니다"); return;}
	if(UNum < 0) return;
	if(g_RI.NowUserNum >= g_RI.MaxUserNum) { g_pMainDlg->m_MsgBoxDlg.MsgBox(0, "이미 사용자가 초과하였습니다"); return;}

	CSendInviteDlg Dlg;
	int rtn = Dlg.DoModal(UNum, szID);
	if(rtn == IDCANCEL) return;

	// 초청 메세지를 보냄
	// ### [ 관전기능 ] ###
	CSV_ASK_INVITE aimsg;
	aimsg.Set(UNum, (char*)szID.operator LPCTSTR(), g_RI.RoomNum, g_RI.Pass, &g_MyInfo.UI, (char*)Dlg.m_InviteMsg.operator LPCTSTR());
	SockMan.SendData(g_MainSrvSID, aimsg.pData, aimsg.GetTotalSize());
}

void CInviteDlg::OnButtonWhisper() 
{
	// 선택된 아이템중 첫번째 아이템을 얻는다
	POSITION pos = m_UserList.GetFirstSelectedItemPosition();
	if (pos == NULL) return;
	nUserSel = m_UserList.GetNextSelectedItem(pos);

	if(nUserSel < 0) return;
	if(nUserSel >= m_UserList.GetItemCount()) return;

	CString szID;
	szID = m_UserList.GetItemText(nUserSel, 0);
	if(szID.GetLength() == 0) return;

	//CWhisperDlg Dlg;
	g_WhisperDlg.Create((char*)szID.operator LPCTSTR());
}


void CInviteDlg::OnRadioAlluser() 
{
	// TODO: Add your control notification handler code here
	ViewMode = 1;
	User_ReloadList();
}

void CInviteDlg::OnRadioWaituser() 
{
	// TODO: Add your control notification handler code here
	ViewMode = 0;
	User_ReloadList();
}

void CInviteDlg::MoveToSide()
{
	CRect rcmain, rc;
	if(Display.DDRAWINIT==TRUE)
	{
		//GetWindowRect(&rc);
		// 창 사이즈를 줄임
		rc = OrgWindRect;
		rc.OffsetRect(-rc.left, -rc.top);
		rc.OffsetRect(220, 85);
		rc.bottom -= 200;
		MoveWindow(&rc);
		
		// 리스트의 사이즈를 줄임
		CRect listrc;
		listrc = OrgListRect;
		listrc.bottom = OrgListRect.bottom - 200;
		GetDlgItem(IDC_LIST_USER)->MoveWindow(&listrc);
	}
	else
	{
		g_pMainDlg->GetWindowRect(&rcmain);
		//GetWindowRect(&rc);
		rc = OrgWindRect;
		rc.OffsetRect(rcmain.right - OrgWindRect.left, rcmain.top - OrgWindRect.top);
		MoveWindow(&rc);
		GetDlgItem(IDC_LIST_USER)->MoveWindow(&OrgListRect);
	}
}

void CInviteDlg::User_InitList()
{
	// 비트맵 읽기
	CBitmap Bmp;
	Bmp.LoadBitmap(IDB_LEVEL15);

	// 이미지 리스트 생성
	m_UserImage.Create(15, 15, ILC_COLOR8 | ILC_MASK, 15, 0);
	m_UserImage.Add(&Bmp, RGB(255,0,255));

	m_UserList.SetImageList(&m_UserImage, LVSIL_SMALL);
	m_UserList.SetFullRowSel(TRUE);
	m_UserList.SetTextColor(RGB(0,0,70));

//	=========================================================================================
//	샘플작업
    m_UserList.SetGridLineUse( true );
    m_UserList.SetScrollBarChange( true );
	m_UserList.SetHeaderColors( RGB( 255, 255, 255 ), RGB( 66, 65, 66 ) );
	m_UserList.SetBackColors( RGB( 254, 253, 253 ), RGB( 253, 253, 253 ) );

	m_UserList.Init( true, true, 3, RGB( 45, 44, 0 ), RGB( 45, 44, 0 ) ); //정렬
//	=========================================================================================


	//// Column 삽입부
	LV_COLUMN col;
	//char szName[256];

	//wsprintf(szName, "아이디");
	
	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_WIDTH | LVCF_TEXT;
	col.fmt  = LVCFMT_LEFT;
	col.cx   = 80;
	//col.pszText = szName;
	col.pszText = "          ID";
	col.iSubItem = 0;
	m_UserList.InsertColumn(0, &col);

	col.fmt  = LVCFMT_CENTER;
	col.cx   = 40;
	//wsprintf(szName, "성별");
	col.pszText = g_StrMan._Get(_T("SEX"));
	col.iSubItem = 1;
	m_UserList.InsertColumn(1, &col);

	col.fmt  = LVCFMT_LEFT;
	col.cx   = 52;
	//wsprintf(szName, "위치");
	col.pszText = g_StrMan._Get(_T("STATE"));
	col.iSubItem = 2;
	m_UserList.InsertColumn(2, &col);
}

void CInviteDlg::User_InsItem(int unum, char *id, BOOL sex, int winnum, int loosenum, int drawnum, int point, int position, int roomnum, int level)
{
	// 대기실 보기 상태이면 위치가 대기실이 아닌 사용자는 삽입 취소
	if(ViewMode==0 && position!=1) return;
/*
	// 이미 같은 사용자가 삽입되어 있는지 검사하고 있다면 삽입 취소
	for(;;)
	{
		CString szID;
		int num = m_UserList.GetNextItem(num, LVNI_ALL);
		if(num == -1) break;
		szID = m_UserList.GetItemText(num, 0);
		int UNum = m_UserList.GetItemData(num);
		if(strcmp(id, szID.operator LPCTSTR())==0 && UNum == unum) return;
	}
*/
	// 레코드를 리스트 뷰에 삽입
	LV_ITEM item;
	char szName[256];

	// 이미지
	//int image=GetLevel(point, winnum, loosenum);
	//int image = Play[PNum].UI.nIcon;

	// 아이디
	wsprintf(szName, "%s", id);
	item.mask = LVIF_TEXT | LVIF_IMAGE;
	item.pszText = szName;
	item.iItem = m_UserList.GetItemCount();
	item.iSubItem = 0;
	item.iImage = level;//image;
	m_UserList.InsertItem(&item);

	// 유저번호
	m_UserList.SetItemData(item.iItem, unum);

	// 성별
	if(sex==TRUE) wsprintf(szName, "♂");
	else wsprintf(szName, "♀");
	item.iSubItem = 1;
	m_UserList.SetItem(&item);

	// 위치
	switch(position)
	{
	//case 0: wsprintf(szName, "몰라용"); break;
	//case 1: wsprintf(szName, "대기실"); break;
	//case 2: wsprintf(szName, "Shop"); break;
	//case 3: wsprintf(szName, "%d번방", roomnum); break;
		case 0: wsprintf(szName, "#####"); break;
		case 1: wsprintf(szName, g_StrMan._Get(_T("LOBBY"))); break;
		case 2: wsprintf(szName, "Shop"); break;
		case 3: wsprintf(szName, g_StrMan._Get(_T("ROOMNUM")), roomnum); break;
	}
	item.iSubItem = 2;
	m_UserList.SetItem(&item);
}

void CInviteDlg::User_SetItem(int unum, char *id, int winnum, int loosenum, int point, int position, int roomnum, int level)
{
	// 대기실 보기 상태이고 위치가 대기실이 아니면 삭제
	if(ViewMode==0 && position!=1)
	{
		User_DelItem(unum, id);
		return;
	}

	int num=-1;
	CString szID;
	char szName[256]={0,};

	for(;;)
	{
		num = m_UserList.GetNextItem(num, LVNI_ALL);
		if(num == -1) break;
		szID = m_UserList.GetItemText(num, 0);
		int UNum = m_UserList.GetItemData(num);
		if(strcmp(id, szID.operator LPCTSTR())==0 && UNum == unum)
		{
			// 레벨
			//int Image = GetLevel(point, winnum, loosenum);
			//int Image = Play[PNum].UI.nIcon;
			LV_ITEM item;
			item.mask = LVIF_IMAGE;
			item.iItem = num;
			item.iSubItem = 0;
			item.iImage = level;
			m_UserList.SetItem(&item);
			
			// 위치
			switch(position)
			{
			case 0: wsprintf(szName, "????"); break;
			case 1: wsprintf(szName, "대기실"); break;
			case 2: wsprintf(szName, "Shop"); break;
			case 3: wsprintf(szName, "%d번방", roomnum); break;
			}
			m_UserList.SetItemText(num, 2, szName);
			break;
		}
	}
}

void CInviteDlg::User_DelItem(int unum, char *id)
{
	int num=-1;
	CString szID;
	char szName[256]={0,};

	for(;;)
	{
		num = m_UserList.GetNextItem(num, LVNI_ALL);
		if(num == -1) break;
		szID = m_UserList.GetItemText(num, 0);
		int UNum = m_UserList.GetItemData(num);
		if(strcmp(id, szID.operator LPCTSTR())==0 && UNum == unum) {m_UserList.DeleteItem(num);	break;}
	}
}

void CInviteDlg::User_ReloadList()
{
	nUserSel = -1;

	int num=-1;
	CString szID, szSex, szPos;
	LV_ITEM item;

	m_UserList.DeleteAllItems();

	for(;;)
	{
		num = g_pLobyDlg->m_UserList.GetNextItem(num, LVNI_ALL);
		if(num == -1) break;
		int UNum, Image;

		// 아이디, 성별, 현재위치, 사용자 번호 구하기
		szID = g_pLobyDlg->m_UserList.GetItemText(num, 0);
		szSex = g_pLobyDlg->m_UserList.GetItemText(num, 1);
		szPos = g_pLobyDlg->m_UserList.GetItemText(num, 2);
		UNum = g_pLobyDlg->m_UserList.GetItemData(num);

		// 이미지 인덱스 구하기
		item.mask = LVIF_IMAGE;
		item.iItem = num;
		item.iSubItem = 0;
		g_pLobyDlg->m_UserList.GetItem(&item);
		Image = item.iImage;

		if(szPos.Compare("대기실")==0 || ViewMode==1)
		{
			char szName[256];
			strcpy(szName, szID.operator LPCTSTR());
			item.mask = LVIF_TEXT | LVIF_IMAGE;
			item.pszText = szName;
			item.iItem = m_UserList.GetItemCount();
			item.iSubItem = 0;
			item.iImage = Image;
			m_UserList.InsertItem(&item);

			// 유저번호
			m_UserList.SetItemData(item.iItem, UNum);

			// 성별
			strcpy(szName, szSex.operator LPCTSTR());
			item.iSubItem = 1;
			m_UserList.SetItem(&item);

			// 위치
			strcpy(szName, szPos.operator LPCTSTR());
			item.iSubItem = 2;
			m_UserList.SetItem(&item);

			//User_InsItem(UNum, (char*)szID.operator LPCTSTR(), FALSE, 0, 0, 0, 0, 2);
		}
	}
}

void CInviteDlg::OnDblclkListUser(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	nUserSel = pNMListView->iItem;
	OnButtonProfile();
	*pResult = 0;
}

void CInviteDlg::OnItemchangedListUser(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	nUserSel = pNMListView->iItem;
	*pResult = 0;
}

void CInviteDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	g_pGameDlg->SetFocus();
}


void CInviteDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;
	
	if(Back.m_hObject) Back.DeleteObject();
	Back.LoadBitmapFile(".\\image\\Invite\\inviteback.bmp");

	HRGN h = BitmapToRegion( Back.operator HBITMAP(), RGB(255,0,255));
	if(h) SetWindowRgn(h,TRUE);// 사용방법

	CBitmap *pOldBmp = cdc.SelectObject(&Back);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);

	::DeleteObject(h);
	h = NULL;
	cdc.SelectObject(pOldBmp);
	cdc.DeleteDC();
	Back.DeleteObject();
	// Do not call CDialog::OnPaint() for painting messages
}


void CInviteDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	Back.DeleteObject();
}

void CInviteDlg::OnRclickListUser(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	nUserSel = pNMListView->iItem;
	*pResult = 0;
	
	if(nUserSel < 0)
	{
		m_InviteMenu.GetSubMenu(0)->EnableMenuItem(IDC_BUTTON_PROFILE, MF_GRAYED);
		m_InviteMenu.GetSubMenu(0)->EnableMenuItem(IDC_BUTTON_INVITE, MF_GRAYED);
		m_InviteMenu.GetSubMenu(0)->EnableMenuItem(IDC_BUTTON_WHISPER, MF_GRAYED);
	}
	else
	{
		m_InviteMenu.GetSubMenu(0)->EnableMenuItem(IDC_BUTTON_PROFILE, MF_ENABLED);
		m_InviteMenu.GetSubMenu(0)->EnableMenuItem(IDC_BUTTON_INVITE, MF_ENABLED);
		m_InviteMenu.GetSubMenu(0)->EnableMenuItem(IDC_BUTTON_WHISPER, MF_ENABLED);
	}

	// 팝업 메뉴
	CPoint pnt;
	GetCursorPos(&pnt);
	m_InviteMenu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pnt.x, pnt.y, this);
}

void CInviteDlg::OnButtonClose() 
{
	// TODO: Add your control notification handler code here
	Sound.Play(SND45);

	PostMessage(WM_COMMAND, IDCANCEL, 0);
	
}

void CInviteDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
	
	
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CInviteDlg::OnButtonLoby() 
{
	// TODO: Add your control notification handler code here
	ViewMode = 0;
	SetBtnStat();
	User_ReloadList();
	
}

void CInviteDlg::OnButtonAlluser() 
{
	// TODO: Add your control notification handler code here
	ViewMode = 1;
	SetBtnStat();
	User_ReloadList();
	
}

void CInviteDlg::SetBtnStat()
{
	if( ViewMode == 0) {
		m_BtnLoby.EnableWindow(FALSE);
		m_BtnAllUser.EnableWindow(TRUE);
	}
	else
	{
		m_BtnLoby.EnableWindow(TRUE);
		m_BtnAllUser.EnableWindow(FALSE);
	}

}

// LobyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "LobyDlg.h"
#include "Global.h"
#include "ProfileDlg.h"
#include "ChangeCharDlg.h"
#include "RoomPassDlg.h"
#include "WhisperDlg.h"
#include "BadUserDlg.h"
#include "MyBitmap.h"

#include "OptionDlg.h"
#include "ResultFindIdDlg.h"

#include "AdminTitleDlg.h"// [관리자 모드 작업]
#include "ResultHi.h"
#include "MiniDlg.h"
//#include "SelCardDlg.h"
//#include "SelCardChoiceDlg.h"
//#include "Oring.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLobyDlg dialog


CLobyDlg::CLobyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLobyDlg::IDD, pParent)
{
	g_pLobyDlg = this;
	iAccessRoomState = 0;
	nRoomSel = nUserSel = -1;
	m_bFullScr = FALSE;
	m_LobyMsg = "";
	//{{AFX_DATA_INIT(CLobyDlg)
	m_RefuseInviteCheck = FALSE;
	m_RefuseWhisper = FALSE;
	//}}AFX_DATA_INIT

	m_nLobyOffSetX[0] = 600;
	m_nLobyOffSetX[1] = 350;
	
	m_nLobyOffSetY[0] = 460;
	m_nLobyOffSetY[1] = 410;
	
	m_nLobyBtnIndex = 0;
	m_bLobyBtnMouseDown = FALSE; 
}


void CLobyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLobyDlg)
	DDX_Control(pDX, IDC_BUTTON_NORMAL, m_btnNormal);
	DDX_Control(pDX, IDC_BUTTON_ALLKIND, m_btnAllKind);
	DDX_Control(pDX, IDC_BUTTON_CHOICE, m_btnChoice);
	DDX_Control(pDX, IDC_BUTTON_CAPTURE, m_CaptureBtn);
	DDX_Control(pDX, IDC_BUTTON_ITEMBAG, m_ItemBagBtn);
	DDX_Control(pDX, IDC_BUTTON_MINIMIZE, m_MiniBtn);
	//DDX_Control(pDX, IDC_BUTTON_QUICKSTART, m_QuickStart);
	DDX_Control(pDX, IDC_BUTTON_OPTION, m_Option);
	DDX_Control(pDX, IDC_BUTTON_EXIT1, m_ExitLoby);
	DDX_Control(pDX, IDC_BUTTON_CARD, m_CardShop);
	DDX_Control(pDX, IDC_BUTTON_FINDID, m_FindIDBtn);
	DDX_Control(pDX, IDC_STATIC_BANNER, m_BannerWnd);
	DDX_Control(pDX, IDC_BUTTON_BADUSER, m_BadUserBtn);
	DDX_Control(pDX, IDC_BUTTON_CHANGEDISPLAY, m_ChangeDisplayBtn);
	DDX_Control(pDX, IDC_BUTTON_ROOMINFO, m_RoomInfoBtn);
	DDX_Control(pDX, IDC_BUTTON_ENTERGAME, m_EnterRoomBtn);
	DDX_Control(pDX, IDC_BUTTON_CREATEROOM, m_CreateRoomBtn);
	DDX_Control(pDX, IDC_BUTTON_CHANGEPROFILE, m_ChangeProfileBtn);
	DDX_Control(pDX, IDC_BUTTON_VIEWPROFILE, m_ViewProfileBtn);
	DDX_Control(pDX, IDC_BUTTON_LOGOUT, m_LogoutBtn);
	DDX_Control(pDX, IDC_LIST_ROOM, m_RoomList);
	DDX_Control(pDX, IDC_LIST_USER, m_UserList);
	DDX_Control(pDX, IDC_RICHEDIT_LOBYCHAT, m_ChatViewEdit);
	DDX_Control(pDX, IDC_EDIT_LOBYSEND, m_SendEdit);
	DDX_Check(pDX, IDC_CHECK_REFUSEINVITE, m_RefuseInviteCheck);
	DDX_Check(pDX, IDC_CHECK_REFUSEWHISPER, m_RefuseWhisper);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLobyDlg, CDialog)
	//{{AFX_MSG_MAP(CLobyDlg)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ROOM, OnDblclkListRoom)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ROOM, OnItemchangedListRoom)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_USER, OnDblclkListUser)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_USER, OnItemchangedListUser)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_ENTERGAME, OnButtonEntergame)
	ON_BN_CLICKED(IDC_BUTTON_CREATEROOM, OnButtonCreateroom)
	ON_BN_CLICKED(IDC_BUTTON_HOMPAGE, OnButtonHompage)
	ON_BN_CLICKED(IDC_BUTTON_LOGOUT, OnButtonLogout)
	ON_BN_CLICKED(IDC_BUTTON_VIEWPROFILE, OnButtonViewprofile)
	ON_BN_CLICKED(IDC_BUTTON_CHANGEPROFILE, OnButtonChangeprofile)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_ROOMINFO, OnButtonRoominfo)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_ROOM, OnRclickListRoom)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_USER, OnRclickListUser)
	ON_COMMAND(IDC_BUTTON_WHISPER, OnButtonWhisper)
	ON_BN_CLICKED(IDC_BUTTON_CHANGEDISPLAY, OnButtonChangedisplay)
	ON_BN_CLICKED(IDC_BUTTON_BADUSER, OnButtonBaduser)
	ON_BN_CLICKED(IDC_BUTTON_FINDID, OnButtonFindid)
	ON_BN_CLICKED(IDC_BUTTON_CARD, OnButtonCard)
	ON_BN_CLICKED(IDC_BUTTON_OPTION, OnButtonOption)
	ON_BN_CLICKED(IDC_BUTTON_EXIT1, OnButtonExit1)
	//ON_BN_CLICKED(IDC_BUTTON_QUICKSTART, OnButtonQuickstart)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_MINIMIZE, OnButtonMinimize)
	ON_BN_CLICKED(IDC_BUTTON_ITEMBAG, OnButtonItembag)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_CAPTURE, OnButtonScreencaptureloby)
	ON_COMMAND(IDC_MENU_ADM_WARN_USER, OnMenuAdmWarnUser) // 유저 
	ON_COMMAND(IDC_MENU_ADM_USERINFO, OnMenuAdmUserinfo)
	ON_COMMAND(IDC_MENU_ADM_SENDWHISPHER, OnMenuAdmSendwhispher)
	ON_BN_CLICKED(IDC_CHECK_REFUSEWHISPER, OnCheckRefusewhisper)
	ON_BN_CLICKED(IDC_CHECK_REFUSEINVITE, OnCheckRefuseinvite)
	ON_BN_CLICKED(IDC_BUTTON_NORMAL, OnButtonNormal)
	ON_BN_CLICKED(IDC_BUTTON_CHOICE, OnButtonChoice)
	ON_BN_CLICKED(IDC_BUTTON_ALLKIND, OnButtonAllkind)
	ON_WM_TIMER()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*
	ON_COMMAND(IDC_MENU_ADM_INROOM, OnMenuAdmInroom)   // 방
	ON_COMMAND(IDC_MENU_ADM_RENAMEROOM, OnMenuAdmRenameroom)
	ON_COMMAND(IDC_MENU_ADM_ROOMDEL, OnMenuAdmRoomdel)
	ON_COMMAND(IDC_MENU_ADM_ROOMINFO, OnMenuAdmRoominfo)
	ON_COMMAND(IDC_MENU_ADM_MAKEROOM, OnMenuAdmMakeroom)
	ON_COMMAND(IDC_MENU_ADM_ROOMMASTER, OnMenuAdmRoommaster)
*/
/////////////////////////////////////////////////////////////////////////////
// CLobyDlg message handlers

BOOL CLobyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	if(LobybackSpr.Load(".\\data\\lobyback.spr", 555)==FALSE) AfxMessageBox("lobyback.spr 파일을 읽을 수 없습니다");
	if(InsertCoinSpr.Load(".\\data\\insertcoin.spr", 555)==FALSE) AfxMessageBox("insertcoin.spr 파일을 읽을 수 없습니다");
	if(QuickStartSpr.Load(".\\data\\StartBtn.spr", 555)==FALSE) AfxMessageBox("MainStartBtn.spr 파일을 읽을 수 없습니다");
	if(OutCoinSpr.Load(".\\data\\outcoin.spr", 555)==FALSE) AfxMessageBox("outcoin.spr 파일을 읽을 수 없습니다");

	m_ChoiceKind = 0 ;
	m_nLobyCnt = 0;
	SetTimer(LOBY_TIMER , 60 , NULL);
	InitPage();

	if(Font.m_hObject == NULL)
		Font.CreateFont(12,0,0,0,
		FW_NORMAL,
		FALSE,FALSE,
		0,
		//ANSI_CHARSET,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		"굴림");


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLobyDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	// 더블 버퍼링용 메모리 DC 생성

	SetFocus();

	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);
	
	// 메모리DC에 버퍼용 비트맵 핸들을 선택
	MemDC.SelectObject(Page.hBmp);
	
	Page.PutSprAuto(0, 0, &LobybackSpr, 0);

	OutCoinBtn.Draw(&MemDC);
	QuickStartBtn.Draw(&MemDC);

	if( g_MyInfo.UI.PMoney < 500)
	{
		if( (m_nLobyCnt/12) %2 == 0 )
			Page.PutSprAuto(300, 450, &InsertCoinSpr, 0);
	}

	// 마우스 좌표 체크
	POINT mpos;
	GetCursorPos(&mpos);
	ScreenToClient(&mpos);
	CString str;
	str.Format("Mouse (%d, %d)", mpos.x, mpos.y);
	MemDC.SelectObject(&Font);
	MemDC.SetBkMode(TRANSPARENT);
	MemDC.SetTextColor(RGB(255,255,255));
	MemDC.TextOut(100,100, str);
	
	// Credit
	CString strM;
	CRect rtMoney;
	GetClientRect(&rtMoney);
	rtMoney.SetRect(0,0,200,80);
	rtMoney.OffsetRect(565, 525);
	strM = NumberToOrientalString(g_MyInfo.UI.PMoney-g_MyInfo.BankMoney);
	str.Format("%s원", strM);
	MemDC.DrawText(str, &rtMoney, DT_RIGHT | DT_WORDBREAK);

	// Bank
	rtMoney.SetRect(0,0,200,80);
	rtMoney.OffsetRect(565, 545);
	strM = NumberToOrientalString(g_MyInfo.BankMoney);
	str.Format("%s원", strM);
	MemDC.DrawText(str, &rtMoney, DT_RIGHT | DT_WORDBREAK);

	

	CRect rect;
	GetClientRect(&rect);
	
	dc.BitBlt(0, 0, rect.right, rect.bottom, &MemDC, 0, 0, SRCCOPY);

	MemDC.DeleteDC();


	OutCoinBtn.OnMouseMove( m_nLobyOffSetX[m_nLobyBtnIndex], m_nLobyOffSetY[m_nLobyBtnIndex] );
	QuickStartBtn.OnMouseMove( m_nLobyOffSetX[m_nLobyBtnIndex], m_nLobyOffSetY[m_nLobyBtnIndex] );
	
	short mxp, myp;
	if( m_bLobyBtnMouseDown )
	{
		m_bLobyBtnMouseDown = FALSE;
		
		mxp = m_nLobyOffSetX[m_nLobyBtnIndex];
		myp = m_nLobyOffSetY[m_nLobyBtnIndex];
		
		OutCoinBtn.OnLButtonDown(mxp, myp);
		QuickStartBtn.OnLButtonDown(mxp, myp);
		
		OutCoinBtn.OnLButtonUp(mxp, myp);
		QuickStartBtn.OnLButtonUp(mxp, myp);
	}
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CLobyDlg::OnOK() 
{
	// TODO: Add extra validation here
	//CDialog::OnOK();
}

void CLobyDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	//CDialog::OnCancel();
}

void CLobyDlg::RefreshList()
{
	// 방 목록 및 사용자 목록 리스트를 초기화
	////////////////////////////////////
	// ### [ 대기실 리스트 관련 ] ###
	m_RoomView.DelAllList();

	//m_RoomList.DeleteAllItems();
	m_UserList.DeleteAllItems();

	// ###순서 변경###

	// 전체 사용자 목록을 요구함
	CSV_ASK_ALLUSERINFO auimsg;
	auimsg.Set();
	SockMan.SendData(g_MainSrvSID, auimsg.pData, auimsg.GetTotalSize());

	// 전체 방 목록을 요구함
	CSV_ASK_ALLROOMINFO sndmsg;
	sndmsg.Set();
	SockMan.SendData(g_MainSrvSID, sndmsg.pData, sndmsg.GetTotalSize());

	// 메시지 창
	m_MsgDlg.MsgBox(3, "방정보와 사용자 목록을 내려받는 중입니다");
}


/*
void CLobyDlg::Room_InitList()
{

	// 비트맵 읽기
	CBitmap Bmp;
	Bmp.LoadBitmap(IDB_IMAGE_ROOM);

	// 이미지 리스트 생성
	m_RoomImage.Create(15, 15, ILC_COLOR8 | ILC_MASK, 6, 0);
	m_RoomImage.Add(&Bmp, RGB(255,255,255));

	m_RoomList.SetImageList(&m_RoomImage, LVSIL_SMALL);
	m_RoomList.SetFullRowSel(TRUE);
	m_RoomList.SetTextColor(RGB(0,0,70));

	// 테스트 상태 이미지 추가 연습
	//m_RoomList.SetImageList(&m_RoomImage, LVSIL_STATE);


	
//	=========================================================================================
//	샘플작업
	//	3 - 아이콘 1,2 같은색과, 나머지는 같은색( 방에 들어갈수 있는곳과 없는곳 구분 )

//	m_RoomList.SetGridLineUse( false );
//  m_RoomList.SetScrollBarChange( true );
//	m_RoomList.SetHeaderColors( RGB( 255, 255, 255 ), RGB( 66, 65, 66 ) );
//	m_RoomList.SetBackColors( RGB( 246, 251, 255 ), RGB( 246, 251, 255 ) );
//	m_RoomList.Init( true, true, 3, RGB( 0, 0, 0 ), RGB( 54,  75, 123 ) ); // 정렬
//	=========================================================================================


	// [게임 아이템 작업]
	m_RoomList.Init(TRUE, TRUE, 0, RGB(245,245,255), RGB(0,0,0));
	m_RoomList.SetHeaderColors(RGB(210,210,255), RGB(0,0,0));
	m_RoomList.SetScrollBarChange(TRUE);


	//// Column 삽입부
	LV_COLUMN col;
	//char szName[256];

	// 넘버
	//wsprintf(szName, g_StrMan._Get(_T("L_RNUM")) );
	
	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_WIDTH | LVCF_TEXT;
	col.fmt  = LVCFMT_LEFT;
	col.cx   = 40;
	col.pszText = g_StrMan._Get(_T("L_RNUM"));
	col.iSubItem = 0;
	m_RoomList.InsertColumn(0, &col);

	// 방장
	col.fmt  = LVCFMT_CENTER;
	col.cx   = 89;
	//wsprintf(szName, g_StrMan._Get(_T("L_MASTER")) );
	col.pszText = g_StrMan._Get(_T("L_MASTER"));
	col.iSubItem = 1;
	m_RoomList.InsertColumn(1, &col);

	// 방제
	//col.fmt  = LVCFMT_LEFT;
	col.cx   = 189;
	col.pszText = g_StrMan._Get(_T("L_NAME"));
	col.iSubItem = 2;
	m_RoomList.InsertColumn(2, &col);

	// 게임종류
	col.cx   = 65;
	col.pszText = g_StrMan._Get(_T("GAMETYPE"));//g_StrMan._Get(_T("RAISE"));
	col.iSubItem = 3;
	m_RoomList.InsertColumn(3, &col);

	// 인원
	col.fmt  = LVCFMT_RIGHT;
	col.cx   = 40;
	col.pszText = g_StrMan._Get(_T("L_NUM"));
	col.iSubItem = 4;
	m_RoomList.InsertColumn(4, &col);

	// 상태
	col.cx   = 45;
	col.pszText = g_StrMan._Get(_T("STATE"));
	col.iSubItem = 5;
	m_RoomList.InsertColumn(5, &col);

	// 베팅
	col.cx   = 40;
	col.pszText = g_StrMan._Get(_T("L_BET"));
	col.iSubItem = 6;
	m_RoomList.InsertColumn(6, &col);

}
*/
/////////////////////////////////
// ### [ 대기실 리스트 관련 ] ###
void CLobyDlg::Room_InsItem(ROOMINFO *pRoom)
{
	m_RoomView.InsertList( pRoom );
}


void CLobyDlg::Room_SetItem(void *pRoom, BOOL bTitle) //(SMALLROOMINFO *pSRoom, BOOL bTitle)
{
	if(!bTitle)
		m_RoomView.SetList((SMALLROOMINFO*) pRoom, bTitle );
	else
		m_RoomView.SetList((CHANGESTYLEROOMINFO*) pRoom, bTitle );
}


void CLobyDlg::Room_DelItem(int roomnum)
{
	m_RoomView.DelList( roomnum, NULL );
}
/////////////////////////////////

void CLobyDlg::User_InitList()
{
	// 비트맵 읽기
	CBitmap Bmp;
	Bmp.LoadBitmap(IDB_LEVEL15);

	// 이미지 리스트 생성
	m_UserImage.Create(15, 15, ILC_COLOR8 | ILC_MASK, 15, 0);
	m_UserImage.Add(&Bmp, RGB(255,0,255));

	m_UserList.SetImageList(&m_UserImage, LVSIL_SMALL);
	m_UserList.SetFullRowSel(TRUE);
	m_RoomList.SetTextColor(RGB(0,0,70));

	
//	=========================================================================================
//	샘플작업
//    m_UserList.SetGridLineUse( true );
//    m_UserList.SetScrollBarChange( true );
//	m_UserList.SetHeaderColors( RGB( 255, 255, 255 ), RGB( 66, 65, 66 ) );
//	m_UserList.SetBackColors( RGB( 254, 253, 253 ), RGB( 253, 253, 253 ) );

//	m_UserList.Init( true, true, 3, RGB( 45, 44, 0 ), RGB( 45, 44, 0 ) ); // 정렬
//	m_UserList.Init( false, true, 3, RGB( 45, 44, 0 ), RGB( 45, 44, 0 ) ); // 정렬 안함
//	=========================================================================================

	// [게임 아이템 작업]
	m_UserList.Init(TRUE, TRUE, 0, RGB(245,245,255), RGB(0,0,0));
	m_UserList.SetHeaderColors(RGB(115,193,240), RGB(0,0,0));
	m_UserList.SetScrollBarChange(TRUE);
	m_UserList.SetGridLineUse(TRUE);


	//// Column 삽입부
	LV_COLUMN col;
	//char szName[256];

	//wsprintf(szName, "     ID");
	
	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_WIDTH | LVCF_TEXT;
	col.fmt  = LVCFMT_LEFT;
	col.cx   = 100;
	//col.pszText = szName;
	col.pszText = "          ID";
	col.iSubItem = 0;
	m_UserList.InsertColumn(0, &col);

	col.fmt  = LVCFMT_CENTER;
	col.cx   = 37;
	//wsprintf(szName, "성별");
	col.pszText = g_StrMan._Get(_T("SEX"));
	col.iSubItem = 1;
	m_UserList.InsertColumn(1, &col);

	col.fmt  = LVCFMT_CENTER;
	col.cx   = 53;
	//wsprintf(szName, "위치");
	col.pszText = g_StrMan._Get(_T("STATE"));
	col.iSubItem = 2;
	m_UserList.InsertColumn(2, &col);
}

void CLobyDlg::User_InsItem(int unum, char *id, BOOL sex, int winnum, int loosenum, int drawnum, int point, int position, int roomnum, int level )
{
	// 레코드를 리스트 뷰에 삽입
	LV_ITEM item;
	char szName[256];

	// 같은 아이디가 존재하는지 먼저 검사하고 이미 존재하면 취소
	int num=-1;
	CString szRN;
	for(;;)
	{
		num = m_UserList.GetNextItem(num, LVNI_ALL);
		if(num == -1) break;
		szRN = m_UserList.GetItemText(num, 0);
		if(strcmp(id, szRN.operator LPCTSTR())==0) return;
	}

	// 이미지
	//int image=GetLevel(point, winnum, loosenum);
	//int image = Play[PNum].UI.nIcon;

	// 아이디
	wsprintf(szName, "%s", id);
	item.mask = LVIF_TEXT | LVIF_IMAGE;
	item.pszText = szName;
	item.iItem = m_UserList.GetItemCount();
	item.iSubItem = 0;
	item.iImage = level; //image;
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

	g_pGameDlg->m_InviteDlg.User_InsItem(unum, id, sex, winnum, loosenum, drawnum, point, position, roomnum, level);
}

void CLobyDlg::User_SetItem(int unum, char *id, int winnum, int loosenum, int drawnum,int point, int position, int roomnum, int level )
{
	int num=-1;
	CString szID, szSex;
	char szName[256]={0,};

	for(;;)
	{
		num = m_UserList.GetNextItem(num, LVNI_ALL);
		if(num == -1) break;
		szID = m_UserList.GetItemText(num, 0);
		szSex = m_UserList.GetItemText(num, 1);
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
			item.iImage = level;// Image;
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

			if(g_pGameDlg->m_InviteDlg.ViewMode==0 && strcmp(szName, "대기실")==0)
			{
				int sex = TRUE;
				if(szSex.Compare("♀")==0) sex = FALSE;
				g_pGameDlg->m_InviteDlg.User_InsItem(unum, id, sex, winnum, loosenum, drawnum, point, position, roomnum, level);
			}
			break;
		}
	}

	g_pGameDlg->m_InviteDlg.User_SetItem(unum, id, winnum, loosenum, point, position, roomnum, level);
}

void CLobyDlg::User_DelItem(int unum, char *id)
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

	g_pGameDlg->m_InviteDlg.User_DelItem(unum, id);
}

void CLobyDlg::OnButtonLogout() 
{
	// TODO: Add your control notification handler code here
	if(g_Wait) return;

	g_Wait = TRUE;
	Sound.Play(SND45);

	CSV_ASK_OUTCHAN aomsg;
	aomsg.Set();
	SockMan.SendData(g_MainSrvSID, aomsg.pData, aomsg.GetTotalSize());
}

void CLobyDlg::OnDblclkListRoom(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	nRoomSel = pNMListView->iItem;
	OnButtonEntergame();
	*pResult = 0;
}


void CLobyDlg::OnItemchangedListRoom(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	nRoomSel = pNMListView->iItem;
	*pResult = 0;
}

void CLobyDlg::OnDblclkListUser(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	nUserSel = pNMListView->iItem;
	OnButtonViewprofile();
	*pResult = 0;
}

void CLobyDlg::OnItemchangedListUser(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	nUserSel = pNMListView->iItem;
	*pResult = 0;
}


// ### [ 대기실 리스트 관련 ] ###
void CLobyDlg::OnButtonRoominfo() 
{
	if(g_Wait == TRUE) return;
	Sound.Play(SND45);

	int rnum = m_RoomView.GetSelectIndex();
	if(rnum < 0) return;
	
	// 방 정보를 요구
	CSV_ASK_ROOMINFO arimsg;
	arimsg.Set(rnum);
	SockMan.SendData(g_MainSrvSID, arimsg.pData, arimsg.GetTotalSize());

	g_Wait = TRUE;

}
// ### [ 대기실 리스트 관련 ] ###
void CLobyDlg::OnButtonEntergame() 
{
	if(g_Wait == TRUE) return;
	if(iAccessRoomState != 0) return;

	//0원일때 입장 제한 메세지
//	if(g_MyInfo.UI.PMoney <=0){
//		g_pMainDlg->m_MsgBoxDlg.MsgBox(0,g_StrMan.Get(_T("LECK_PMONEY1")));  return;
//	}

	Sound.Play(SND45);

	int rnum = m_RoomView.GetSelectIndex();
	if(rnum < 0) return;

	char pass[5]={0,};

	ROOMINFO *pRi = m_RoomView.GetRoomInfo( rnum );
	if( pRi == NULL )		return;

	if( pRi->Secret == 1 ) {
		CRoomPassDlg dlg;
		if(dlg.DoModal()==IDOK) strncpy(pass, dlg.m_Pass.operator LPCTSTR(), 5);
		else return;
	}

	g_Wait = TRUE;
	iAccessRoomState=1; // 방에 접속 혹은 방을 생성 중임
	// 방에 입장하기를 요구
	CSV_ASK_ENTERROOM aermsg;
	aermsg.Set(rnum, g_MyInfo.UI.UNum,  g_MyInfo.nSndFxKind, g_MyInfo.UI.ID, pass);// ### [ 관전기능 ] ###  // ### [사운드 추가 작업] ###
	SockMan.SendData(g_MainSrvSID, aermsg.pData, aermsg.GetTotalSize());

	m_MsgDlg.MsgBox(1, "요청한 방에 연결중입니다");
}




/*
void CLobyDlg::OnButtonRoominfo() 
{
	// TODO: Add your control notification handler code here
	if(g_Wait == TRUE) return;

	Sound.Play(SND45);

	// 선택된 아이템중 첫번째 아이템을 얻는다
	POSITION pos = m_RoomList.GetFirstSelectedItemPosition();
	if (pos == NULL) return;
	nRoomSel = m_RoomList.GetNextSelectedItem(pos);

	if(nRoomSel < 0) return;
	if(nRoomSel >= m_RoomList.GetItemCount()) return;

	CString rstr;
	rstr = m_RoomList.GetItemText(nRoomSel, 0);
	if(rstr.GetLength()==0) return;
	int rnum = ::atoi(rstr.operator LPCTSTR());
	if(rnum < 0) return;

	// 방 정보를 요구
	CSV_ASK_ROOMINFO arimsg;
	arimsg.Set(rnum);
	SockMan.SendData(g_MainSrvSID, arimsg.pData, arimsg.GetTotalSize());

	g_Wait = TRUE;
}

void CLobyDlg::OnButtonEntergame() 
{
	// TODO: Add your control notification handler code here
	if(g_Wait == TRUE) return;
	if(iAccessRoomState != 0) return;

	Sound.Play(SND45);

	// 선택된 아이템중 첫번째 아이템을 얻는다
	POSITION pos = m_RoomList.GetFirstSelectedItemPosition();
	if (pos == NULL) return;
	nRoomSel = m_RoomList.GetNextSelectedItem(pos);
	
	if(nRoomSel < 0) return;
	if(nRoomSel >= m_RoomList.GetItemCount()) return;

	//if(Play[0].UI.Marble <= 0) {AfxMessageBox("게임 진행에 필요한 마블이 부족합니다"); return;}

	CString rstr;
	rstr = m_RoomList.GetItemText(nRoomSel, 0);
	if(rstr.GetLength()==0) return;
	int rnum = ::atoi(rstr.operator LPCTSTR());
	if(rnum < 0) return;

	// 이미지 번호 얻기
	LVITEM item = {0,};
	item.mask = LVIF_IMAGE; // 이미지
	item.iItem = nRoomSel; // 몇번
	item.iSubItem = 0; // 번호, 방제...
	
	char pass[5]={0,};

	m_RoomList.GetItem(&item);
	if(item.iImage == 3 || item.iImage == 4 || item.iImage == 5 ){
		CRoomPassDlg dlg;
		if(dlg.DoModal()==IDOK) strncpy(pass, dlg.m_Pass.operator LPCTSTR(), 5);
		else return;
	}

	g_Wait = TRUE;
	iAccessRoomState=1; // 방에 접속 혹은 방을 생성 중임

	// 방에 입장하기를 요구
	CSV_ASK_ENTERROOM aermsg;
	// ### [ 관전기능 ] ### 
	aermsg.Set(rnum, g_MyInfo.UI.UNum,  g_MyInfo.nSndFxKind, g_MyInfo.UI.ID, pass);// ### [사운드 추가 작업] ###
	//aermsg.Set(rnum, Play[0].UI.UNum,  Play[0].nSndFxKind, Play[0].UI.ID, pass);// ### [사운드 추가 작업] ###
	SockMan.SendData(g_MainSrvSID, aermsg.pData, aermsg.GetTotalSize());

	m_MsgDlg.MsgBox(1, "요청한 방에 연결중입니다");
}
*/
void CLobyDlg::OnButtonCreateroom() 
{
	// TODO: Add your control notification handler code here
	if(g_Wait == TRUE) return;
	if(iAccessRoomState != 0) return;
	
	// ### [ 관전기능 ] ###
	if(g_MyInfo.UI.PMoney < 100) {
		//g_pMainDlg->m_MsgBoxDlg.MsgBox( 0, (char*)(LPCSTR)g_StrMan.Get(_T("LECK_PMONEY1")) );

		// [수호천사] 2004.07.08
		CNewAskChargeDlg BigDlg;
		//BigDlg.DoModal();
		return; 
	}

	//Sound.Play(SND45);

	// ### [ 관전기능 ] ###
	//softpark
	//CCreateRoomDlg dlg(g_MyInfo.UI.nIcon, g_MyInfo.UI.PMoney);// g_MyData.GetMoney());
	

	//if(dlg.DoModal()==IDOK)
	{
		CSV_ASK_CREATEROOM msg;
		
		ROOMINFO ri;
		ZeroMemory(&ri, sizeof(ROOMINFO));
		// ### [ 관전기능 ] ###
		ri.UNum = g_MyInfo.UI.UNum;
		//strncpy(ri.Title, dlg.m_RoomTitle.operator LPCTSTR(), 30);
		strncpy(ri.Title, "ok let's go", 30);
		strncpy(ri.ID, g_MyInfo.UI.ID, 15);
		ri.NowUserNum = 1;
		ri.MaxUserNum = 2;//(char)dlg.nMaxNum;

		ri.CreateRGrade = 0;//dlg.ComboSelCreateRGrade;
		ri.UsernIcon = Play[0].UI.nIcon;

		// 레이즈 설정
		//switch(dlg.Level){
		int lev  = 0;
		//switch(dlg.Level){
		switch(lev){
		case 0:  ri.Level = 0; break;
		case 1:  ri.Level = 1; break;
		case 2:  ri.Level = 2; break;
		default: ri.Level = 0; break;
		}


	//	ri.MaxUserNum = dlg.nMaxNum;
		ri.RoomNum = 0;
		ri.Secret = 0;//dlg.bSecret;
		ri.GameKind = 0;//dlg.GameKind; // 아템 노템
		//ri.GameKind = dlg.m_nStyle;
		//if(ri.Secret==TRUE) strncpy(ri.Pass, dlg.m_Pass.operator LPCTSTR(), 4);
		ri.State = 0;
		ri.FormKind = 1;//dlg.FormKind;
		ri.bSuperMaster = 0;//dlg.m_bSuperRoomMaster;	// [게임 아이템 작업]
		
		ri.bPlayer = 0;//dlg.m_nPlayer; // 0=5,1=6 		

		// ### [사운드 추가 작업] ###
		// port(UINT) ->  SndFxKind 변경 
		// ### [ 관전기능 ] ###
		msg.Set(&ri, g_MyInfo.nSndFxKind);

		// 메인서버에게 방생성 요청을 함
		SockMan.SendData(g_MainSrvSID, msg.pData, msg.GetTotalSize());

		// 방에 접속 혹은 방을 생성 중임을 나타냄
		iAccessRoomState=1;
		g_Wait = TRUE;
	}
	/* softpark
	else
	{
		iAccessRoomState=0;
		g_Wait = FALSE;
	}
	*/
	
}

void CLobyDlg::OnRclickListRoom(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	nRoomSel = pNMListView->iItem;
	*pResult = 0;
	
	if(nRoomSel < 0)
	{
		m_RoomMenu.GetSubMenu(0)->EnableMenuItem(IDC_BUTTON_ROOMINFO, MF_GRAYED);
		m_RoomMenu.GetSubMenu(0)->EnableMenuItem(IDC_BUTTON_ENTERGAME, MF_GRAYED);
	}
	else
	{
		m_RoomMenu.GetSubMenu(0)->EnableMenuItem(IDC_BUTTON_ROOMINFO, MF_ENABLED);
		m_RoomMenu.GetSubMenu(0)->EnableMenuItem(IDC_BUTTON_ENTERGAME, MF_ENABLED);
	}

	// 팝업 메뉴
	CPoint pnt;
	GetCursorPos(&pnt);

	//### [관리자 모드 작업] ###
	if(g_bAdminUser){
		if(nRoomSel < 0){
			m_RoomMenu.GetSubMenu(3)->EnableMenuItem(IDC_MENU_ADM_ROOMINFO	, MF_GRAYED);
			m_RoomMenu.GetSubMenu(3)->EnableMenuItem(IDC_MENU_ADM_INROOM	, MF_GRAYED);
			m_RoomMenu.GetSubMenu(3)->EnableMenuItem(IDC_MENU_ADM_WARN		, MF_GRAYED);
			m_RoomMenu.GetSubMenu(3)->EnableMenuItem(IDC_MENU_ADM_RENAMEROOM, MF_GRAYED);
			m_RoomMenu.GetSubMenu(3)->EnableMenuItem(IDC_MENU_ADM_ROOMDEL	, MF_GRAYED);
		//	m_RoomMenu.GetSubMenu(3)->EnableMenuItem(IDC_MENU_ADM_MAKEROOM	, MF_GRAYED);
			m_RoomMenu.GetSubMenu(3)->EnableMenuItem(IDC_MENU_ADM_ROOMMASTER, MF_GRAYED);

		}
		else{
			m_RoomMenu.GetSubMenu(3)->EnableMenuItem(IDC_MENU_ADM_ROOMINFO	, MF_GRAYED);
			m_RoomMenu.GetSubMenu(3)->EnableMenuItem(IDC_MENU_ADM_INROOM	, MF_GRAYED);
			m_RoomMenu.GetSubMenu(3)->EnableMenuItem(IDC_MENU_ADM_WARN		, MF_GRAYED);
			m_RoomMenu.GetSubMenu(3)->EnableMenuItem(IDC_MENU_ADM_RENAMEROOM, MF_ENABLED);
			m_RoomMenu.GetSubMenu(3)->EnableMenuItem(IDC_MENU_ADM_ROOMDEL	, MF_ENABLED);
			m_RoomMenu.GetSubMenu(3)->EnableMenuItem(IDC_MENU_ADM_MAKEROOM	, MF_GRAYED);
			m_RoomMenu.GetSubMenu(3)->EnableMenuItem(IDC_MENU_ADM_ROOMMASTER, MF_GRAYED);
		}
		m_RoomMenu.GetSubMenu(3)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pnt.x, pnt.y, this);
	}
	else{
		m_RoomMenu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pnt.x, pnt.y, this);
	}

	//m_RoomMenu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pnt.x, pnt.y, this);
}


void CLobyDlg::OnButtonHompage() 
{
	// TODO: Add your control notification handler code here
	// 홈페이지 자동 띄우기
	char strDir[500]={0,};
	GetCurrentDirectory(500, strDir);
	CString runstr;
	runstr.Format("%s\\Homepage.url", strDir);
	::ShellExecute(NULL, "open", runstr.operator LPCTSTR(), NULL, ".", SW_SHOWNORMAL);
}

void CLobyDlg::OnButtonViewprofile() 
{
	if(g_Wait == TRUE) return;
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

	GetDlgItem(IDC_EDIT_FINDID)->SetWindowText(szID);// ### [ 관전기능 ] ###

	CSV_ASK_USERINFO aumsg;
	aumsg.Set(UNum, (char*)szID.operator LPCTSTR());
	SockMan.SendData(g_MainSrvSID, aumsg.pData, aumsg.GetTotalSize());
}

void CLobyDlg::OnButtonWhisper() 
{
	// TODO: Add your command handler code here
	Sound.Play(SND45);

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

void CLobyDlg::OnButtonChangeprofile() 
{
	// TODO: Add your control notification handler code here
	if(g_Wait == TRUE) return;

	Sound.Play(SND45);
/*
	CChangeCharDlg Dlg;
	int rtn = Dlg.DoModal();
	if(rtn==IDOK)
	{
		if(Dlg.NowSel == Play[0].UI.Character) return;
		CSV_ASK_CHANGECHAR accmsg;
		accmsg.Set(Play[0].UI.UNum, Play[0].UI.ID, Dlg.NowSel);
		SockMan.SendData(g_MainSrvSID, accmsg.pData, accmsg.GetTotalSize());
		g_Wait = TRUE;
	}
*/
}

void CLobyDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	Back.DeleteObject();

	KillTimer( LOBY_TIMER );
}

void CLobyDlg::OnRclickListUser(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	nUserSel = pNMListView->iItem;
	*pResult = 0;
	
	if(nUserSel < 0)
	{
		m_RoomMenu.GetSubMenu(1)->EnableMenuItem(IDC_BUTTON_VIEWPROFILE, MF_GRAYED);
		m_RoomMenu.GetSubMenu(1)->EnableMenuItem(IDC_BUTTON_WHISPER, MF_GRAYED);
	}
	else
	{
		m_RoomMenu.GetSubMenu(1)->EnableMenuItem(IDC_BUTTON_VIEWPROFILE, MF_ENABLED);
		m_RoomMenu.GetSubMenu(1)->EnableMenuItem(IDC_BUTTON_WHISPER, MF_ENABLED);
	}

	// 팝업 메뉴
	CPoint pnt;
	GetCursorPos(&pnt);

	//### [관리자 모드 작업] ###
	if(g_bAdminUser){
		if(nUserSel < 0){
			m_RoomMenu.GetSubMenu(2)->EnableMenuItem(IDC_MENU_ADM_USERINFO,      MF_GRAYED);
			m_RoomMenu.GetSubMenu(2)->EnableMenuItem(IDC_MENU_ADM_SENDWHISPHER,  MF_GRAYED);
			m_RoomMenu.GetSubMenu(2)->EnableMenuItem(IDC_MENU_ADM_WARN_USER,	 MF_GRAYED);
		}
		else{
			m_RoomMenu.GetSubMenu(2)->EnableMenuItem(IDC_MENU_ADM_USERINFO,      MF_GRAYED);
			m_RoomMenu.GetSubMenu(2)->EnableMenuItem(IDC_MENU_ADM_SENDWHISPHER,  MF_GRAYED);

			m_RoomMenu.GetSubMenu(2)->EnableMenuItem(IDC_MENU_ADM_WARN_USER, MF_ENABLED);
		}
		m_RoomMenu.GetSubMenu(2)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pnt.x, pnt.y, this);
	}
	else{
		m_RoomMenu.GetSubMenu(1)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pnt.x, pnt.y, this);
	}


//	m_RoomMenu.GetSubMenu(1)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pnt.x, pnt.y, this);
}


void CLobyDlg::OnButtonChangedisplay() 
{
	if(!bDontClickSound) Sound.Play(SND45);
	bDontClickSound = FALSE;

	static CRect OrgRect;
	static LONG orgstyle, orgExstyle;
	// TODO: Add your control notification handler code here
	if(Display.DDRAWINIT==TRUE)
	{
		// 변경되기 전의 윈도우 속성으로 복구하고 원래의 위치로 이동
		::SetWindowLong(g_pMainDlg->m_hWnd, GWL_STYLE, orgstyle);
		::SetWindowLong(g_pMainDlg->m_hWnd, GWL_EXSTYLE, orgExstyle);
		g_pMainDlg->SetWindowPos(&wndNoTopMost,OrgRect.left, OrgRect.top, OrgRect.right, OrgRect.bottom, SWP_SHOWWINDOW);
		g_pMainDlg->MoveWindow(OrgRect);
		Display.CloseDirectDraw();
		// 버튼 초기화
		m_ChangeDisplayBtn.Init(718,3,".\\image\\Loby\\LBtn_fullscreen.bmp", 4);
		g_pTitleDlg->m_ChangeDisplayBtn.Init(694, 504,".\\image\\Title\\TBtn_fullscreen.bmp", 4);
		
		g_pGameView->ChangeDisplayBtn.SetSprite(&FullScreenBtnSpr,0);	// ### [ 관전기능 ] ###
		g_pGameDlg->m_ObserverDlg.ShowWindow(SW_HIDE);					// ### [ 관전기능 ] ###
		g_pGameDlg->m_InviteDlg.ShowWindow(SW_HIDE);					// 초대 대화창 감춤
		
		// 변경된 화면모드와 일치하는 DIB 섹션을 생성(속도 향상)
		g_pGameView->Page.ReCreateDIBSection();
		m_bFullScr = FALSE;
	}
	else 
	{
		// 변경되기 전의 원래 사이즈와 위치를 저장
		g_pMainDlg->GetWindowRect(OrgRect);

		// 타이틀바와 다이얼로그 경계속성을 제거
		orgstyle = ::GetWindowLong(g_pMainDlg->m_hWnd, GWL_STYLE);
		LONG newstyle =  orgstyle & (!WS_CAPTION);
		::SetWindowLong(g_pMainDlg->m_hWnd, GWL_STYLE, newstyle);
		orgExstyle = ::GetWindowLong(g_pMainDlg->m_hWnd, GWL_EXSTYLE);
		LONG newExstyle =  orgExstyle & (!WS_EX_OVERLAPPEDWINDOW);
		::SetWindowLong(g_pMainDlg->m_hWnd, GWL_EXSTYLE, newExstyle);

		g_pMainDlg->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW);

		if(Display.InitDirectDraw(g_pMainDlg->m_hWnd, 800, 600, 16)==TRUE)
		{
			CRect MovRect(0,0,800,600);
			g_pMainDlg->MoveWindow(MovRect);
			m_ChangeDisplayBtn.Init(718,3,".\\image\\Loby\\LBtn_winmode.bmp", 4);

			g_pTitleDlg->m_ChangeDisplayBtn.Init(694, 504,".\\image\\Title\\TBtn_winmode.bmp", 4);
			
			g_pGameView->ChangeDisplayBtn.SetSprite(&WindowBtnSpr,0);	// ### [ 관전기능 ] ###
			g_pGameDlg->m_ObserverDlg.ShowWindow(SW_HIDE);	// ### [ 관전기능 ] ###

			// 초대 대화창 감춤
			g_pGameDlg->m_InviteDlg.ShowWindow(SW_HIDE);
			// 변경된 화면모드와 일치하는 DIB 섹션을 생성(속도 향상)
			g_pGameView->Page.ReCreateDIBSection();
			m_bFullScr = TRUE;
		}
		else
		{
			// 변경되기 전의 윈도우 속성으로 복구하고 원래의 위치로 이동
			::SetWindowLong(g_pMainDlg->m_hWnd, GWL_STYLE, orgstyle);
			::SetWindowLong(g_pMainDlg->m_hWnd, GWL_EXSTYLE, orgExstyle);
			g_pMainDlg->MoveWindow(OrgRect);
			g_pMainDlg->CFG.nScreenMode = 0;
			g_pMainDlg->m_MsgBoxDlg.MsgBox(0,"화면모드 변경에 실패하였습니다.\nDirect-X가 설치되어있는지 확인하세요.");
		}
		
		g_pMainDlg->SetWindowPos(&CWnd::wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW);
	}
}

void CLobyDlg::OnButtonBaduser() 
{
	// TODO: Add your control notification handler code here
	Sound.Play(SND45);

	CBadUserDlg Dlg;
	if(Dlg.DoModal() == IDOK)
	{
		CString strwatchid, strchat;
		m_ChatViewEdit.GetWindowText(strchat);
		if(m_ChatViewEdit.GetLineCount()<4 || strchat.GetLength()<10){ g_pMainDlg->m_MsgBoxDlg.MsgBox(0,"신고할 내용이 너무 짧습니다"); return; }

		// 4000자 이상은 잘라냄
		if(strchat.GetLength()>4000) strchat = strchat.Mid(strchat.GetLength() - 4000);

		CSV_ASK_BADUSER abumsg;
		abumsg.Set(Dlg.m_bAnnounce, 0, Play[0].UI.UNum, Play[0].UI.ID,
			(char*)Dlg.m_TargetID.operator LPCTSTR(),
			(char*)Dlg.m_SubmitReason.operator LPCTSTR(),
			(char*)strwatchid.operator LPCTSTR(), (char*)strchat.operator LPCTSTR());
		SockMan.SendData(g_MainSrvSID, abumsg.pData, abumsg.GetTotalSize());

		// 신고기능 이용회수 카운터 증가
		Dlg.SubmitCnt++;
		// 신고기능 딜레이 타이머 세팅
		g_pMainDlg->ResetBadUserTimer();
	}
}

void CLobyDlg::OnButtonFindid() 
{
	// TODO: Add your control notification handler code here
	Sound.Play(SND45);
	
	CString FindID;
	GetDlgItem(IDC_EDIT_FINDID)->GetWindowText(FindID);
	if(FindID.GetLength()==0) return;

	CResultFindIdDlg Dlg;

	int num=-1;
	CString szRN;
	for(;;)
	{
		num = m_UserList.GetNextItem(num, LVNI_ALL);
		if(num == -1) break;
		CString szID = m_UserList.GetItemText(num, 0);
		if(FindID.Compare(szID)==0)
		{
			// 선택된 셀렉션으로 마크하고 선택된 리스트가 확실히 보이도록 한다
			m_UserList.SetItemState(num, LVIS_SELECTED, LVIS_SELECTED);
			m_UserList.EnsureVisible(num, FALSE);

			CString szWhere = m_UserList.GetItemText(num, 2);
			CString str;
			str.Format("[%s]님은 %s에 있습니다", (LPCTSTR)FindID, (LPCTSTR)szWhere);
			//::MessageBox(m_hWnd, (LPCTSTR)str, "사용자를 찾았습니다", MB_OK);
			Dlg.DoModal(str);
			return;
		}
	}

	szRN="존재하지 않는 사용자입니다.";
//	::MessageBox(m_hWnd, "존재하지 않는 사용자입니다.", "사용자 찾기 실패", MB_OK);
	Dlg.DoModal(szRN);
}


void CLobyDlg::OnCheckRefusewhisper() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CSV_NOWHISPER nwmsg;
	nwmsg.Set(m_RefuseWhisper);
	SockMan.SendData(g_MainSrvSID, nwmsg.pData, nwmsg.GetTotalSize());
	g_pMainDlg->CFG.bNoWhisper = m_RefuseWhisper;
}

void CLobyDlg::OnCheckRefuseinvite() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	g_pMainDlg->CFG.bNoInvite = m_RefuseInviteCheck;
}

void CLobyDlg::OnButtonCard() 
{
	// TODO: Add your control notification handler code here
	
}




void CLobyDlg::OnButtonOption() 
{
	// TODO: Add your control notification handler code here
	Sound.Play(SND45);
	COptionDlg Dlg;
	Dlg.DoModal();
}

void CLobyDlg::OnButtonExit1() 
{
	// TODO: Add your control notification handler code here

	
	
#ifdef _DEBUG
/*
	g_Wait = TRUE;
	Sound.Play(SND45);

	CSV_ASK_LOGOUT alomsg;
	// ### [ 관전기능 ] ###
	alomsg.Set(g_MyInfo.UI.UNum, g_MyInfo.UI.ID);
	SockMan.SendData(g_MainSrvSID, alomsg.pData, alomsg.GetTotalSize());
	*/
	PostMessage(WM_COMMAND, IDC_BUTTON_LOGOUT, 0);

#else if
	
	PostMessage(WM_COMMAND, IDC_BUTTON_LOGOUT, 0);
/*
	if(g_Wait) return;
	g_Wait = TRUE;
	Sound.Play(SND45);

	CSV_ASK_OUTCHAN aomsg;
	aomsg.Set();
	SockMan.SendData(g_MainSrvSID, aomsg.pData, aomsg.GetTotalSize());
*/
#endif
	
	
}

//#include "Oring.h"

// [###바로시작###] // ### [ 관전기능 ] ###
void CLobyDlg::OnButtonQuickstart() 
{

	// TODO: Add your control notification handler code here
	//Sound.Play(SND45);

	// ### [ 관전기능 ] ###
	CSV_ASK_QUICKSTART	msg;
	msg.Set( g_MyInfo.UI.UNum, g_MyInfo.UI.ID, g_MyInfo.nSndFxKind , m_ChoiceKind);// ### [사운드 추가 작업] ###
	SockMan.SendData(g_MainSrvSID, msg.pData, msg.GetTotalSize());


	return;
	
}

// [ ###바로시작### ]
void CLobyDlg::CreateRoom()
{
	OnButtonCreateroom();
}

void CLobyDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rc;
	rc.SetRect( 0, 0, 635, 28 );
	POINT pt;
	pt.x = point.x;
	pt.y = point.y;
	if( rc.PtInRect( pt ) )
	{
		g_pMainDlg->SendMessage( WM_LBUTTONDOWN, MK_LBUTTON, 0 );
	}
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CLobyDlg::OnButtonMinimize() 
{
	// TODO: Add your control notification handler code here
	g_pMainDlg->ShowWindow(SW_MINIMIZE);
	
}

void CLobyDlg::OnButtonItembag() 
{
	// TODO: Add your control notification handler code here
	g_pMainDlg->ShowMyItemBag();
}

void CLobyDlg::DrawBkgnd(CDC& dc)
{
	/*
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;

	if(Back.m_hObject) Back.DeleteObject();
	Back.LoadBitmapFile(".\\image\\Loby\\lobyback.bmp");
	CBitmap *pOldBmp = cdc.SelectObject(&Back);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);

	// 폰트 생성
	CFont Font1;
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
		"굴림");

	dc.SetBkMode(TRANSPARENT);

	if( Play[0].UI.PMoney < 500 )
		m_QuickStart.EnableWindow(FALSE);
	else
		m_QuickStart.EnableWindow(TRUE);

	
	CString str;
	CString strM="";
	CFont* pOldFont = dc.SelectObject(&Font1);
	
	dc.SetTextColor(RGB(255,255,255));
	rect.SetRect(0,0,200,80);
	rect.OffsetRect(560, 525);
	strM = NumberToOrientalString(g_MyInfo.UI.PMoney);
	str.Format("%s원", strM);// ### [ 관전기능 ] ###	
	dc.DrawText(str, &rect, DT_RIGHT | DT_WORDBREAK);	
	
	rect.SetRect(0,0,200,80);
	rect.OffsetRect(560, 500);
	str.Format("[알림] S키를 눌러야 입장 가능");// ### [ 관전기능 ] ###	
	dc.DrawText(str, &rect, DT_RIGHT | DT_WORDBREAK);

	Font1.DeleteObject();
	cdc.SelectObject(pOldBmp);
	cdc.DeleteDC();
	Back.DeleteObject();
	*/
}

// Draw Insert Coin  - jeong
void CLobyDlg::DrawEffect(CDC& dc)
{
	CMyBitmap bmpInsertCoin;
	bmpInsertCoin.LoadBitmapFile(".\\image\\insertcoin.bmp");	
	
	if(bmpInsertCoin.m_hObject == NULL) return;
	
	if( Play[0].UI.PMoney < 500 )
	{
		if((m_nLobyCnt/15)%2  == 0 )
			bmpInsertCoin.TransDraw(dc.m_hDC, 300, 550, 230, 35, 0, 0, RGB(248,0,248));
	}
	
	bmpInsertCoin.DeleteObject();
}

BOOL CLobyDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	//DrawBkgnd(*pDC);
	DrawEffect(*pDC);
	return TRUE;

	
	//return CDialog::OnEraseBkgnd(pDC);
}

void CLobyDlg::OnButtonScreencaptureloby() 
{
	// TODO: Add your control notification handler code here
	g_pMainDlg->DoScreenCapture();
	
}




/////////////////////////////////////////////////////////////////////////////
//////////////////////////    [관리자 모드 작업]    /////////////////////////
/////////////////////////////////////////////////////////////////////////////
//유저관련
void CLobyDlg::OnMenuAdmUserinfo() 
{
	SendMessage(WM_COMMAND, IDC_BUTTON_VIEWPROFILE);
}

void CLobyDlg::OnMenuAdmSendwhispher() 
{
	OnButtonWhisper();
}

void CLobyDlg::OnMenuAdmWarnUser() // 사용자 관리하기
{
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
	
	if(strcmp(g_MyInfo.UI.ID, szID) == 0) 
	{
		AfxMessageBox("잘하자공");
		return;
	}
	g_pAMes->Show(UNum, szID);
}


//[관리자 모드 작업]
int CLobyDlg::GetUserNum(CString strID)
{
	int UNum = -1;
	if(strID.GetLength()<=0 || strID.GetLength()>15) return -1;

	int nFindIndex = -1;
	for(int i=0;i<m_UserList.GetItemCount();i++){
		if(m_UserList.GetItemText(i,0)==strID)
			nFindIndex = i;
	}
	if(nFindIndex>=0 && nFindIndex<m_UserList.GetItemCount())
		UNum = m_UserList.GetItemData(nFindIndex);

	return UNum;
}

// 방스타일을 구한다
BOOL CLobyDlg::GetStyleRoomInfo(int rnum, STYLEROOMINFO &sri)
{
	if(rnum<1 || rnum>=MAX_ROOM) return FALSE;

	memset(&sri,0,sizeof(STYLEROOMINFO));

	STYLEROOMINFO tmpri ={ 0,};

	// 선택된 아이템중 첫번째 아이템을 얻는다
	POSITION pos = m_RoomList.GetFirstSelectedItemPosition();
	if (pos == NULL) return FALSE;
	nRoomSel = m_RoomList.GetNextSelectedItem(pos);
	if(nRoomSel < 0) return FALSE;
	if(nRoomSel >= m_RoomList.GetItemCount()) return FALSE;
	CString rstr;
	rstr = m_RoomList.GetItemText(nRoomSel, 0);
	if(rstr.GetLength()==0) return FALSE;
	int nowrnum = ::atoi(rstr.operator LPCTSTR());
	if(nowrnum < 0) return FALSE;

	tmpri.RoomNum = nowrnum;
	rstr = m_RoomList.GetItemText(nRoomSel, 1);
	strncpy(tmpri.ID, rstr,15);       // 방장아이디
	rstr = m_RoomList.GetItemText(nRoomSel, 2);
	strncpy(tmpri.Title, rstr, 31); // 방타이틀

	if(tmpri.RoomNum != rnum || strlen(tmpri.ID)==0) return FALSE; // 방번호가 틀리거나, 방장이 없다면
	memcpy(&sri, &tmpri, sizeof(STYLEROOMINFO));
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////






/*
// 방관련
void CLobyDlg::OnMenuAdmRenameroom() // 방제 변경하기
{
	// TODO: Add your command handler code here
	// 선택된 아이템중 첫번째 아이템을 얻는다
	POSITION pos = m_RoomList.GetFirstSelectedItemPosition();
	if (pos == NULL) return;
	nRoomSel = m_RoomList.GetNextSelectedItem(pos);

	if(nRoomSel < 0) return;
	if(nRoomSel >= m_RoomList.GetItemCount()) return;

	CString rstr;
	rstr = m_RoomList.GetItemText(nRoomSel, 0);
	if(rstr.GetLength()==0) return;
	int rnum = ::atoi(rstr.operator LPCTSTR());
	if(rnum < 0) return;

	STYLEROOMINFO sri; memset(&sri,0,sizeof(sri));

	if(!GetStyleRoomInfo(rnum,sri)) return;

	CAdminTitleDlg dlg(sri.Title);

	if(dlg.DoModal()==IDCANCEL) return;

	// 다시한번 방정보를 확인하자!!
	if(!GetStyleRoomInfo(rnum,sri)) return;

	strcpy(sri.Title,dlg.m_strName.operator LPCTSTR());

	ADMINMESSAGE amsg={0,};
	amsg.AdminUNum = g_MyInfo.UI.UNum;
	strncpy(amsg.AdminID, g_MyInfo.UI.ID, 15);


	CNM_ADMIN_CHANGEROOM croom;
	croom.Set(&amsg, &sri);
	SockMan.SendData(g_MainSrvSID, croom.pData, croom.GetTotalSize());


	
}

void CLobyDlg::OnMenuAdmRoomdel() // 방제거하기
{
	// 선택된 아이템중 첫번째 아이템을 얻는다
	POSITION pos = m_RoomList.GetFirstSelectedItemPosition();
	if (pos == NULL) return;
	nRoomSel = m_RoomList.GetNextSelectedItem(pos);

	if(nRoomSel < 0) return;
	if(nRoomSel >= m_RoomList.GetItemCount()) return;

	CString rstr;
	rstr = m_RoomList.GetItemText(nRoomSel, 0);
	if(rstr.GetLength()==0) return;
	int rnum = ::atoi(rstr.operator LPCTSTR());
	if(rnum < 0) return;

	STYLEROOMINFO sri; memset(&sri,0,sizeof(sri));
	if(!GetStyleRoomInfo(rnum,sri)) return;
	CString strMess; strMess.Format(g_StrMan.Get(_T("ADMIN_DELROOM")), rnum, sri.Title);

	int result=AfxMessageBox(strMess, MB_YESNO);
	if(result==IDNO) return;

	// 다시한번 방정보를 확인하자!!
	if(!GetStyleRoomInfo(rnum,sri)) return;

	ADMINMESSAGE amsg={0,};
	amsg.AdminUNum = g_MyInfo.UI.UNum;
	strncpy(amsg.AdminID, g_MyInfo.UI.ID, 15);

	amsg.TarUNum = sri.RoomNum;			//방번호
	strncpy(amsg.TarID, sri.ID, 15);	// 방장 아이디


	CNM_ADMIN_DELROOM droom;
	droom.Set(&amsg);
	SockMan.SendData(g_MainSrvSID, droom.pData, droom.GetTotalSize());
}


void CLobyDlg::OnMenuAdmRoommaster()  // 방장에게 경고하기
{
	// TODO: Add your command handler code here

	
}


void CLobyDlg::OnMenuAdmInroom()  //방입장
{
	// TODO: Add your command handler code here
	OnButtonEntergame();
	
}
void CLobyDlg::OnMenuAdmMakeroom() //방만들기
{
	// TODO: Add your command handler code here
	OnButtonCreateroom();
	
}

void CLobyDlg::OnMenuAdmRoominfo() //방정보 보기
{
	// TODO: Add your command handler code here
	OnButtonRoominfo();
	
}

*/

void CLobyDlg::OnButtonAllkind() //바로 가기 설정 전체 
{
	// TODO: Add your control notification handler code here
	m_ChoiceKind = 0 ;
	m_btnAllKind.EnableWindow(FALSE);
	m_btnNormal.EnableWindow(TRUE);	
	m_btnChoice.EnableWindow(TRUE);
	
}

void CLobyDlg::OnButtonNormal() //바로 가기 설정 normal
{
	// TODO: Add your control notification handler code here
	m_ChoiceKind = 1 ;
	m_btnAllKind.EnableWindow(TRUE);
	m_btnNormal.EnableWindow(FALSE);	
	m_btnChoice.EnableWindow(TRUE);
}

void CLobyDlg::OnButtonChoice() //바로 가기 설정 choice
{
	// TODO: Add your control notification handler code here
	m_ChoiceKind = 2 ;
	m_btnAllKind.EnableWindow(TRUE);
	m_btnNormal.EnableWindow(TRUE);	
	m_btnChoice.EnableWindow(FALSE);
	
}



void CLobyDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	Invalidate(FALSE);
	m_nLobyCnt++;
	

	// 풀스크린
	if( m_nLobyCnt == 7 )
		SendMessage(WM_COMMAND, IDC_BUTTON_CHANGEDISPLAY);
	
	//CDialog::OnTimer(nIDEvent);
}

LRESULT CLobyDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CLobyDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class	
	if(pMsg->message == WM_KEYDOWN) 
	{
		if(pMsg->wParam == 'M' || pMsg->wParam == 'C')
		{
			// 서버에 플레이어정보 돈 추가  - jeong
			int money = pMsg->wParam=='M'?100:-100;
			Play[0].PrevMoney +=money;			
			CSV_ASK_MONEYINFO aumsg;
			aumsg.Set(Play[0].UI.UNum, money, g_RI.RoomNum);
			SockMan.SendData(g_MainSrvSID, aumsg.pData, aumsg.GetTotalSize());

		}
		else if(/*pMsg->wParam == VK_DOWN ||*/ pMsg->wParam == 'S' || pMsg->wParam == 's')
		{
			if( Play[0].UI.PMoney >= 500 )
				OnButtonQuickstart();
		}
		else if(pMsg->wParam == 'T' || pMsg->wParam == 't')
		{
			CAdminBtnTestDlg dlg(this);
			dlg.DoModal();
			return 1;
		}

	
		if(pMsg->wParam == VK_LEFT)
		{	
			m_nLobyBtnIndex--;
			
			if( m_nLobyBtnIndex < 0)
				m_nLobyBtnIndex = 1;
		}
		else if( pMsg->wParam == VK_RIGHT )
		{
			m_nLobyBtnIndex++;
			if( m_nLobyBtnIndex > 1)
				m_nLobyBtnIndex = 0;
		}
		else if( pMsg->wParam == VK_DOWN )
		{
			m_bLobyBtnMouseDown = TRUE;
		}
			
		
	}
		
	return CDialog::PreTranslateMessage(pMsg);
}

void CLobyDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnChar(nChar, nRepCnt, nFlags);
}

void CLobyDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CLobyDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here

	if (bShow)
		((C62CutPokerDlg *)AfxGetMainWnd())->m_hBaseWindow = m_hWnd;
	
}

void CLobyDlg::InitPage( )
{

	Page.Init(GAMEVIEW_WIDTH, GAMEVIEW_HEIGHT, 16);
	
	
	OutCoinBtn.Init(this, &Page, 591, 457, &OutCoinSpr, 0,IDM_OUT_COIN);
	OutCoinBtn.Show(TRUE);
	OutCoinBtn.m_Width = 170;
	OutCoinBtn.m_Height = 44;
	
	QuickStartBtn.Init(this, &Page, 345, 401, &QuickStartSpr, 0,IDM_QUICK_START);
	QuickStartBtn.Show(TRUE);
	QuickStartBtn.m_Width = 100;
	QuickStartBtn.m_Height = 44;
	
}


BOOL CLobyDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(wParam)
	{
		case IDM_OUT_COIN:
		{
			Play[0].BankMoney = g_MyInfo.BankMoney;
			int nCoin = Play[0].BankMoney/100;
			int nExtra = Play[0].BankMoney%100;
			if (nCoin>0)
			{
				g_MyInfo.BankMoney -= nCoin*100;

				CSV_ASK_MONEYINFO aumsg;
				aumsg.Set(Play[0].UI.UNum, -(nCoin*100), g_RI.RoomNum);
				SockMan.SendData(g_MainSrvSID, aumsg.pData, aumsg.GetTotalSize());
				
				// Bank 서버로 전송
				CSV_ASK_BANKINFO abmsg;
				abmsg.Set(Play[0].UI.UNum, nExtra);
				SockMan.SendData(g_MainSrvSID, abmsg.pData, abmsg.GetTotalSize());

				((C62CutPokerDlg *)AfxGetMainWnd())->m_clsRS232.OutCoin(nCoin);
			}	

			//g_MyInfo.BankMoney = 0;
		}
		break;
		
		case IDM_QUICK_START:
		{
			if( g_MyInfo.UI.PMoney >= 500)
				OnButtonQuickstart();

			
		}
		break;
	}

	return CDialog::OnCommand(wParam, lParam);
}

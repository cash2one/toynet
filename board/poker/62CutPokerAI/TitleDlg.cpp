// TitleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "TitleDlg.h"

#include "OptionDlg.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTitleDlg dialog

CTitleDlg::CTitleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTitleDlg::IDD, pParent)
{
	g_pTitleDlg = this;
	m_pLocWebCtrl = NULL;	// <- 추가
	m_pWebCtrl = NULL;
	LastSelChan = -1;
	m_TitleMsg ="";
	//{{AFX_DATA_INIT(CTitleDlg)
	m_strFindID = _T("");
	//}}AFX_DATA_INIT
}


void CTitleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTitleDlg)
	DDX_Control(pDX, IDC_RICHEDIT_NOTIC, m_ChatViewNotic);
	DDX_Control(pDX, IDC_LIST_CHANNEL, m_ChanList);
	DDX_Control(pDX, IDC_BUTTON_EXIT, m_ExitBtn);
	DDX_Control(pDX, IDC_BUTTON_ENTER, m_EnterBtn);
	DDX_Control(pDX, IDC_BUTTON_REFRESH, m_RefreshBtn);
	DDX_Control(pDX, IDC_BUTTON_CHANGEDISPLAY, m_ChangeDisplayBtn);
	DDX_Control(pDX, IDC_BUTTON_OPTION, m_OptionBtn);
	DDX_Control(pDX, IDC_BUTTON_FIND, m_FindIDBtn);
	DDX_Text(pDX, IDC_EDIT_FINDID, m_strFindID);
	DDV_MaxChars(pDX, m_strFindID, 15);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTitleDlg, CDialog)
	//{{AFX_MSG_MAP(CTitleDlg)
	ON_WM_PAINT()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CHANNEL, OnDblclkListChannel)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, OnButtonRefresh)
	ON_BN_CLICKED(IDC_BUTTON_ENTER, OnButtonEnter)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_FIND, OnButtonFind)
	ON_BN_CLICKED(IDC_BUTTON_OPTION, OnButtonOption)
	ON_BN_CLICKED(IDC_BUTTON_CHANGEDISPLAY, OnButtonChangedisplay)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_WINDOWPOSCHANGING()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTitleDlg message handlers

void CTitleDlg::OnOK() 
{
	// TODO: Add extra validation here
	//CDialog::OnOK();
}

void CTitleDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	//CDialog::OnCancel();
}

BOOL CTitleDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here

	// 다이얼로그 컨트롤 포지션과 위치 재정렬
	CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);
	
	// 버튼 초기화
/*
	m_FindIDBtn.Init(IDB_TITLE_FINDID1, IDB_TITLE_FINDID2, IDB_TITLE_FINDID3, NULL);
	m_OptionBtn.Init(IDB_TITLE_OPTION1, IDB_TITLE_OPTION2, IDB_TITLE_OPTION3, NULL);
	m_ChangeDisplayBtn.Init(IDB_TITLE_FULLSCREEN1, IDB_TITLE_FULLSCREEN2, IDB_TITLE_FULLSCREEN3, NULL);
	m_RefreshBtn.Init(IDB_TITLE_REFRESH1, IDB_TITLE_REFRESH2, IDB_TITLE_REFRESH3, NULL);
	m_EnterBtn.Init(IDB_TITLE_ENTERCHAN1, IDB_TITLE_ENTERCHAN2, IDB_TITLE_ENTERCHAN3, NULL);
	m_ExitBtn.Init(IDB_TITLE_EXIT1, IDB_TITLE_EXIT2, IDB_TITLE_EXIT3, NULL);
*/
	m_FindIDBtn.Init(453, 504,".\\image\\Title\\TBtn_findid.bmp", 4);
	m_OptionBtn.Init(596, 502,".\\image\\Title\\TBtn_option.BMP", 4);
	m_ChangeDisplayBtn.Init(693, 502,".\\image\\Title\\TBtn_fullscreen.bmp", 4);
	m_RefreshBtn.Init(20, 554,".\\image\\Title\\TBtn_refresh.bmp", 4);
	m_EnterBtn.Init(193, 554,".\\image\\Title\\TBtn_channel.bmp", 4);
	m_ExitBtn.Init(653, 554,".\\image\\Title\\TBtn_exit.bmp", 4);

	Chan_InitList();
	LastSelChan = -1;
	bDontClickSound = FALSE;

	/*
	// 웹 컨트롤 생성
	CRect rc(0,0,450,400);
	//rc.OffsetRect(328,83);
	rc.OffsetRect(328,91);

	if(!m_pWebCtrl) m_pWebCtrl = new CWebCtrl();
	m_pWebCtrl->Create(NULL, "WebControl", WS_VISIBLE|WS_CHILD, rc, this, 1321);
*/
	//m_pWebCtrl->Navigate("http://www.netmarble.net/gamedown/notice/poweral/notice.htm");

	// 위치 알림용 웹 컨트롤 생성(보이지 않는 곳으로 옮겨둠)
	CRect rc(0,0,80,80);
	//rc.OffsetRect(0,0);
	if(!m_pLocWebCtrl) m_pLocWebCtrl = new CWebCtrl();
	m_pLocWebCtrl->Create(NULL, "WebControl", WS_VISIBLE|WS_CHILD, rc, this, 1322);
	m_pLocWebCtrl->ShowWindow(SW_HIDE);

	// 타이틀 배너 웹 컨트롤 생성
	CreateBanner();

	
	// [ 세븐 포커 ] 잭팟 테스트
	rc.SetRect(0, 0, 241, 40);	
	rc.OffsetRect(270 , 37);
	m_JackPotBox.Create(this, rc, 0, 1223);
	
	// 채팅 뷰 리치 에디트 컨트롤 초기화
	m_ChatViewNotic.SetReadOnly(TRUE);
	m_ChatViewNotic.SetBackgroundColor(FALSE, RGB(61,116,161));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTitleDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	// 웹 컨트롤 파괴
	//if(m_pWebCtrl) { delete m_pWebCtrl; m_pWebCtrl = NULL; }
	
	// 위치 알림용 웹 컨트롤 파괴
	if(m_pLocWebCtrl) { 
		m_pLocWebCtrl->Navigate("about:blank");
		delete m_pLocWebCtrl; m_pLocWebCtrl = NULL; 
	}
	// 타이틀 배너 웹 컨트롤 파괴
	DestroyBanner();
}

void CTitleDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	DrawBkgnd(dc);

	// Do not call CDialog::OnPaint() for painting messages
}

void CTitleDlg::Chan_InitList()
{
	// 비트맵 읽기
	CBitmap Bmp;
	Bmp.LoadBitmap(IDB_IMAGE_CHAN);

	// 이미지 리스트 생성
	m_ChanImage.Create(32, 16, ILC_COLOR8 | ILC_MASK, 11, 0);
	m_ChanImage.Add(&Bmp, RGB(255,255,255));

	m_ChanList.SetImageList(&m_ChanImage, LVSIL_SMALL);
	m_ChanList.SetFullRowSel(TRUE); 	// ### [ 채널 입장 구분 ] ###
	m_ChanList.SetTextBkColor(RGB(45,33,55));
//	m_ChanList.SetBkColor(RGB(45,33,55));
//	m_ChanList.SetTextColor(RGB(235,255,235));

	// 전체행 셀렉트 설정
	m_ChanList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	// ### [ 채널 입장 구분 ] ###
	//////////////////////////////////////////////////////////////////////////////
	//m_ChanList.Init(FALSE, TRUE, 0, RGB(45,33,55), RGB(235,255,235));
	m_ChanList.Init(FALSE, TRUE, 0, RGB(255,255,255), RGB(235,255,235));
	m_ChanList.SetHeaderColors(RGB(210,210,255), RGB(0,0,0));
	m_ChanList.SetScrollBarChange(TRUE);
	//////////////////////////////////////////////////////////////////////////////

	//// Column 삽입부
	LV_COLUMN col;
	char szName[256];

	wsprintf(szName, "상태");
	
	col.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_WIDTH | LVCF_TEXT;
	col.fmt  = LVCFMT_LEFT;
	col.cx   = 40;
	col.pszText = szName;
	col.iSubItem = 0;
	m_ChanList.InsertColumn(0, &col);

	col.fmt  = LVCFMT_CENTER;
	col.cx   = 140;
	wsprintf(szName, "채널명");
	col.iSubItem = 1;
	m_ChanList.InsertColumn(2, &col);

	col.fmt  = LVCFMT_RIGHT;
	col.cx   = 60;
	wsprintf(szName, "인원");
	col.iSubItem = 2;
	m_ChanList.InsertColumn(4, &col);
}

void CTitleDlg::Chan_InsItem(int cnum, char *title, int nowuser, int maxuser, int channelstyle)
{
	// 레코드를 리스트 뷰에 삽입
	LV_ITEM item;
	char szName[256]={0,};

	int image = nowuser / (maxuser/10);
	if(nowuser % (maxuser/10)) image += 1;

	// 채널번호
	//wsprintf(szName, "%d", cnum);
	wsprintf(szName, " ");
	item.mask = LVIF_TEXT | LVIF_IMAGE;
	item.pszText = szName;
	item.iItem = m_ChanList.GetItemCount();
	item.iSubItem = 0;
	item.iImage=image;

	// 2003 0620 카드게임 채널 표시 및 점프 아이템 사용 구분
	int nItem = m_ChanList.InsertItem(&item);
	int grade = GetMyLeveltoGrade(Play[0].UI.nIcon); // 등급 구함
	
	//////////////////////////////////////////////////////////////////////////////
	// ### [ 채널 입장 구분 ] ###
	// 채널 리스트 색깔 변경
	if(channelstyle == 3 && grade == 4) {// 도신은 고수 서버 입장가능(포커2)
		m_ChanList.SetItemTextColor(nItem, RGB(0,0,0));	
	}
/*
	else if(channelstyle == 1 && channelstyle < grade) // 초보전용채널 (하이로우, 훌라)
	{// 입장불가
		m_ChanList.SetItemTextColor(nItem, RGB(128,128,128));	
	}
	else if(channelstyle == 0 || channelstyle <= grade )  // 하위채널 모두 입장 가능(하이로우, 훌라)
*/	else if(channelstyle == 0 || channelstyle == grade )  {// 자유채널, 입장가능 등급 (포커2)
		m_ChanList.SetItemTextColor(nItem, RGB(0,0,0));	
	}
	else if(channelstyle == grade+1)  { // 점프 사용가능
		m_ChanList.SetItemTextColor(nItem, RGB(255,128,0));	
	}
	else { // 입장불가
		m_ChanList.SetItemTextColor(nItem, RGB(128,128,128));	
	}
	//////////////////////////////////////////////////////////////////////////////

	// 채널명
	wsprintf(szName, "%s", title);
	item.iSubItem = 1;
	m_ChanList.SetItem(&item);

	//////////////////////////////////////////////////////////////////////////////
	// 채널 리스트 색깔 변경
//	m_ChanList.SetItemTextColor(nItem, RGB(235,255,235));	// [게임 아이템 작업]
	/////////////////////////////////////////////////////////////////////////

	// 인원
	wsprintf(szName, "%d/%d", nowuser, maxuser);
	item.iSubItem = 2;
	m_ChanList.SetItem(&item);

	// 채널 번호 세팅
	m_ChanList.SetItemData(item.iItem, cnum);

}

void CTitleDlg::Chan_DeleteList()
{
	m_ChanList.DeleteAllItems();
}

void CTitleDlg::Chan_SelectLast()
{
	if(LastSelChan != -1) {
		m_ChanList.SetItemState(LastSelChan, LVIS_SELECTED, LVIS_SELECTED);
	}
}

CString CTitleDlg::GetChannelName(int cnum)
{
	CString str;
	if(cnum < 0) return str;
	
	int num = -1;
	for(;;) {
		num = m_ChanList.GetNextItem(num, LVNI_ALL);
		if(num == -1) break;
		if(cnum == (int)m_ChanList.GetItemData(num)) {
			str = m_ChanList.GetItemText(num, 1);
			break;
		}
	}

	return str;
}

void CTitleDlg::OnDblclkListChannel(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnButtonEnter();
	*pResult = 0;
}

void CTitleDlg::OnButtonRefresh() 
{
	// TODO: Add your control notification handler code here

	if(g_Where != WH_TITLE) return; // <- 추가
	if(g_bChannel ) return; // <- 추가  아이템 사용시 버튼 비활성화

	if(!bDontClickSound) Sound.Play(SND45);
	bDontClickSound = FALSE;

	// 마지막 선택 항목을 저장
	// 선택된 아이템중 첫번째 아이템을 얻는다
	POSITION pos = m_ChanList.GetFirstSelectedItemPosition();
	if(pos == NULL) LastSelChan = -1;
	else LastSelChan = m_ChanList.GetNextSelectedItem(pos);

	Chan_DeleteList();

	// 전체 채널 목록을 요구함
	CSV_ASK_ALLCHANINFO acmsg;
	acmsg.Set();
	SockMan.SendData(g_MainSrvSID, acmsg.pData, acmsg.GetTotalSize());

	GetDlgItem(IDC_BUTTON_REFRESH)->EnableWindow(FALSE);

	// 메시지
	m_MsgDlg.MsgBox(3, "전체 채널 목록을 내려받는 중입니다");
}

void CTitleDlg::OnButtonEnter() 
{
	// TODO: Add your control notification handler code here
	if(g_Wait == TRUE) return;
	if(g_Where != WH_TITLE) return; // <- 추가
	if(g_bChannel ) return; // <- 추가  아이템 사용시 버튼 비활성화

	Sound.Play(SND45);

	// 선택된 아이템중 첫번째 아이템을 얻는다
	POSITION pos = m_ChanList.GetFirstSelectedItemPosition();
	if (pos == NULL) return;
	int nNowSel = m_ChanList.GetNextSelectedItem(pos);
	
	if(nNowSel < 0) return;
	if(nNowSel >= m_ChanList.GetItemCount()) return;

	int cnum = m_ChanList.GetItemData(nNowSel);
	if(cnum < 0) return;

	g_Wait = TRUE;

	GetDlgItem(IDC_BUTTON_ENTER)->EnableWindow(FALSE);

	// 채널에 입장하기를 요구
	CSV_ASK_ENTERCHAN aemsg;
	aemsg.Set(cnum);
	SockMan.SendData(g_MainSrvSID, aemsg.pData, aemsg.GetTotalSize());

	// 메시지
	m_MsgDlg.MsgBox(3, "요청한 채널에 연결중입니다");
}

void CTitleDlg::OnButtonFind() 
{
	// TODO: Add your control notification handler code here
	Sound.Play(SND45);
	
	UpdateData(TRUE);
	if(m_strFindID.GetLength() <= 0) return;

	GetDlgItem(IDC_EDIT_FINDID)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_FIND)->EnableWindow(FALSE);

	CSV_ASK_FINDID afmsg;
	afmsg.Set((char*)(LPCTSTR)m_strFindID);
	SockMan.SendData(g_MainSrvSID, afmsg.pData, afmsg.GetTotalSize());
}

void CTitleDlg::OnButtonOption() 
{
	// TODO: Add your control notification handler code here
	Sound.Play(SND45);
	COptionDlg Dlg;
	Dlg.DoModal();
}

void CTitleDlg::OnButtonChangedisplay() 
{
	// TODO: Add your control notification handler code here
	g_pLobyDlg->SendMessage(WM_COMMAND, IDC_BUTTON_CHANGEDISPLAY);
}

void CTitleDlg::OnButtonExit() 
{
	// TODO: Add your control notification handler code here
	Sound.Play(SND45);
	g_pMainDlg->OnCancel();
}

// 이 함수를 추가
void CTitleDlg::CreateBanner()
{
	if(m_pWebCtrl) return;

	// 웹 컨트롤 생성
	CRect rc(0,0,450,400);
	rc.OffsetRect(329,93);
	if(!m_pWebCtrl) m_pWebCtrl = new CWebCtrl();
	m_pWebCtrl->Create(NULL, "WebControl", WS_VISIBLE|WS_CHILD, rc, this, 1321);
}

// 이 함수를 추가
void CTitleDlg::DestroyBanner()
{
	if(!m_pWebCtrl) return;
	delete m_pWebCtrl;
	m_pWebCtrl = NULL;
}



void CTitleDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rc;
	rc.SetRect( 0, 0, 790, 28 );
	POINT pt;
	pt.x = point.x;
	pt.y = point.y;
	if( rc.PtInRect( pt ) )
	{
		g_pMainDlg->SendMessage( WM_LBUTTONDOWN, MK_LBUTTON, 0 );
	}
	
	CDialog::OnLButtonDown(nFlags, point);
}


void CTitleDlg::DrawBkgnd(CDC& dc)
{
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;
	
	if(Back.m_hObject) Back.DeleteObject();
	Back.LoadBitmapFile(".\\image\\Title\\TitleBack.bmp");
	CBitmap *pOldBmp = cdc.SelectObject(&Back);

	
//	dc.TextOut(30, 18, m_TitleMsg);

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


	CFont* pOldFont = dc.SelectObject(&Font1);

	//////////////////////////////////////////////
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor( RGB(255, 255, 255) );
	dc.TextOut(120, 8, m_TitleMsg);
	//////////////////////////////////////////////

	dc.SelectObject(pOldFont);
	Font1.DeleteObject();


	cdc.SelectObject(pOldBmp);
	cdc.DeleteDC();
	Back.DeleteObject();
}

BOOL CTitleDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	DrawBkgnd(*pDC);
	return TRUE;
	
	//return CDialog::OnEraseBkgnd(pDC);
}

void CTitleDlg::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	CDialog::OnWindowPosChanging(lpwndpos);
	
	// TODO: Add your message handler code here
//	lpwndpos->flags &= ~SWP_SHOWWINDOW;
	
}

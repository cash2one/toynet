// GameItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPoker.h"
#include "GameItemDlg.h"
#include "DialogCtrlReposition.h"
#include "MyBitmap.h"
#include "SysUtil.h"

#include "Global.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 배경 이미지 파일명
static const LPCTSTR BACKIMAGE_NAME = "image\\gameitem\\GameItemBack.bmp";

/////////////////////////////////////////////////////////////////////////////
// CGameItemDlg dialog


CGameItemDlg::CGameItemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGameItemDlg::IDD, pParent)
{
	m_bModal = FALSE;
	m_pHelpWeb = NULL;
	//m_pShopWeb = NULL;
	m_CurrentTab = 0;
	m_DefShopItemKind = 0;

	//{{AFX_DATA_INIT(CGameItemDlg)
	a = NULL;
	//}}AFX_DATA_INIT
}


CGameItemDlg::~CGameItemDlg()
{
}

void CGameItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGameItemDlg)
	DDX_Control(pDX, IDC_BUTTON_CASH, m_CashBtn);
	DDX_Control(pDX, IDC_BUTTON_SHOP, m_ShopBtn);
	DDX_Control(pDX, IDC_BUTTON_ITEM, m_ItemBtn);
	DDX_Control(pDX, IDC_BUTTON_HELP, m_HelpBtn);
	DDX_Control(pDX, IDCANCEL, m_CancelBtn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGameItemDlg, CDialog)
	//{{AFX_MSG_MAP(CGameItemDlg)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_HELP, OnButtonHelp)
	ON_BN_CLICKED(IDC_BUTTON_ITEM, OnButtonItem)
	ON_BN_CLICKED(IDC_BUTTON_SHOP, OnButtonShop)
	ON_BN_CLICKED(IDC_BUTTON_CASH, OnButtonCash)
	ON_WM_PAINT()
	ON_WM_WINDOWPOSCHANGING()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGameItemDlg message handlers

BOOL CGameItemDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	// 다이얼로그 컨트롤 포지션과 위치 재정렬
	CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);

	// 배경 비트맵 사이즈에 맞게 윈도우 사이즈 조정
	WindowResize2Bitmap(m_hWnd, BACKIMAGE_NAME);

	// 버튼 초기화
	m_CancelBtn.Init(455, 5, "image\\gameitem\\btn_exit.bmp", 4);

	// 탭용 버튼 초기화
	m_HelpBtn.Init( 11, 23, "image\\gameitem\\btn_help.bmp", 4);
	m_ItemBtn.Init(111, 23, "image\\gameitem\\btn_item.bmp", 4);
	m_ShopBtn.Init(211, 23, "image\\gameitem\\btn_shop.bmp", 4);
	m_CashBtn.Init(311, 23, "image\\gameitem\\btn_cash.bmp", 4);

	// 차일드 다이얼로그 생성
	{
		// 아이템 도움말 웹 컨트롤 생성
		CRect helprc(0,0,100,100);
		helprc.OffsetRect(0,0);
		if(!m_pHelpWeb) m_pHelpWeb = new CWebCtrl();
		m_pHelpWeb->Create(NULL, "ItemHelp", WS_VISIBLE|WS_CHILD, helprc, this, 1331);

		// 아이템 리스트 다이얼로그 생성
		m_ItemListDlg.Create(m_ItemListDlg.IDD, this);

		// 상점 웹 컨트롤 생성
		CRect shoprc(0,0,100,100);
		shoprc.OffsetRect(0,0);		
	}

	//CRect rc;
	//GetClientRect(&rc);
	//m_GameItemTab.MoveWindow(&rc);

	// 탭 컨트롤 내에 붙어야할 윈도우를 탭 클라이언트 영역 내로 사이즈 조정
	{
		CRect rc(0,0,463,355);
		rc.OffsetRect(9,58);
		//m_GameItemTab.GetClientRect(&rc);
		//m_GameItemTab.AdjustRect(FALSE, &rc);
		//rc.DeflateRect(5,5,5,5);

		if(m_pHelpWeb) m_pHelpWeb->MoveWindow(&rc, TRUE);
		if(m_ItemListDlg.GetSafeHwnd()) m_ItemListDlg.MoveWindow(&rc, TRUE);
	}

	///////////////

	m_bHelpWebNavigated = FALSE;
	m_bShopWebNavigated = FALSE;

	///////////////

	ChangeTab(m_FirstTab);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGameItemDlg::OnOK() 
{
	// TODO: Add extra validation here
}

void CGameItemDlg::OnCancel() 
{
	// TODO: Add extra cleanup here

	// 메인 다이얼로그가 뒤로 사라지는 경우를 막기 위함
	if(g_pMainDlg->GetSafeHwnd()) {
		g_pMainDlg->SetFocus();
	}

	if(!m_bModal) {
		DestroyWindow();
		return;
	}

	CDialog::OnCancel();
}

int CGameItemDlg::DoModal(int firsttab, BOOL bRefreshList, int iDefSelItemKind) 
{
	// TODO: Add your specialized code here and/or call the base class

	// 이미 다이얼로그가 생성된 상태라면 파괴	[프리미엄 작업]
	if(GetSafeHwnd()) SendMessage(WM_COMMAND, IDCANCEL);

	m_bModal = TRUE;

	m_FirstTab = firsttab;
	m_bRefreshList = bRefreshList;

	m_DefShopItemKind = 0;
	m_ItemListDlg.m_DefSelItemKind = 0;

	// 아이템이 리프레쉬 될 때 적용되도록 저장
	m_ItemListDlg.m_DefSelItemKind = iDefSelItemKind;
	// 아이템 상점에서 특정 아이템이 제일 위에 리스트업 되도록 설정
	m_DefShopItemKind = iDefSelItemKind;

	return CDialog::DoModal();
}

BOOL CGameItemDlg::DoModaless(int firsttab, BOOL bRefreshList, int iDefSelItemKind)
{
	if(GetSafeHwnd()) 
		return FALSE;

	m_bModal = FALSE;

	m_FirstTab = firsttab;
	m_bRefreshList = bRefreshList;

	m_DefShopItemKind = 0;
	m_ItemListDlg.m_DefSelItemKind = 0;

	// 아이템이 리프레쉬 될 때 적용되도록 저장
	m_ItemListDlg.m_DefSelItemKind = iDefSelItemKind;
	// 아이템 상점에서 특정 아이템이 제일 위에 리스트업 되도록 설정
	m_DefShopItemKind = iDefSelItemKind;

	BOOL rtn = Create(IDD, g_pMainDlg);
	if(rtn) ShowWindow(SW_SHOW);

	return rtn;
}

void CGameItemDlg::OpenShopPage() 
{
	#if (0)
	{
		//
		// 인자를 POST 방식으로 전달하는 예제 (사용 안함)
		//
	
		//m_pShopWeb->Navigate("about:blank");
		
		// 한글 캐릭터 셋으로 설정
		m_pShopWeb->PutCharSet("euc-kr");

		// Html 코드 삽입 - POST 방식으로 인자를 전달하기 위함
		CString strHtml;
		strHtml.Format("<Form Name='Send' Method='POST' Action='%s'>\
			<Input type=hidden Name='id' value='%s'>\
			<Input type=hidden Name='pass' value='%s'>\
			<Input type=hidden Name='gamecode' value='%d'>\
			</Form>", 
			g_ItemURL, g_LoginID, g_LoginPass, g_GameCode);

		m_pShopWeb->InsertHTML(strHtml);

		// 페이지 호출
		char* strScript = "document.Send.submit();";
		m_pShopWeb->ExecJavascript(strScript);
	}
	#else 
	{

		CString strurl;
		strurl.Format("%s?gamecode=%d", g_ItemURL, g_GameCode);

		if(m_DefShopItemKind > 0) {
			// 기본 아이템이 설정되어 있다면 최상단에 리스트 될 수 있도록 인자 추가
			CString addstr;
			addstr.Format("&default=%d", m_DefShopItemKind);
			strurl += addstr;
		}
	}
	#endif

}

void CGameItemDlg::ItemRefresh()
{
	if(m_bRefreshList) {
		// 한번이라도 웹으로 네비게이션을 해야만 아이템 리스트의 재전송 요청
		m_ItemListDlg.AskRefresh();
		// 카운터 초기화		
		m_bRefreshList = FALSE;
	}
}

void CGameItemDlg::ChangeTab(int tabno)
{
	// 모든 윈도우를 숨김
	m_pHelpWeb->ShowWindow(SW_HIDE);
	m_ItemListDlg.ShowWindow(SW_HIDE);

	// 모든 탭버튼을 활성화
	m_HelpBtn.EnableWindow(TRUE);
	m_ItemBtn.EnableWindow(TRUE);	

	if(tabno == 0) {
		m_pHelpWeb->ShowWindow(SW_SHOW);
		m_HelpBtn.EnableWindow(FALSE);
		
		if( !m_bHelpWebNavigated ) {
			// 최초로 웹이 오픈되는 경우만 아이템 도움말 URL을 오픈함
			CString strurl;
			strurl.Format("%s?gamecode=%d", g_ItemHelpURL, g_GameCode);
			m_pHelpWeb->Navigate(strurl);

			m_bHelpWebNavigated = TRUE;
		}
	}
	if(tabno == 1) {
		m_ItemListDlg.ShowWindow(SW_SHOW);
		m_ItemBtn.EnableWindow(FALSE);
		
		// 아이템 리프레쉬
		ItemRefresh();
	}
	if(tabno == 2) {
		m_ShopBtn.EnableWindow(FALSE);
		
		if( !m_bShopWebNavigated ) {
			
			// [PC방 작업] - 기존 코드를 삭제하고 새로 덮어 씌울것

			{
				/*
				// 최초로 웹이 오픈되는 경우만 아이템 상점 URL을 오픈함
				// (끝에 '&'가 붙음 - 인코딩시 '/'가 붙어서 URL이 오픈되지 않는 현상을 없애기 위함)
				CString strurl;
				strurl.Format("%s?id=%s&pass=%s&gamecode=%d&", g_ItemURL, g_LoginID, g_LoginPass, g_GameCode);
				char openurl[256]={0,};
				g_pMainDlg->URLEncode(openurl, (char*)(LPCTSTR)strurl);
				m_pShopWeb->Navigate(openurl);

				m_pShopWeb->m_NavigatedCount = 0;
				m_bShopWebNavigated = TRUE;
				*/

				OpenShopPage();

	
				m_bShopWebNavigated = TRUE;
			}
		}
	}
	if(tabno == 3) {

		m_CashBtn.EnableWindow(FALSE);
	}

	m_CurrentTab = tabno;
}

BOOL CGameItemDlg::SafeClose()
{
	if(!GetSafeHwnd()) return FALSE;
	SendMessage(WM_COMMAND, IDCANCEL);
	return TRUE;
}

void CGameItemDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

}

void CGameItemDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here

	// 아이템 리프레쉬(구입한 아이템이 있는데 그냥 닫는 경우 리스트를 재전송 요청)
	ItemRefresh();

	if(m_pHelpWeb) { delete m_pHelpWeb; m_pHelpWeb = NULL; }	
	if(m_ItemListDlg.GetSafeHwnd()) m_ItemListDlg.DestroyWindow();
}

void CGameItemDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
	CDialog::OnLButtonDown(nFlags, point);
}

void CGameItemDlg::OnButtonHelp() 
{
	// TODO: Add your control notification handler code here
	ChangeTab(0);
}

void CGameItemDlg::OnButtonItem() 
{
	// TODO: Add your control notification handler code here
	ChangeTab(1);
}

void CGameItemDlg::OnButtonShop() 
{
	// TODO: Add your control notification handler code here
	ChangeTab(2);
}

void CGameItemDlg::OnButtonCash() 
{
	// TODO: Add your control notification handler code here
	ChangeTab(3);
}

void CGameItemDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) return;
	
	CMyBitmap	Back;
	Back.LoadBitmapFile(BACKIMAGE_NAME);
	CBitmap *pOldBmp = cdc.SelectObject(&Back);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);

	cdc.SelectObject(pOldBmp);
	cdc.DeleteDC();
	Back.DeleteObject();

	// Do not call CDialog::OnPaint() for painting messages
}

void CGameItemDlg::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	CDialog::OnWindowPosChanging(lpwndpos);
	
	// TODO: Add your message handler code here
//	lpwndpos->flags &= ~SWP_SHOWWINDOW;
	
}

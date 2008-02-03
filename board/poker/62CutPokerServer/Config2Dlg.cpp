// Config2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPokerServer.h"
#include "Config2Dlg.h"
#include "62CutPokerServerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfig2Dlg dialog

CConfig2Dlg::CConfig2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfig2Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfig2Dlg)
	m_NoticeURL = _T("");
	m_MaxUser = 0;
	m_MaxDBThread = 0;
	m_LoginDBQueNum = 0;
	m_bDBClose = FALSE;
	m_BannerURL1 = _T("");
	m_BannerURL2 = _T("");
	m_ProfileURL = _T("");
	m_strSeverInfoMsg = _T("");
	m_LocationURL = _T("");
	m_ItemURL = _T("");
	m_ItemHelpURL = _T("");
	m_CashURL = _T("");
	m_bDBClose2 = FALSE; //### [�α� ��Ͽ�] ###
	m_MaxDBThread2 = 0;
	m_LoginDBQueNum2 = 0;
	m_SameIPLimit = 0;
	m_strTitleNoticMsg = _T("");
	m_bUseMacAddr = FALSE;
	m_LeadersJoinURL = _T("");
	//}}AFX_DATA_INIT
}


void CConfig2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfig2Dlg)
	DDX_Text(pDX, IDC_EDIT_NOTICEURL, m_NoticeURL);
	DDV_MaxChars(pDX, m_NoticeURL, 63);
	DDX_Text(pDX, IDC_EDIT_MAXUSER, m_MaxUser);
	DDX_Text(pDX, IDC_EDIT_DBTHREADNUM, m_MaxDBThread);
	DDX_Text(pDX, IDC_EDIT_LOGINDBQUENUM, m_LoginDBQueNum);
	DDV_MinMaxInt(pDX, m_LoginDBQueNum, 1, 500);
	DDX_Check(pDX, IDC_CHECK_DBCLOSE, m_bDBClose);
	DDX_Text(pDX, IDC_EDIT_BANNERURL1, m_BannerURL1);
	DDV_MaxChars(pDX, m_BannerURL1, 63);
	DDX_Text(pDX, IDC_EDIT_BANNERURL2, m_BannerURL2);
	DDV_MaxChars(pDX, m_BannerURL2, 63);
	DDX_Text(pDX, IDC_EDIT_PROFILEURL, m_ProfileURL);
	DDV_MaxChars(pDX, m_ProfileURL, 63);
	DDX_Text(pDX, IDC_EDIT_SERVERINFO, m_strSeverInfoMsg);
	DDV_MaxChars(pDX, m_strSeverInfoMsg, 79);
	DDX_Text(pDX, IDC_EDIT_LOCATIONURL, m_LocationURL);
	DDV_MaxChars(pDX, m_LocationURL, 63);
	DDX_Text(pDX, IDC_EDIT_ITEMURL, m_ItemURL);
	DDV_MaxChars(pDX, m_ItemURL, 63);
	DDX_Text(pDX, IDC_EDIT_ITEMHELPURL, m_ItemHelpURL);
	DDV_MaxChars(pDX, m_ItemHelpURL, 63);
	DDX_Text(pDX, IDC_EDIT_CASHURL, m_CashURL);
	DDV_MaxChars(pDX, m_CashURL, 63);
	DDX_Check(pDX, IDC_CHECK_DBCLOSE2, m_bDBClose2); //### [�α� ��Ͽ�] ###
	DDX_Text(pDX, IDC_EDIT_DBTHREADNUM2, m_MaxDBThread2);
	DDX_Text(pDX, IDC_EDIT_LOGINDBQUENUM2, m_LoginDBQueNum2);
	DDX_Text(pDX, IDC_EDIT_SAMEIPLIMIT, m_SameIPLimit);
	DDV_MinMaxInt(pDX, m_SameIPLimit, 0, 9);
	DDX_Text(pDX, IDC_EDIT_TNOTIC, m_strTitleNoticMsg);
	DDX_Check(pDX, IDC_CHECK_MACADDR, m_bUseMacAddr);
	DDX_Text(pDX, IDC_EDIT_LEADERSJOIN, m_LeadersJoinURL);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfig2Dlg, CDialog)
	//{{AFX_MSG_MAP(CConfig2Dlg)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfig2Dlg message handlers

BOOL CConfig2Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_strSeverInfoMsg = g_pMainView->Cfg2.NoticeInfoMsg;

//	g_pMainView->m_strServerInfoMsg = m_strSeverInfoMsg;

	m_NoticeURL = g_pMainView->Cfg2.NoticeURL;
	m_MaxUser = g_pMainView->Cfg2.MaxUser;
	m_MaxDBThread = g_pMainView->Cfg2.MaxDBThread;
	m_LoginDBQueNum = g_pMainView->Cfg2.LoginDBQueNum;
	m_bDBClose = g_pMainView->Cfg2.bDBClose;
	m_BannerURL1 = g_pMainView->Cfg2.BannerURL1;
	m_BannerURL2 = g_pMainView->Cfg2.BannerURL2;
	m_ProfileURL = g_pMainView->Cfg2.ProfileURL; 
	m_LocationURL = g_pMainView->Cfg2.LocationURL;		// <- �߰�
	m_ItemURL = g_pMainView->Cfg2.ItemURL;
	m_CashURL = g_pMainView->Cfg2.CashURL;
	m_ItemHelpURL = g_pMainView->Cfg2.ItemHelpURL;
	
	//### [�α� ��Ͽ�] ###
	////////////////////////////////////////////////////
	m_MaxDBThread2 = g_pMainView->Cfg2.MaxDBThread2;
	m_LoginDBQueNum2 = g_pMainView->Cfg2.LoginDBQueNum2;
	m_bDBClose2 = g_pMainView->Cfg2.bDBClose2;
	////////////////////////////////////////////////////

	// ### [ �ߺ��� IP�� ] ###
	m_SameIPLimit = g_pMainView->Cfg2.SameIPLimit;
	m_strTitleNoticMsg = g_pMainView->Cfg2.TitleNotice; // Ÿ��Ʋ ����

	m_LeadersJoinURL = g_pMainView->Cfg2.LeadersJoinURL;

	m_bUseMacAddr = g_pMainView->Cfg2.UseMacLimit; // �ƾ�巹���˻�


	if(m_MaxUser < 1) m_MaxUser = 1;
	if(m_MaxUser > MAX_USER-1) m_MaxUser = MAX_USER-1;

	UpdateData(FALSE);

	CString str;
	str.Format("��(1-%d)", MAX_USER-1);
	GetDlgItem(IDC_STATIC_MAXUSER)->SetWindowText(str);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfig2Dlg::OnOK() 
{
	// TODO: Add extra validation here

	UpdateData(TRUE);

	CString str;
	str.Format("�����ο� ���ѹ����� 1-%d������Դϴ�", MAX_USER-1);
	if(m_MaxUser < 1) { AfxMessageBox(str); return; }
	if(m_MaxUser > MAX_USER-1) { AfxMessageBox(str); return; }
	
	str.Format("DB������ ���� ���ѹ����� %d-%d�������Դϴ�", MINIMUM_DBTHREAD, MAXIMUM_DBTHREAD);
	if(m_MaxDBThread < MINIMUM_DBTHREAD) { AfxMessageBox(str); return; }
	if(m_MaxDBThread > MAXIMUM_DBTHREAD) { AfxMessageBox(str); return; }

	// ���ŵ� DB ������ ������ �����Ų��
	if(DBMan.pDBThread && g_pMainView->Cfg2.MaxDBThread != m_MaxDBThread) {
		if(!DBMan.ChangeDBThreadNum(m_MaxDBThread)) {
			AfxMessageBox("Ȱ������ DB�������� ������ �����Ϸ��� ������ Ů�ϴ�");
			return;
		}
	}

	// ���ŵ� DB ������ ������ �����Ų��
	if(LogDBMan.pDBThread && g_pMainView->Cfg2.MaxDBThread2 != m_MaxDBThread2) {
		if(!LogDBMan.ChangeDBThreadNum(m_MaxDBThread2)) {
			AfxMessageBox("Ȱ������ �αױ�Ͽ� DB�������� ������ �����Ϸ��� ������ Ů�ϴ�");
			return;
		}
	}

	strncpy(g_pMainView->Cfg2.NoticeInfoMsg, m_strSeverInfoMsg, 79);
	//g_pMainView->m_strServerInfoMsg = m_strSeverInfoMsg;
	g_pMainView->ShowState(_T(m_strSeverInfoMsg.operator LPCTSTR()));


	strncpy(g_pMainView->Cfg2.NoticeURL, m_NoticeURL, 63);
	g_pMainView->Cfg2.MaxUser = m_MaxUser;
	g_pMainView->Cfg2.MaxDBThread = m_MaxDBThread;
	g_pMainView->Cfg2.LoginDBQueNum = m_LoginDBQueNum;
	g_pMainView->Cfg2.bDBClose = m_bDBClose;
	strncpy(g_pMainView->Cfg2.BannerURL1, m_BannerURL1, 63);
	strncpy(g_pMainView->Cfg2.BannerURL2, m_BannerURL2, 63);
	strncpy(g_pMainView->Cfg2.ProfileURL, m_ProfileURL, 63);
	strncpy(g_pMainView->Cfg2.LocationURL, m_LocationURL, 63);	// <- �߰�
	strncpy(g_pMainView->Cfg2.ItemURL, m_ItemURL, 63);
	strncpy(g_pMainView->Cfg2.ItemHelpURL, m_ItemHelpURL, 63);
	strncpy(g_pMainView->Cfg2.CashURL, m_CashURL, 63);

	strncpy(g_pMainView->Cfg2.LeadersJoinURL, m_LeadersJoinURL, 63);
	

	//### [�α� ��Ͽ�] ###
	///////////////////////////////////////////////
	g_pMainView->Cfg2.MaxDBThread2 = m_MaxDBThread2;
	g_pMainView->Cfg2.LoginDBQueNum2 = m_LoginDBQueNum2;
	g_pMainView->Cfg2.bDBClose2 = m_bDBClose2;
	///////////////////////////////////////////////

	// ### [ �ߺ��� IP�� ] ###
	g_pMainView->Cfg2.SameIPLimit = m_SameIPLimit;
	strncpy(g_pMainView->Cfg2.TitleNotice, m_strTitleNoticMsg, 1024-1); // Ÿ��Ʋ ����
	g_pMainView->Cfg2.UseMacLimit = m_bUseMacAddr; // �ƾ�巹���˻�

	FILE *fp = fopen( "game.cfg", "wb");
	if( fp == NULL ) { 
		AfxMessageBox("ȯ�漳�� ������ ����� �� �����ϴ�.");
		return;
	}
	fwrite(&g_pMainView->Cfg2, sizeof(CONFIG2), 1, fp);
	fclose(fp);
	
	CDialog::OnOK();
}

void CConfig2Dlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CConfig2Dlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
	CDialog::OnLButtonDown(nFlags, point);
}

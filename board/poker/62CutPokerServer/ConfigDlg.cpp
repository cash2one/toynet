// ConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPokerServer.h"
#include "ConfigDlg.h"
#include "62CutPokerServerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg dialog


CConfigDlg::CConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigDlg)
	m_GameCode = 0;
	m_ServerCode = 0;
	m_ServerName = _T("");
	m_ServerPort = 0;
	m_BadUserDSN = _T("");
	m_GameDSN = _T("");
	m_StatusDSN = _T("");
	m_UserDSN = _T("");
	m_BadUserPass = _T("");
	m_GamePass = _T("");
	m_StatusPass = _T("");
	m_UserPass = _T("");
	m_BadUserTable = _T("");
	m_GameTable = _T("");
	m_StatusTable = _T("");
	m_UserTable = _T("");
	m_BadUserUID = _T("");
	m_GameUID = _T("");
	m_StatusUID = _T("");
	m_UserUID = _T("");
	m_AvatarDSN = _T("");
	m_AvatarPass = _T("");
	m_AvatarTable = _T("");
	m_AvatarUID = _T("");
	m_nMSCode = -1;
	m_ItemDSN = _T("");
	m_ItemPass = _T("");
	m_ItemTable = _T("");
	m_ItemUID = _T("");
	m_PremiumDSN = _T("");			// [프리미엄 작업]
	m_PremiumPass = _T("");
	m_PremiumTable = _T("");
	m_PremiumUID = _T("");
	m_PremIPDSN = _T("");			// [PC방 작업]
	m_PremIPPass = _T("");
	m_PremIPTable = _T("");
	m_PremIPUID = _T("");
	m_strMaxMoney = _T("");
	m_bItem = FALSE;
	m_bScret = FALSE;
	m_bJump = FALSE;
	m_LogRecDSN = _T("");           // ### [로그 기록용] ###
	m_LogRecPass = _T("");
	m_LogRecTable = _T("");
	m_LogRecUID = _T("");
	m_bLogDB = FALSE;
	m_EventDSN = _T("");            // ### [이벤트 티켓] ###
	m_EventPass = _T("");
	m_EventTable = _T("");
	m_EventUID = _T("");
	m_bEventDB = FALSE;
	m_StatisticsDSN = _T("");		// [ 통계기록 ]
	m_StatisticsPass = _T("");		// [ 통계기록 ]
	m_StatisticsTable = _T("");		// [ 통계기록 ]
	m_StatisticsUID = _T("");		// [ 통계기록 ]
	m_bStatisticsDB = FALSE;		// [ 통계기록 ]
	m_PreUserLGMLogDSN = _T("");	// [ 총액제한]
	m_PreUserLGMLogPass = _T("");
	m_PreUserLGMLogTable = _T("");
	m_PreUserLGMLogUID = _T("");
	m_bPreUserLGMLogDB = FALSE;
	m_AllinChargeDSN = _T("");		// [수호천사]
	m_AllinChargePass = _T("");		// [수호천사]
	m_AllinChargeTable = _T("");		// [수호천사]
	m_AllinChargeUID = _T("");		// [수호천사]
	m_bAllinDB = FALSE;
	//}}AFX_DATA_INIT
}


void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigDlg)
	DDX_Text(pDX, IDC_EDIT_GAMECODE, m_GameCode);
	DDX_Text(pDX, IDC_EDIT_SERVERCODE, m_ServerCode);
	DDX_Text(pDX, IDC_EDIT_SERVERNAME, m_ServerName);
	DDV_MaxChars(pDX, m_ServerName, 19);
	DDX_Text(pDX, IDC_EDIT_SERVERPORT, m_ServerPort);
	DDX_Text(pDX, IDC_EDIT_BADUSERDB_DSN, m_BadUserDSN);
	DDV_MaxChars(pDX, m_BadUserDSN, 39);
	DDX_Text(pDX, IDC_EDIT_GAMEDB_DSN, m_GameDSN);
	DDV_MaxChars(pDX, m_GameDSN, 39);
	DDX_Text(pDX, IDC_EDIT_STATUSDB_DSN, m_StatusDSN);
	DDV_MaxChars(pDX, m_StatusDSN, 39);
	DDX_Text(pDX, IDC_EDIT_USERDB_DSN, m_UserDSN);
	DDV_MaxChars(pDX, m_UserDSN, 39);
	DDX_Text(pDX, IDC_EDIT_BADUSERDB_PASS, m_BadUserPass);
	DDV_MaxChars(pDX, m_BadUserPass, 19);
	DDX_Text(pDX, IDC_EDIT_GAMEDB_PASS, m_GamePass);
	DDV_MaxChars(pDX, m_GamePass, 19);
	DDX_Text(pDX, IDC_EDIT_STATUSDB_PASS, m_StatusPass);
	DDV_MaxChars(pDX, m_StatusPass, 19);
	DDX_Text(pDX, IDC_EDIT_USERDB_PASS, m_UserPass);
	DDV_MaxChars(pDX, m_UserPass, 19);
	DDX_Text(pDX, IDC_EDIT_BADUSERDB_TABLE, m_BadUserTable);
	DDV_MaxChars(pDX, m_BadUserTable, 39);
	DDX_Text(pDX, IDC_EDIT_GAMEDB_TABLE, m_GameTable);
	DDV_MaxChars(pDX, m_GameTable, 39);
	DDX_Text(pDX, IDC_EDIT_STATUSDB_TABLE, m_StatusTable);
	DDV_MaxChars(pDX, m_StatusTable, 39);
	DDX_Text(pDX, IDC_EDIT_USERDB_TABLE, m_UserTable);
	DDV_MaxChars(pDX, m_UserTable, 39);
	DDX_Text(pDX, IDC_EDIT_BADUSERDB_UID, m_BadUserUID);
	DDV_MaxChars(pDX, m_BadUserUID, 19);
	DDX_Text(pDX, IDC_EDIT_GAMEDB_UID, m_GameUID);
	DDV_MaxChars(pDX, m_GameUID, 19);
	DDX_Text(pDX, IDC_EDIT_STATUSDB_UID, m_StatusUID);
	DDV_MaxChars(pDX, m_StatusUID, 19);
	DDX_Text(pDX, IDC_EDIT_USERDB_UID, m_UserUID);
	DDV_MaxChars(pDX, m_UserUID, 19);
	DDX_Text(pDX, IDC_EDIT_AVATARDB_DSN, m_AvatarDSN);
	DDV_MaxChars(pDX, m_AvatarDSN, 39);
	DDX_Text(pDX, IDC_EDIT_AVATARDB_PASS, m_AvatarPass);
	DDV_MaxChars(pDX, m_AvatarPass, 19);
	DDX_Text(pDX, IDC_EDIT_AVATARDB_TABLE, m_AvatarTable);
	DDV_MaxChars(pDX, m_AvatarTable, 39);
	DDX_Text(pDX, IDC_EDIT_AVATARDB_UID, m_AvatarUID);
	DDV_MaxChars(pDX, m_AvatarUID, 19);
	DDX_CBIndex(pDX, IDC_COMBO_MSCODE, m_nMSCode);
	DDX_Text(pDX, IDC_EDIT_ITEMDB_DSN, m_ItemDSN);
	DDX_Text(pDX, IDC_EDIT_ITEMDB_PASS, m_ItemPass);
	DDX_Text(pDX, IDC_EDIT_ITEMDB_TABLE, m_ItemTable);
	DDX_Text(pDX, IDC_EDIT_ITEMDB_UID, m_ItemUID);
	DDX_Text(pDX, IDC_EDIT_PREMIUMDB_DSN, m_PremiumDSN);
	DDV_MaxChars(pDX, m_PremiumDSN, 39);
	DDX_Text(pDX, IDC_EDIT_PREMIUMDB_PASS, m_PremiumPass);
	DDV_MaxChars(pDX, m_PremiumPass, 19);
	DDX_Text(pDX, IDC_EDIT_PREMIUMDB_TABLE, m_PremiumTable);
	DDV_MaxChars(pDX, m_PremiumTable, 39);
	DDX_Text(pDX, IDC_EDIT_PREMIUMDB_UID, m_PremiumUID);
	DDV_MaxChars(pDX, m_PremiumUID, 19);
	DDX_Text(pDX, IDC_EDIT_PREMIPDB_DSN, m_PremIPDSN);
	DDV_MaxChars(pDX, m_PremIPDSN, 39);
	DDX_Text(pDX, IDC_EDIT_PREMIPDB_PASS, m_PremIPPass);
	DDV_MaxChars(pDX, m_PremIPPass, 19);
	DDX_Text(pDX, IDC_EDIT_PREMIPDB_TABLE, m_PremIPTable);
	DDV_MaxChars(pDX, m_PremIPTable, 39);
	DDX_Text(pDX, IDC_EDIT_PREMIPDB_UID, m_PremIPUID);
	DDV_MaxChars(pDX, m_PremIPUID, 19);
	DDX_Text(pDX, IDC_EDIT_MAXPMONEY, m_strMaxMoney);
	DDX_Check(pDX, IDC_CHECK_ITEM, m_bItem);
	DDX_Check(pDX, IDC_CHECK_SCRET, m_bScret);
	DDX_Check(pDX, IDC_CHECK_JUMP, m_bJump);
	DDX_Text(pDX, IDC_EDIT_LOGREC_DSN, m_LogRecDSN); 
	DDX_Text(pDX, IDC_EDIT_LOGREC_PASS, m_LogRecPass);
	DDX_Text(pDX, IDC_EDIT_LOGREC_TABLE, m_LogRecTable);
	DDX_Text(pDX, IDC_EDIT_LOGREC_UID, m_LogRecUID);
	DDX_Check(pDX, IDC_CHECK_LOG, m_bLogDB);
	DDX_Text(pDX, IDC_EDIT_EVENT_DSN, m_EventDSN); 
	DDX_Text(pDX, IDC_EDIT_EVENT_PASS, m_EventPass);
	DDX_Text(pDX, IDC_EDIT_EVENT_TABLE, m_EventTable);
	DDX_Text(pDX, IDC_EDIT_EVENT_UID, m_EventUID);
	DDX_Check(pDX, IDC_CHECK_EVENT, m_bEventDB);
	DDX_Text(pDX, IDC_EDIT_STATIST_DSN, m_StatisticsDSN);	  // [통계작업]
	DDX_Text(pDX, IDC_EDIT_STATIST_PASS, m_StatisticsPass);   // [통계작업]
	DDX_Text(pDX, IDC_EDIT_STATIST_TABLE, m_StatisticsTable); // [통계작업]
	DDX_Text(pDX, IDC_EDIT_STATIST_UID, m_StatisticsUID);     // [통계작업]
	DDX_Check(pDX, IDC_CHECK_STATIST, m_bStatisticsDB);       // [통계작업]
	DDX_Text(pDX, IDC_EDIT_PREUSERMLLOG_DSN, m_PreUserLGMLogDSN);
	DDX_Text(pDX, IDC_EDIT_PREUSERMLLOG_PASS, m_PreUserLGMLogPass);
	DDX_Text(pDX, IDC_EDIT_PREUSERMLLOG_TABLE, m_PreUserLGMLogTable);
	DDX_Text(pDX, IDC_EDIT_PREUSERMLLOG_UID, m_PreUserLGMLogUID);
	DDX_Check(pDX, IDC_CHECK_PREUSERMLLOG, m_bPreUserLGMLogDB);
	DDX_Text(pDX, IDC_EDIT_ALLINCHARGE_DSN, m_AllinChargeDSN);	  // [수호천사]
	DDX_Text(pDX, IDC_EDIT_ALLINCHARGE_PASS, m_AllinChargePass);   // [수호천사]
	DDX_Text(pDX, IDC_EDIT_ALLINCHARGE_TABLE, m_AllinChargeTable); // [수호천사]
	DDX_Text(pDX, IDC_EDIT_ALLINCHARGE_UID, m_AllinChargeUID);     // [수호천사]
	DDX_Check(pDX, IDC_CHECK_ALLIN, m_bAllinDB);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CConfigDlg)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg message handlers

BOOL CConfigDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_ServerName = g_pMainView->Cfg.ServerName;
	m_GameCode = g_pMainView->Cfg.GameCode;
	m_ServerCode = g_pMainView->Cfg.ServerCode;
	m_ServerPort = g_pMainView->Cfg.ServerPort;

	m_UserDSN = g_pMainView->Cfg.UserDSN;
	m_UserTable = g_pMainView->Cfg.UserTable;
	m_UserUID = g_pMainView->Cfg.UserUID;
	m_UserPass = g_pMainView->Cfg.UserPass;

	m_GameDSN = g_pMainView->Cfg.GameDSN;
	m_GameTable = g_pMainView->Cfg.GameTable;
	m_GameUID = g_pMainView->Cfg.GameUID;
	m_GamePass = g_pMainView->Cfg.GamePass;

	m_BadUserDSN = g_pMainView->Cfg.BadUserDSN;
	m_BadUserTable = g_pMainView->Cfg.BadUserTable;
	m_BadUserUID = g_pMainView->Cfg.BadUserUID;
	m_BadUserPass = g_pMainView->Cfg.BadUserPass;

	m_StatusDSN = g_pMainView->Cfg.StatusDSN;
	m_StatusTable = g_pMainView->Cfg.StatusTable;
	m_StatusUID = g_pMainView->Cfg.StatusUID;
	m_StatusPass = g_pMainView->Cfg.StatusPass;

	m_AvatarDSN = g_pMainView->Cfg.AvatarDSN;
	m_AvatarTable = g_pMainView->Cfg.AvatarTable;
	m_AvatarUID = g_pMainView->Cfg.AvatarUID;
	m_AvatarPass = g_pMainView->Cfg.AvatarPass;

	// [프리미엄 작업]
	m_PremiumDSN = g_pMainView->Cfg.PremiumDSN;
	m_PremiumTable = g_pMainView->Cfg.PremiumTable;
	m_PremiumUID = g_pMainView->Cfg.PremiumUID;
	m_PremiumPass = g_pMainView->Cfg.PremiumPass;

	// [PC방 작업]
	m_PremIPDSN = g_pMainView->Cfg.PremIPDSN;
	m_PremIPTable = g_pMainView->Cfg.PremIPTable;
	m_PremIPUID = g_pMainView->Cfg.PremIPUID;
	m_PremIPPass = g_pMainView->Cfg.PremIPPass;

	// [게임 아이템 작업] 게임아이템 정보 읽기
//	m_bItem = g_pMainView->Cfg.bItemServer = 0; // <- 현재 사용 하지 않는다.
	// [비공개방 아이템]
//	m_bScret = g_pMainView->Cfg.bItemScret;
	// [점프 아이템 작업]
//	m_bJump = g_pMainView->Cfg.bJumpItem;
	m_ItemDSN = g_pMainView->Cfg.ItemDSN;
	m_ItemTable = g_pMainView->Cfg.ItemTable;
	m_ItemUID = g_pMainView->Cfg.ItemUID;
	m_ItemPass = g_pMainView->Cfg.ItemPass;


	// ### [로그 기록용] ###
	m_LogRecDSN = g_pMainView->Cfg.LogRecDSN;
	m_LogRecTable = g_pMainView->Cfg.LogRecTable;
	m_LogRecUID = g_pMainView->Cfg.LogRecUID;
	m_LogRecPass = g_pMainView->Cfg.LogRecPass;
	m_bLogDB = g_pMainView->Cfg.bLogUseDB;			


	// ### [이벤트 티켓] ###
	m_EventDSN = g_pMainView->Cfg.EventDSN;
	m_EventTable = g_pMainView->Cfg.EventTable;
	m_EventUID = g_pMainView->Cfg.EventUID;
	m_EventPass = g_pMainView->Cfg.EventPass;
	m_bEventDB = g_pMainView->Cfg.bEventDB;			

	// [통계작업]
	m_StatisticsDSN = g_pMainView->Cfg.StatisticsDSN;
	m_StatisticsTable = g_pMainView->Cfg.StatisticsTable;
	m_StatisticsUID = g_pMainView->Cfg.StatisticsUID;
	m_StatisticsPass = g_pMainView->Cfg.StatisticsPass;
	m_bStatisticsDB = g_pMainView->Cfg.bStatisticsDB;

	//2004.05.07 리더스총액 제한
	m_PreUserLGMLogDSN = g_pMainView->Cfg.PreUserLGMLogDSN;
	m_PreUserLGMLogTable = g_pMainView->Cfg.PreUserLGMLogTable;
	m_PreUserLGMLogUID = g_pMainView->Cfg.PreUserLGMLogUID;
	m_PreUserLGMLogPass = g_pMainView->Cfg.PreUserLGMLogPass;
	m_bPreUserLGMLogDB = g_pMainView->Cfg.bPreUserLGMLogDB;

	//수호천사 
	m_AllinChargeDSN = g_pMainView->Cfg.AllinChargeDSN;		// [수호천사]
	m_AllinChargePass = g_pMainView->Cfg.AllinChargePass;		// [수호천사]
	m_AllinChargeTable = g_pMainView->Cfg.AllinChargeTable;		// [수호천사]
	m_AllinChargeUID = g_pMainView->Cfg.AllinChargeUID;		// [수호천사]	
	m_bAllinDB = g_pMainView->Cfg.bAllinDB;	



	g_pMainView->Cfg.MAX_PMONEY = M_1H; // 제한머니 1경

	// [ $$$$ 포커머니 오버플로우 체크 $$$$ ]
	m_strMaxMoney.Format(_T("%I64d"),g_pMainView->Cfg.MAX_PMONEY );
	m_nMSCode = g_pMainView->Cfg.nMSCode;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	CONFIG tempCfg;
	ZeroMemory(&tempCfg, sizeof(CONFIG));

	strcpy(tempCfg.ServerName, (LPCTSTR)m_ServerName);
	tempCfg.GameCode = m_GameCode;
	tempCfg.ServerCode = m_ServerCode;
	tempCfg.ServerPort = m_ServerPort;
	
	strcpy(tempCfg.UserDSN, (LPCTSTR)m_UserDSN);
	strcpy(tempCfg.UserTable, (LPCTSTR)m_UserTable);
	strcpy(tempCfg.UserUID, (LPCTSTR)m_UserUID);
	strcpy(tempCfg.UserPass, (LPCTSTR)m_UserPass);
	
	strcpy(tempCfg.GameDSN, (LPCTSTR)m_GameDSN);
	strcpy(tempCfg.GameTable, (LPCTSTR)m_GameTable);
	strcpy(tempCfg.GameUID, (LPCTSTR)m_GameUID);
	strcpy(tempCfg.GamePass, (LPCTSTR)m_GamePass);
	
	strcpy(tempCfg.BadUserDSN, (LPCTSTR)m_BadUserDSN);
	strcpy(tempCfg.BadUserTable, (LPCTSTR)m_BadUserTable);
	strcpy(tempCfg.BadUserUID, (LPCTSTR)m_BadUserUID);
	strcpy(tempCfg.BadUserPass, (LPCTSTR)m_BadUserPass);
	
	strcpy(tempCfg.StatusDSN, (LPCTSTR)m_StatusDSN);
	strcpy(tempCfg.StatusTable, (LPCTSTR)m_StatusTable);
	strcpy(tempCfg.StatusUID, (LPCTSTR)m_StatusUID);
	strcpy(tempCfg.StatusPass, (LPCTSTR)m_StatusPass);

	strcpy(tempCfg.AvatarDSN, (LPCTSTR)m_AvatarDSN);
	strcpy(tempCfg.AvatarTable, (LPCTSTR)m_AvatarTable);
	strcpy(tempCfg.AvatarUID, (LPCTSTR)m_AvatarUID);
	strcpy(tempCfg.AvatarPass, (LPCTSTR)m_AvatarPass);

	// [게임 아이템 작업] 게임아이템 정보 읽기
//	tempCfg.bItemServer = m_bItem;
	// [비공개방 아이템]
//	tempCfg.bItemScret = m_bScret;
	// [점프 아이템]
//	tempCfg.bJumpItem = m_bJump;
	
	strcpy(tempCfg.ItemDSN, (LPCTSTR)m_ItemDSN);
	strcpy(tempCfg.ItemTable, (LPCTSTR)m_ItemTable);
	strcpy(tempCfg.ItemUID, (LPCTSTR)m_ItemUID);
	strcpy(tempCfg.ItemPass, (LPCTSTR)m_ItemPass);

	// [프리미엄 작업]
	strcpy(tempCfg.PremiumDSN, (LPCTSTR)m_PremiumDSN);
	strcpy(tempCfg.PremiumTable, (LPCTSTR)m_PremiumTable);
	strcpy(tempCfg.PremiumUID, (LPCTSTR)m_PremiumUID);
	strcpy(tempCfg.PremiumPass, (LPCTSTR)m_PremiumPass);

	// [PC방 작업]
	strcpy(tempCfg.PremIPDSN, (LPCTSTR)m_PremIPDSN);
	strcpy(tempCfg.PremIPTable, (LPCTSTR)m_PremIPTable);
	strcpy(tempCfg.PremIPUID, (LPCTSTR)m_PremIPUID);
	strcpy(tempCfg.PremIPPass, (LPCTSTR)m_PremIPPass);


	// ### [로그 기록용] ###
	strcpy(tempCfg.LogRecDSN, (LPCTSTR)m_LogRecDSN);
	strcpy(tempCfg.LogRecTable, (LPCTSTR)m_LogRecTable);
	strcpy(tempCfg.LogRecUID, (LPCTSTR)m_LogRecUID);
	strcpy(tempCfg.LogRecPass, (LPCTSTR)m_LogRecPass);
	tempCfg.bLogUseDB = m_bLogDB;

	// ### [이벤트 티켓] ###
	strcpy(tempCfg.EventDSN, (LPCTSTR)m_EventDSN);
	strcpy(tempCfg.EventTable, (LPCTSTR)m_EventTable);
	strcpy(tempCfg.EventUID, (LPCTSTR)m_EventUID);
	strcpy(tempCfg.EventPass, (LPCTSTR)m_EventPass);
	tempCfg.bEventDB = m_bEventDB;

	// [통계작업]
	strcpy(tempCfg.StatisticsDSN, (LPCTSTR)m_StatisticsDSN);
	strcpy(tempCfg.StatisticsTable, (LPCTSTR)m_StatisticsTable);
	strcpy(tempCfg.StatisticsUID, (LPCTSTR)m_StatisticsUID);
	strcpy(tempCfg.StatisticsPass, (LPCTSTR)m_StatisticsPass);
	tempCfg.bStatisticsDB = m_bStatisticsDB;

	//2004.05.07 리더스총액 제한
	strcpy(tempCfg.PreUserLGMLogDSN, (LPCTSTR)m_PreUserLGMLogDSN);
	strcpy(tempCfg.PreUserLGMLogTable, (LPCTSTR)m_PreUserLGMLogTable);
	strcpy(tempCfg.PreUserLGMLogUID, (LPCTSTR)m_PreUserLGMLogUID);
	strcpy(tempCfg.PreUserLGMLogPass, (LPCTSTR)m_PreUserLGMLogPass);
	tempCfg.bPreUserLGMLogDB = m_bPreUserLGMLogDB;

	//수호천사 
	strcpy(tempCfg.AllinChargeDSN, (LPCTSTR)m_AllinChargeDSN);		// [수호천사]
	strcpy(tempCfg.AllinChargePass, (LPCTSTR)m_AllinChargePass);	// [수호천사]
	strcpy(tempCfg.AllinChargeTable, (LPCTSTR)m_AllinChargeTable);	// [수호천사]
	strcpy(tempCfg.AllinChargeUID, (LPCTSTR)m_AllinChargeUID);		// [수호천사]
	tempCfg.bAllinDB = m_bAllinDB;
	
	tempCfg.nMSCode = m_nMSCode;
	tempCfg.MAX_PMONEY = (INT64)::_atoi64(m_strMaxMoney.operator LPCTSTR());
	if(tempCfg.MAX_PMONEY < M_1H)  // 10경
	{
		AfxMessageBox("MAX_PMONEY 확인 하십시요.");
		return;
	}
		

	FILE *fp = fopen( "62CutPoker.cfg", "wb");
	if( fp == NULL ) { 
		AfxMessageBox("환경설정 파일을 기록할 수 없습니다.");
		return;
	}
	fwrite(&tempCfg, sizeof(CONFIG), 1, fp);
	fclose(fp);

	if(!g_pMainView->bNowStartUpDlg) AfxMessageBox("변경된 환경설정을 적용하려면 프로그램을 재시작해야합니다.");

	CDialog::OnOK();
}

void CConfigDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CConfigDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
	CDialog::OnLButtonDown(nFlags, point);
}

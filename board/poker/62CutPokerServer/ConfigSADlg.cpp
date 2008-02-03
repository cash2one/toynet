// ConfigSADlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPokerServer.h"
#include "62CutPokerServerView.h"
#include "ConfigSADlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigSADlg dialog


CConfigSADlg::CConfigSADlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigSADlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigSADlg)
	m_bSaveSetLog = FALSE;
	m_LeadersMoney = _T("");
	m_LeadersPlusMoney = _T("");
	m_NormalMoney = _T("");
	m_bSafeAngel = FALSE;//[수호천사]
	m_bTotalMoneyLimit = FALSE;
	m_strSafeMax = _T("");
	m_strSafeMin = _T("");
	//}}AFX_DATA_INIT
}


void CConfigSADlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigSADlg)
	DDX_Check(pDX, IDC_CHECK_SAVE, m_bSaveSetLog);
	DDX_Text(pDX, IDC_EDIT_LEADERS, m_LeadersMoney);
	DDV_MaxChars(pDX, m_LeadersMoney, 19);
	DDX_Text(pDX, IDC_EDIT_LEADERSPLUS, m_LeadersPlusMoney);
	DDV_MaxChars(pDX, m_LeadersPlusMoney, 19);
	DDX_Text(pDX, IDC_EDIT_NORMAL, m_NormalMoney);
	DDV_MaxChars(pDX, m_NormalMoney, 19);
	DDX_Check(pDX, IDC_CHECK_LIMIT, m_bTotalMoneyLimit);
	DDX_Check(pDX, IDC_CHECK_SAFEANGEL, m_bSafeAngel);
	DDX_Text(pDX, IDC_EDIT_SAFEMAX, m_strSafeMax);
	DDX_Text(pDX, IDC_EDIT_SAFEMIN, m_strSafeMin);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigSADlg, CDialog)
	//{{AFX_MSG_MAP(CConfigSADlg)
	ON_BN_CLICKED(IDC_CHECK_LIMIT, OnCheckLimit)
	ON_EN_CHANGE(IDC_EDIT_NORMAL, OnChangeEditNormal)
	ON_EN_CHANGE(IDC_EDIT_LEADERS, OnChangeEditLeaders)
	ON_EN_CHANGE(IDC_EDIT_LEADERSPLUS, OnChangeEditLeadersplus)
	ON_EN_CHANGE(IDC_EDIT_SAFEMIN, OnChangeEditSafemin)
	ON_EN_CHANGE(IDC_EDIT_SAFEMAX, OnChangeEditSafemax)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigSADlg message handlers

BOOL CConfigSADlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_bSaveSetLog = g_pMainView->StatisCfg.bSaveSetLog;

	//2004.05.07 리더스총액 제한
	m_bTotalMoneyLimit = g_pMainView->StatisCfg.bTotalMoneyLimit;
	m_NormalMoney.Format(_T("%I64d"),		g_pMainView->StatisCfg.NormalMoney );
	m_LeadersMoney.Format(_T("%I64d"),		g_pMainView->StatisCfg.LeadersMoney );
	m_LeadersPlusMoney.Format(_T("%I64d"),	g_pMainView->StatisCfg.LeadersPlusMoney );
	
	CString str;
	str = MoneyToString(g_pMainView->StatisCfg.NormalMoney);
	GetDlgItem(IDC_STATIC_NOR)->SetWindowText(str.operator LPCTSTR() );

	str = MoneyToString(g_pMainView->StatisCfg.LeadersMoney);
	GetDlgItem(IDC_STATIC_LEADERS)->SetWindowText(str.operator LPCTSTR() );

	str = MoneyToString(g_pMainView->StatisCfg.LeadersPlusMoney);
	GetDlgItem(IDC_STATIC_LEADERSP)->SetWindowText(str.operator LPCTSTR() );

	// [수호천사] 2004.07.09
	m_bSafeAngel  = g_pMainView->StatisCfg.bSafeAngel;
	m_strSafeMin.Format(_T("%I64d"), g_pMainView->StatisCfg.MinMoneyLimit );
	m_strSafeMax.Format(_T("%I64d"), g_pMainView->StatisCfg.MaxMoneyLimit );
	str = MoneyToString(g_pMainView->StatisCfg.MinMoneyLimit);
	GetDlgItem(IDC_STATIC_SAFEMIN)->SetWindowText(str.operator LPCTSTR() );
	str = MoneyToString(g_pMainView->StatisCfg.MaxMoneyLimit);
	GetDlgItem(IDC_STATIC_SAFEMAX)->SetWindowText(str.operator LPCTSTR() );

	UpdateData(FALSE);
	SetControlState();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigSADlg::OnOK() 
{
	// TODO: Add your control notification handler code here
	
	int rtn = AfxMessageBox("잘 보고 확인하세요~~~~~", MB_YESNO);
	if(rtn==IDNO) {
		
		CDialog::OnOK();
		return;
	}

	UpdateData(TRUE);
	g_pMainView->StatisCfg.bSaveSetLog = m_bSaveSetLog;

	// [통계기록]
//	if( !g_pMainView->StatisCfg.bSaveSetLog && m_bSaveSetLog) {
//		GetLocalTime( &g_pMainView->m_StatisticsLog.PrevTime_1 ); // 시간 초기화
//	}
	g_pMainView->StatisCfg.bSaveSetLog = m_bSaveSetLog; // [통계기록]

	//2004.05.07 리더스총액 제한
	g_pMainView->StatisCfg.bTotalMoneyLimit = m_bTotalMoneyLimit;
	g_pMainView->StatisCfg.NormalMoney		 = ::_atoi64(m_NormalMoney.operator LPCTSTR());
	g_pMainView->StatisCfg.LeadersMoney		 = ::_atoi64(m_LeadersMoney.operator LPCTSTR());
	g_pMainView->StatisCfg.LeadersPlusMoney  = ::_atoi64(m_LeadersPlusMoney.operator LPCTSTR());

	// [수호천사] 2004.07.09
	g_pMainView->StatisCfg.bSafeAngel = m_bSafeAngel;
	g_pMainView->StatisCfg.MinMoneyLimit = ::_atoi64(m_strSafeMin.operator LPCTSTR());
	g_pMainView->StatisCfg.MaxMoneyLimit = ::_atoi64(m_strSafeMax.operator LPCTSTR());


	FILE *fp = fopen( "statistics.cfg", "wb");
	if( fp == NULL ) { 
		AfxMessageBox("환경설정 파일을 기록할 수 없습니다.");
		return;
	}
	fwrite(&g_pMainView->StatisCfg, sizeof(CONFIGSTATIS), 1, fp);
	fclose(fp);

	/*
	if( g_pMainView->Cfg.bEventDB && g_pMainView->EventCfg.bRunEvent ) {	// ### [이벤트 티켓] ###
		g_pMainView->GetDlgItem(IDC_EDIT_BEVENT)->SetWindowText("▶ EventDB Use");
	} else if( g_pMainView->Cfg.bEventDB ) {
		g_pMainView->GetDlgItem(IDC_EDIT_BEVENT)->SetWindowText("▷ EventDB Use");
	} else {
		g_pMainView->GetDlgItem(IDC_EDIT_BEVENT)->SetWindowText("◐ EventDB Use");
	}
*/	
	CDialog::OnOK();
}

void CConfigSADlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}


void CConfigSADlg::OnCheckLimit() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	SetControlState();
}

void CConfigSADlg::SetControlState()
{
	if(m_bTotalMoneyLimit == 1) {
		GetDlgItem(IDC_EDIT_NORMAL)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LEADERS)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LEADERSPLUS)->EnableWindow(TRUE);
	} 
	else
	{
		GetDlgItem(IDC_EDIT_NORMAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LEADERS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LEADERSPLUS)->EnableWindow(FALSE);
	}
}



void CConfigSADlg::OnChangeEditNormal() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString str;
	INT64 tmp = ::_atoi64(m_NormalMoney.operator LPCTSTR());
	str = MoneyToString(tmp);
	GetDlgItem(IDC_STATIC_NOR)->SetWindowText(str.operator LPCTSTR() );
}

void CConfigSADlg::OnChangeEditLeaders() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
	UpdateData(TRUE);
	CString str;
	INT64 tmp = ::_atoi64(m_LeadersMoney.operator LPCTSTR());
	str = MoneyToString(tmp);
	GetDlgItem(IDC_STATIC_LEADERS)->SetWindowText(str.operator LPCTSTR() );
}

void CConfigSADlg::OnChangeEditLeadersplus() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString str;
	INT64 tmp = ::_atoi64(m_LeadersPlusMoney.operator LPCTSTR());
	str = MoneyToString(tmp);
	GetDlgItem(IDC_STATIC_LEADERSP)->SetWindowText(str.operator LPCTSTR() );
	
}

void CConfigSADlg::OnChangeEditSafemin() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString str;
	INT64 tmp = ::_atoi64(m_strSafeMin.operator LPCTSTR());
	str = MoneyToString(tmp);
	GetDlgItem(IDC_STATIC_SAFEMIN)->SetWindowText(str.operator LPCTSTR() );
	
}

void CConfigSADlg::OnChangeEditSafemax() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString str;
	INT64 tmp = ::_atoi64(m_strSafeMax.operator LPCTSTR());
	str = MoneyToString(tmp);
	GetDlgItem(IDC_STATIC_SAFEMAX)->SetWindowText(str.operator LPCTSTR() );
}


CString CConfigSADlg::MoneyToString(INT64 number)
{
	CString strrtn;
	CString strtemp;
	INT64 t = number; 

	BOOL bMinus = FALSE;
	if( t < 0 ) {
		bMinus = TRUE;
		t *= -1;
	}

	INT64 v = t / (INT64)10000000000000000;	// 경 단위
	if(v > 0) {
		t = t - v*(INT64)10000000000000000;
		strtemp.Format("%I64d경", v);
		strrtn += strtemp;
	}

	v = t / (INT64)1000000000000;	// 조 단위
	if(v > 0) {
		t = t - v*(INT64)1000000000000;
		strtemp.Format("%I64d조", v);
		strrtn += strtemp;
	}

	v = t / (INT64)100000000;	// 억 단위
	if(v > 0) {
		t = t - v*(INT64)100000000;
		strtemp.Format("%I64d억", v);
		strrtn += strtemp;
	}
	
	v = t / (INT64)10000;	// 만 단위
	if(v > 0) {
		t = t - v*(INT64)10000;
		strtemp.Format("%I64d만", v);
		strrtn += strtemp;
	}

	if( t > 0 ) {
		strtemp.Format("%I64d", t);
		strrtn += strtemp;
	}

	if( number == 0 )
	{
		strrtn = "0";
	}

	if( bMinus )
	{
		strrtn.Insert(0,"-");
		//CString temp;
		//temp.Format( "-%s", strrtn );
		//strrtn = temp;
	}
	

   //NUMBERFMT nFmt = { 0, 0, 4, ".", ",", 0 };
   //TCHAR    szVal[32];
   //TCHAR     szOut[32];
   //sprintf ( szVal,"%I64d", number );
   //GetNumberFormat ( NULL, NULL, szVal, &nFmt, szOut, 32 );

   	return strrtn;

}



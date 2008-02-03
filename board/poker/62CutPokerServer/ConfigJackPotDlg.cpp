// ConfigJackPotDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPokerServer.h"
#include "ConfigJackPotDlg.h"
#include "62CutPokerServerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigJackPotDlg dialog


CConfigJackPotDlg::CConfigJackPotDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigJackPotDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigJackPotDlg)
	m_JackPot = _T("");
	m_iJack1 = _T("");
	m_iJack2 = _T("");
	m_iJack3 = _T("");
	m_iJack4 = _T("");
	m_iJack5 = _T("");
	m_bJackPotMode = -1;
	m_rChobo = 0;
	m_rFree = 0;
	m_rGosu = 0;
	m_rJungSu = 0;
	m_rGod = 0;
	m_jackPotChobo = 0;
	m_jackPotDosin = 0;
	m_jackPotFree = 0;
	m_jackPotGosu = 0;
	m_jackPotJungSu = 0;
	m_bRealLogUse = FALSE;
	m_nRange = 0;
	m_bSaveSetLog = FALSE;
	m_jackPotChoboS = 0;
	m_jackPotChoboF = 0;
	m_jackPotJungSuF = 0;
	m_jackPotJungSuS = 0;
	m_jackPotGosuF = 0;
	m_jackPotGosuS = 0;
	m_jackPotDosinF = 0;
	m_jackPotDosinS = 0;
	m_jackPotFreeF = 0;
	m_jackPotFreeS = 0;
	//}}AFX_DATA_INIT
}


void CConfigJackPotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigJackPotDlg)
	DDX_Text(pDX, IDC_EDIT_JACKPOT, m_JackPot);
	DDV_MaxChars(pDX, m_JackPot, 19);
	DDX_Text(pDX, IDC_EDIT_JACKPOT_1, m_iJack1);
	DDX_Text(pDX, IDC_EDIT_JACKPOT_2, m_iJack2);
	DDX_Text(pDX, IDC_EDIT_JACKPOT_3, m_iJack3);
	DDX_Text(pDX, IDC_EDIT_JACKPOT_4, m_iJack4);
	DDX_Text(pDX, IDC_EDIT_JACKPOT_5, m_iJack5);
	DDX_Radio(pDX, IDC_RADIO_JACKPOT, m_bJackPotMode);
	DDX_Text(pDX, IDC_EDIT_RATIO_CHOBO, m_rChobo);
	DDV_MinMaxInt(pDX, m_rChobo, 1, 100);
	DDX_Text(pDX, IDC_EDIT_RATIO_FREE, m_rFree);
	DDV_MinMaxInt(pDX, m_rFree, 1, 30);
	DDX_Text(pDX, IDC_EDIT_RATIO_GOSU, m_rGosu);
	DDV_MinMaxInt(pDX, m_rGosu, 1, 30);
	DDX_Text(pDX, IDC_EDIT_RATIO_JUNGSU, m_rJungSu);
	DDV_MinMaxInt(pDX, m_rJungSu, 1, 30);
	DDX_Text(pDX, IDC_EDIT_RATIO_DOSIN, m_rGod);
	DDV_MinMaxInt(pDX, m_rGod, 1, 30);
	DDX_Text(pDX, IDC_EDIT_JACKPOT_CHOBO, m_jackPotChobo);
	DDV_MinMaxInt(pDX, m_jackPotChobo, 0, 5000);
	DDX_Text(pDX, IDC_EDIT_JACKPOT_DOSIN, m_jackPotDosin);
	DDV_MinMaxInt(pDX, m_jackPotDosin, 0, 5000);
	DDX_Text(pDX, IDC_EDIT_JACKPOT_FREE, m_jackPotFree);
	DDV_MinMaxInt(pDX, m_jackPotFree, 0, 5000);
	DDX_Text(pDX, IDC_EDIT_JACKPOT_GOSU, m_jackPotGosu);
	DDV_MinMaxInt(pDX, m_jackPotGosu, 0, 5000);
	DDX_Text(pDX, IDC_EDIT_JACKPOT_JUNGSU, m_jackPotJungSu);
	DDV_MinMaxInt(pDX, m_jackPotJungSu, 0, 5000);
	DDX_Check(pDX, IDC_CHECK_BLOG, m_bRealLogUse);
	DDX_Text(pDX, IDC_EDIT_LOGRANGE, m_nRange);
	DDV_MinMaxInt(pDX, m_nRange, 1, 10000000);
	DDX_Check(pDX, IDC_CHECK_SAVE, m_bSaveSetLog);
	DDX_Text(pDX, IDC_EDIT_JACKPOT_CHOBOS, m_jackPotChoboS);
	DDV_MinMaxInt(pDX, m_jackPotChoboS, 0, 5000);
	DDX_Text(pDX, IDC_EDIT_JACKPOT_CHOBOF, m_jackPotChoboF);
	DDV_MinMaxInt(pDX, m_jackPotChoboF, 0, 5000);
	DDX_Text(pDX, IDC_EDIT_JACKPOT_JUNGSUF, m_jackPotJungSuF);
	DDV_MinMaxInt(pDX, m_jackPotJungSuF, 0, 5000);
	DDX_Text(pDX, IDC_EDIT_JACKPOT_JUNGSUS, m_jackPotJungSuS);
	DDV_MinMaxInt(pDX, m_jackPotJungSuS, 0, 5000);
	DDX_Text(pDX, IDC_EDIT_JACKPOT_GOSUF, m_jackPotGosuF);
	DDV_MinMaxInt(pDX, m_jackPotGosuF, 0, 5000);
	DDX_Text(pDX, IDC_EDIT_JACKPOT_GOSUS, m_jackPotGosuS);
	DDV_MinMaxInt(pDX, m_jackPotGosuS, 0, 5000);
	DDX_Text(pDX, IDC_EDIT_JACKPOT_DOSINF, m_jackPotDosinF);
	DDV_MinMaxInt(pDX, m_jackPotDosinF, 0, 5000);
	DDX_Text(pDX, IDC_EDIT_JACKPOT_DOSINS, m_jackPotDosinS);
	DDV_MinMaxInt(pDX, m_jackPotDosinS, 0, 5000);
	DDX_Text(pDX, IDC_EDIT_JACKPOT_FREEF, m_jackPotFreeF);
	DDV_MinMaxInt(pDX, m_jackPotFreeF, 0, 5000);
	DDX_Text(pDX, IDC_EDIT_JACKPOT_FREES, m_jackPotFreeS);
	DDV_MinMaxInt(pDX, m_jackPotFreeS, 0, 5000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigJackPotDlg, CDialog)
	//{{AFX_MSG_MAP(CConfigJackPotDlg)
	ON_BN_CLICKED(IDC_RADIO_JACKPOT, OnRadioJackpot)
	ON_BN_CLICKED(IDC_RADIO_JACKPOT1, OnRadioJackpot1)
	ON_BN_CLICKED(IDC_CHECK_BLOG, OnCheckBlog)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigJackPotDlg message handlers


// [ ###잭팟### ] 설정 부분(기본셋팅, 유무, 기본값 , 비율등)
BOOL CConfigJackPotDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//CString strM = g_MakeCommaMoney(g_pMainView->Cfg4.nJackPotMoney);
	//m_JackPot.Format(_T("%s"), (char*)strM.operator LPCTSTR());
	m_bJackPotMode = g_pMainView->Cfg4.JackPotMode;

	//m_JackPot.Format(_T("%I64d"), g_pMainView->Cfg4.nJackPotMoney);
	// 기본 잭팟 금액
	m_JackPot.Format(_T("%I64d"), g_JackPotMoney);
	m_iJack1.Format(_T("%I64d"), g_pMainView->Cfg4.ChoBo);
	m_iJack2.Format(_T("%I64d"), g_pMainView->Cfg4.JungSu);
	m_iJack3.Format(_T("%I64d"), g_pMainView->Cfg4.GoSu);
	m_iJack4.Format(_T("%I64d"), g_pMainView->Cfg4.God);
	m_iJack5.Format(_T("%I64d"), g_pMainView->Cfg4.Free);

	// 적립비율
	m_rChobo =	g_pMainView->Cfg4.rChoBo;   
	m_rJungSu =	g_pMainView->Cfg4.rJungSu; 
	m_rGosu =	g_pMainView->Cfg4.rGosu;
	m_rGod =	g_pMainView->Cfg4.rGod;
	m_rFree =	g_pMainView->Cfg4.rFree;

	// 상금 비율 로티플
	m_jackPotChobo  =	g_pMainView->Cfg4.jackPotChobo;
	m_jackPotJungSu =	g_pMainView->Cfg4.jackPotJungSu;
	m_jackPotGosu   =	g_pMainView->Cfg4.jackPotGoSu;   
	m_jackPotDosin  =	g_pMainView->Cfg4.jackPotDosin;   
	m_jackPotFree   =	g_pMainView->Cfg4.jackPotFree;   

	// 상금 비율 스티플
	m_jackPotChoboS  =	g_pMainView->Cfg4.jackPotChoboS;
	m_jackPotJungSuS =	g_pMainView->Cfg4.jackPotJungSuS;
	m_jackPotGosuS   =	g_pMainView->Cfg4.jackPotGoSuS;   
	m_jackPotDosinS  =	g_pMainView->Cfg4.jackPotDosinS;   
	m_jackPotFreeS   =	g_pMainView->Cfg4.jackPotFreeS;   


	// 상금 비율 포카드
	m_jackPotChoboF  =	g_pMainView->Cfg4.jackPotChoboF;
	m_jackPotJungSuF =	g_pMainView->Cfg4.jackPotJungSuF;
	m_jackPotGosuF   =	g_pMainView->Cfg4.jackPotGoSuF;   
	m_jackPotDosinF  =	g_pMainView->Cfg4.jackPotDosinF;   
	m_jackPotFreeF   =	g_pMainView->Cfg4.jackPotFreeF;   



	// ### [로그 기록용] ###
	m_bRealLogUse   = g_pMainView->Cfg4.bRealLogUse;
	// ### [로그 기록용] ###
	if(g_pMainView->Cfg.bLogUseDB && g_pMainView->Cfg4.bRealLogUse)
		g_pMainView->GetDlgItem(IDC_EDIT_BLOG)->SetWindowText("▶ LogDB Use");
	else if(g_pMainView->Cfg.bLogUseDB)
		g_pMainView->GetDlgItem(IDC_EDIT_BLOG)->SetWindowText("▷ LogDB Use");
	else
		g_pMainView->GetDlgItem(IDC_EDIT_BLOG)->SetWindowText("◐ LogDB Use");


	

	m_nRange = g_pMainView->Cfg4.Range;// 로그 기록용 범위 실시간 변경
	g_LogRangeValue = (INT64)g_pMainView->Cfg4.Range * M_1Y;//1억
	
	UpdateData(FALSE);
	
	SetControlState();
	SetControlLog();
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigJackPotDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	//CString str;
	//str.Format("접속인원 제한범위는 1-%d명까지입니다", MAX_USER-1);
	//if(m_MaxUser < 1) { AfxMessageBox(str); return; }
	//if(m_MaxUser > MAX_USER-1) { AfxMessageBox(str); return; }
	
	
	g_pMainView->Cfg4.JackPotMode = m_bJackPotMode;
	g_pMainView->Cfg4.nJackPotMoney = ::_atoi64(m_JackPot.operator LPCTSTR());
	g_JackPotMoney = g_pMainView->Cfg4.nJackPotMoney;


	g_pMainView->Cfg4.ChoBo  = ::_atoi64(m_iJack1.operator LPCTSTR());
	g_pMainView->Cfg4.JungSu = ::_atoi64(m_iJack2.operator LPCTSTR());
	g_pMainView->Cfg4.GoSu   = ::_atoi64(m_iJack3.operator LPCTSTR());
	g_pMainView->Cfg4.God    = ::_atoi64(m_iJack4.operator LPCTSTR());
	g_pMainView->Cfg4.Free   = ::_atoi64(m_iJack5.operator LPCTSTR());
//	m_JackPot.Format("%I64d", g_JackPotMoney);
//	g_pMainView->Cfg4.nJackPotMoney = g_JackPotMoney;
	// 적립비율
	g_pMainView->Cfg4.rChoBo = m_rChobo;
	g_pMainView->Cfg4.rJungSu = m_rJungSu;
	g_pMainView->Cfg4.rGosu = m_rGosu;
	g_pMainView->Cfg4.rGod  = m_rGod;
	g_pMainView->Cfg4.rFree = m_rFree;

	// 상금 비율 로티플
	g_pMainView->Cfg4.jackPotChobo  = m_jackPotChobo; 
	g_pMainView->Cfg4.jackPotJungSu = m_jackPotJungSu;
	g_pMainView->Cfg4.jackPotGoSu   = m_jackPotGosu;  
	g_pMainView->Cfg4.jackPotDosin  = m_jackPotDosin; 
	g_pMainView->Cfg4.jackPotFree   = m_jackPotFree;  

	// 상금 비율 스티플
	g_pMainView->Cfg4.jackPotChoboS  = m_jackPotChoboS; 
	g_pMainView->Cfg4.jackPotJungSuS = m_jackPotJungSuS;
	g_pMainView->Cfg4.jackPotGoSuS   = m_jackPotGosuS;  
	g_pMainView->Cfg4.jackPotDosinS  = m_jackPotDosinS; 
	g_pMainView->Cfg4.jackPotFreeS   = m_jackPotFreeS;  

	// 상금 비율 포카드
	g_pMainView->Cfg4.jackPotChoboF  = m_jackPotChoboF; 
	g_pMainView->Cfg4.jackPotJungSuF = m_jackPotJungSuF;
	g_pMainView->Cfg4.jackPotGoSuF   = m_jackPotGosuF;  
	g_pMainView->Cfg4.jackPotDosinF  = m_jackPotDosinF; 
	g_pMainView->Cfg4.jackPotFreeF   = m_jackPotFreeF;  


	// ### [로그 기록용] ###
	g_pMainView->Cfg4.bRealLogUse = m_bRealLogUse;
	g_pMainView->Cfg4.Range = m_nRange;// 로그 기록용 범위 실시간 변경
	g_LogRangeValue = (INT64)g_pMainView->Cfg4.Range * M_1Y;//1억

	FILE *fp = fopen( "JackPot.cfg", "wb");
	if( fp == NULL ) { 
		AfxMessageBox("환경설정 파일을 기록할 수 없습니다.");
		return;
	}
	fwrite(&g_pMainView->Cfg4, sizeof(CONFIG4), 1, fp);
	fclose(fp);

	if(g_pMainView->Cfg.bLogUseDB && g_pMainView->Cfg4.bRealLogUse)
		g_pMainView->GetDlgItem(IDC_EDIT_BLOG)->SetWindowText("▶ LogDB Use");
	else if(g_pMainView->Cfg.bLogUseDB)
		g_pMainView->GetDlgItem(IDC_EDIT_BLOG)->SetWindowText("▷ LogDB Use");
	else
		g_pMainView->GetDlgItem(IDC_EDIT_BLOG)->SetWindowText("◐ LogDB Use");

	CDialog::OnOK();
}

void CConfigJackPotDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CConfigJackPotDlg::SetControlLog()
{
	//UpdateData(TRUE);
	if(m_bRealLogUse == 1)
		GetDlgItem(IDC_EDIT_LOGRANGE)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_EDIT_LOGRANGE)->EnableWindow(FALSE);


	if(g_pMainView->Cfg.bLogUseDB && g_pMainView->Cfg4.bRealLogUse)
		g_pMainView->GetDlgItem(IDC_EDIT_BLOG)->SetWindowText("▶ LogDB Use");
	else if(g_pMainView->Cfg.bLogUseDB)
		g_pMainView->GetDlgItem(IDC_EDIT_BLOG)->SetWindowText("▷ LogDB Use");
	else
		g_pMainView->GetDlgItem(IDC_EDIT_BLOG)->SetWindowText("◐ LogDB Use");
}

void CConfigJackPotDlg::SetControlState()
{
	UpdateData(TRUE);

	if(m_bJackPotMode == 1) {
		GetDlgItem(IDC_EDIT_JACKPOT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_JACKPOT_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_JACKPOT_2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_JACKPOT_3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_JACKPOT_4)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_JACKPOT_5)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_RATIO_CHOBO)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RATIO_JUNGSU)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RATIO_GOSU)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RATIO_DOSIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RATIO_FREE)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_JACKPOT_CHOBO)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_JACKPOT_JUNGSU)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_JACKPOT_GOSU)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_JACKPOT_DOSIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_JACKPOT_FREE)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_JACKPOT_CHOBOS)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_JACKPOT_JUNGSUS)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_JACKPOT_GOSUS)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_JACKPOT_DOSINS)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_JACKPOT_FREES)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_JACKPOT_CHOBOF)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_JACKPOT_JUNGSUF)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_JACKPOT_GOSUF)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_JACKPOT_DOSINF)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_JACKPOT_FREEF)->EnableWindow(TRUE);

	}
	else {
		GetDlgItem(IDC_EDIT_JACKPOT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_JACKPOT_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_JACKPOT_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_JACKPOT_3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_JACKPOT_4)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_JACKPOT_5)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_RATIO_CHOBO)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RATIO_JUNGSU)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RATIO_GOSU)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RATIO_DOSIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RATIO_FREE)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_JACKPOT_CHOBO)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_JACKPOT_JUNGSU)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_JACKPOT_GOSU)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_JACKPOT_DOSIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_JACKPOT_FREE)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_JACKPOT_CHOBOS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_JACKPOT_JUNGSUS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_JACKPOT_GOSUS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_JACKPOT_DOSINS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_JACKPOT_FREES)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_JACKPOT_CHOBOF)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_JACKPOT_JUNGSUF)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_JACKPOT_GOSUF)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_JACKPOT_DOSINF)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_JACKPOT_FREEF)->EnableWindow(FALSE);
	}
}

void CConfigJackPotDlg::OnRadioJackpot() 
{
	// TODO: Add your control notification handler code here
	SetControlState();
	
}

void CConfigJackPotDlg::OnRadioJackpot1() 
{
	// TODO: Add your control notification handler code here
	SetControlState();
	
}

void CConfigJackPotDlg::OnCheckBlog() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	SetControlLog();
	
}

void CConfigJackPotDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
	CDialog::OnLButtonDown(nFlags, point);
}

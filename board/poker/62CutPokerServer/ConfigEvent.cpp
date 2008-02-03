// ConfigEvent.cpp : implementation file
//

#include "stdafx.h"
#include "62CutPokerServer.h"
#include "62CutPokerServerview.h"
#include "ConfigEvent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigEvent dialog


CConfigEvent::CConfigEvent(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigEvent::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigEvent)
	m_StartDate = 0;
	m_EndDate = 0;
	m_bRunEvent = FALSE;
	m_EventTicketNum = 0;
	m_bNoEventTime = FALSE;
	m_NoEventStartHour = 0;
	m_NoEventHour = 0;
	m_MinUserNum = 0;
	m_EventNotice = _T("");
	m_RoomEventNotice = _T("");
	m_bAutoEventEnd = FALSE;
	//}}AFX_DATA_INIT
}


void CConfigEvent::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigEvent)
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_STARTDATE, m_StartDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ENDDATE, m_EndDate);
	DDX_Check(pDX, IDC_CHECK_RUNEVENT, m_bRunEvent);
	DDX_Text(pDX, IDC_EDIT_EVENTTICKETNUM, m_EventTicketNum);
	DDX_Check(pDX, IDC_CHECK_NOEVENTTIME, m_bNoEventTime);
	DDX_Text(pDX, IDC_EDIT_NOEVENTSTARTHOUR, m_NoEventStartHour);
	DDV_MinMaxInt(pDX, m_NoEventStartHour, 0, 23);
	DDX_Text(pDX, IDC_EDIT_NOEVENTHOUR, m_NoEventHour);
	DDV_MinMaxInt(pDX, m_NoEventHour, 0, 23);
	DDX_Text(pDX, IDC_EDIT_MINUSERNUM, m_MinUserNum);
	DDV_MinMaxInt(pDX, m_MinUserNum, 0, 5000);
	DDX_Text(pDX, IDC_EDIT_EVENTNOTICE, m_EventNotice);
	DDV_MaxChars(pDX, m_EventNotice, 255);
	DDX_Text(pDX, IDC_EDIT_ROOMEVENTNOTICE, m_RoomEventNotice);
	DDV_MaxChars(pDX, m_RoomEventNotice, 255);
	DDX_Check(pDX, IDC_CHECK_AUTOEVENTEND, m_bAutoEventEnd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigEvent, CDialog)
	//{{AFX_MSG_MAP(CConfigEvent)
	ON_BN_CLICKED(IDC_CHECK_RUNEVENT, OnCheckRunevent)
	ON_BN_CLICKED(IDC_CHECK_NOEVENTTIME, OnCheckNoeventtime)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigEvent message handlers

BOOL CConfigEvent::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_bRunEvent = g_pMainView->EventCfg.bRunEvent;
	memcpy(&m_StartDate, &g_pMainView->EventCfg.StartDate, sizeof(CTime));
	memcpy(&m_EndDate, &g_pMainView->EventCfg.EndDate, sizeof(CTime));
	m_EventTicketNum = g_pMainView->EventCfg.EventTicketNum;
	m_bNoEventTime = g_pMainView->EventCfg.bNoEventTime;
	m_NoEventStartHour = g_pMainView->EventCfg.NoEventStartHour;
	m_NoEventHour = g_pMainView->EventCfg.NoEventHour;
	m_MinUserNum = g_pMainView->EventCfg.MinUserNum;
	m_EventNotice = g_pMainView->EventCfg.EventNotice;
	m_RoomEventNotice = g_pMainView->EventCfg.RoomEventNotice;
	m_bAutoEventEnd = g_pMainView->EventCfg.bAutoEventEnd;

	UpdateData(FALSE);

	EnableEvent(m_bRunEvent);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigEvent::EnableEvent(BOOL bEnable)
{
	GetDlgItem(IDC_DATETIMEPICKER_STARTDATE)->EnableWindow(bEnable);
	GetDlgItem(IDC_DATETIMEPICKER_ENDDATE)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_EVENTTICKETNUM)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_NOEVENTTIME)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_MINUSERNUM)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_EVENTNOTICE)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_ROOMEVENTNOTICE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_AUTOEVENTEND)->EnableWindow(bEnable);
	
	BOOL bEnable2 = FALSE;
	if(bEnable && m_bNoEventTime) bEnable2 = TRUE;
	else bEnable2 = FALSE;

	GetDlgItem(IDC_EDIT_NOEVENTSTARTHOUR)->EnableWindow(bEnable2);
	GetDlgItem(IDC_EDIT_NOEVENTHOUR)->EnableWindow(bEnable2);
}

void CConfigEvent::OnOK() 
{
	// TODO: Add extra validation here

	UpdateData(TRUE);

	g_pMainView->EventCfg.bRunEvent = m_bRunEvent;

	SYSTEMTIME systime;
	SYSTEMTIME st,st1;

	GetLocalTime(&st);
	GetSystemTime(&st1);
	
	// 이벤트 시작일: 0시 0분 0초 부터
	m_StartDate.GetAsSystemTime(systime);
	systime.wHour = 0;
	systime.wMinute = 0;
	systime.wSecond = 0;
	systime.wMilliseconds = 0;
	g_pMainView->EventCfg.StartDate = systime;
	// 이벤트 종료일: 당일 24시 바로 이전(23시 59분 59초 999밀리초)까지
	m_EndDate.GetAsSystemTime(systime);
	systime.wHour = 23;
	systime.wMinute = 59;
	systime.wSecond = 59;
	systime.wMilliseconds = 999;
	g_pMainView->EventCfg.EndDate = systime;

	g_pMainView->EventCfg.EventTicketNum = m_EventTicketNum;

	g_pMainView->EventCfg.bNoEventTime = m_bNoEventTime;
	g_pMainView->EventCfg.NoEventStartHour = m_NoEventStartHour;
	g_pMainView->EventCfg.NoEventHour = m_NoEventHour;
	g_pMainView->EventCfg.MinUserNum = m_MinUserNum;
	strncpy(g_pMainView->EventCfg.EventNotice, (char*)(LPCTSTR)m_EventNotice, 255);
	strncpy(g_pMainView->EventCfg.RoomEventNotice, (char*)(LPCTSTR)m_RoomEventNotice, 255);
	g_pMainView->EventCfg.bAutoEventEnd = m_bAutoEventEnd;

	FILE *fp = fopen( "event.cfg", "wb");
	if( fp == NULL ) { 
		AfxMessageBox("환경설정 파일을 기록할 수 없습니다.");
		return;
	}
	fwrite(&g_pMainView->EventCfg, sizeof(CONFIGEVENT), 1, fp);
	fclose(fp);
	
	if( g_pMainView->Cfg.bEventDB && g_pMainView->EventCfg.bRunEvent ) {	// ### [이벤트 티켓] ###
		g_pMainView->GetDlgItem(IDC_EDIT_BEVENT)->SetWindowText("▶ EventDB Use");
	} else if( g_pMainView->Cfg.bEventDB ) {
		g_pMainView->GetDlgItem(IDC_EDIT_BEVENT)->SetWindowText("▷ EventDB Use");
	} else {
		g_pMainView->GetDlgItem(IDC_EDIT_BEVENT)->SetWindowText("◐ EventDB Use");
	}

	CDialog::OnOK();
}

void CConfigEvent::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CConfigEvent::OnCheckRunevent() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	EnableEvent(m_bRunEvent);
}

void CConfigEvent::OnCheckNoeventtime() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	EnableEvent(m_bRunEvent);
}

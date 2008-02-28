// SystemDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "gostop.h"
#include "Global.h"
#include "SystemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSystemDlg dialog


CSystemDlg::CSystemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSystemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSystemDlg)
	//}}AFX_DATA_INIT
}


void CSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSystemDlg)
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_dtcDate);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_dtcTime);
	DDX_Control(pDX, IDC_CB_DAY, m_cbDay);
	DDX_Control(pDX, IDC_CB_MONTH, m_cbMonth);
	DDX_Control(pDX, IDC_CB_YEAR, m_cbYear);
	DDX_Control(pDX, IDC_LC_DATA, m_lcData);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSystemDlg, CDialog)
	//{{AFX_MSG_MAP(CSystemDlg)
	ON_BN_CLICKED(IDC_BTN_SHOWTODAY, OnBtnShowtoday)
	ON_BN_CLICKED(IDC_BTN_SHOWDAY, OnBtnShowday)
	ON_BN_CLICKED(IDC_BTN_SHOWMONTH, OnBtnShowmonth)
	ON_BN_CLICKED(IDC_BTN_SHOWYEAR, OnBtnShowyear)
	ON_BN_CLICKED(IDC_BTN_TODAY, OnBtnToday)
	ON_BN_CLICKED(IDC_BTN_APPLYTIME, OnBtnApplytime)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSystemDlg message handlers

BOOL CSystemDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetDlgItemText(IDC_EB_SETRETURN, "90");
	SetDlgItemText(IDC_EB_CURRETURN, "85");

	SYSTEMTIME st;
	GetLocalTime( &st );

	if(st.wYear >= 2008 && st.wYear < 2018)
		m_cbYear.SetCurSel(st.wYear - 2008);
	else
		m_cbYear.SetCurSel(0);
	m_cbMonth.SetCurSel(st.wMonth - 1);
	m_cbDay.SetCurSel(st.wDay - 1);

	m_lcData.SetExtendedStyle(LVS_EX_GRIDLINES);
	
	m_lcData.InsertColumn(0, "", LVCFMT_RIGHT);
	m_lcData.InsertColumn(1, "메달투입", LVCFMT_RIGHT);
	m_lcData.InsertColumn(2, "이용점수", LVCFMT_RIGHT);
	m_lcData.InsertColumn(3, "당첨점수", LVCFMT_RIGHT);
	m_lcData.SetColumnWidth(0, 40);
	m_lcData.SetColumnWidth(1, 100);
	m_lcData.SetColumnWidth(2, 100);
	m_lcData.SetColumnWidth(3, 100);

	ShowToday();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSystemDlg::ShowToday()
{
	m_lcData.DeleteAllItems();

	for(int i=0; i<24; i++)
	{
		CString sDate;
	
		sDate.Format("%d", i+1);
		m_lcData.InsertItem(i, sDate);
		m_lcData.SetItemText(i, 1, "0");
		m_lcData.SetItemText(i, 2, "0");
		m_lcData.SetItemText(i, 3, "0");
	}
	m_lcData.InsertItem(i, "");
	m_lcData.InsertItem(i+1, "합계");
	m_lcData.SetItemText(i+1, 1, "0");
	m_lcData.SetItemText(i+1, 2, "0");
	m_lcData.SetItemText(i+1, 3, "0");

	char szQuery[1024];
	char szWhere[1024];
	CString sYear, sMonth, sDay;

	m_cbYear.GetWindowText(sYear);
	m_cbMonth.GetWindowText(sMonth);
	m_cbDay.GetWindowText(sDay);
	char szDate[256];
	sprintf(szDate, "%s%02s%02s", sYear, sMonth, sDay);

	// 느린 쿼리임... 문제될듯하면 바꾸면됨...
	strcpy(szQuery, "SELECT SUM(CreditPoint), SUM(UsePoint), SUM(BankPoint),  LogTime FROM tb_statistic");
	sprintf(szWhere, " WHERE LEFT(LogTime,8) = '%s' GROUP BY LogTime", szDate);
	strcat(szQuery, szWhere);

	TRY
	{
		CString sData;
		long lCreditTotal = 0, lUseTotal = 0, lBankTotal = 0;

		// 레코드셋이 열려있으면 닫는다
		if(g_pMainDlg->m_pStatisticDB->IsOpen()) 
			g_pMainDlg->m_pStatisticDB->Close();

		g_pMainDlg->m_pStatisticDB->m_strFilter = "";
		g_pMainDlg->m_pStatisticDB->m_strSort = "";
		g_pMainDlg->m_pStatisticDB->Open(CRecordset::forwardOnly, szQuery);

		while(!g_pMainDlg->m_pStatisticDB->IsEOF())
		{
			CString sDate;
			char szData[256];
				
			g_pMainDlg->m_pStatisticDB->GetFieldValue((short)3, sDate);
			sDate = sDate.Mid(8,2);
			int nDate = atoi(sDate);
			
			g_pMainDlg->m_pStatisticDB->GetFieldValue((short)0, sData);
			sprintf(szData, "%ld", atol(sData));
			lCreditTotal += atol(sData);
			m_lcData.SetItemText(nDate, 1, szData);

			g_pMainDlg->m_pStatisticDB->GetFieldValue((short)1, sData);
			sprintf(szData, "%ld", atol(sData));
			lUseTotal += atol(sData);
			m_lcData.SetItemText(nDate, 2, szData);
			
			g_pMainDlg->m_pStatisticDB->GetFieldValue((short)2, sData);
			sprintf(szData, "%ld", atol(sData));
			lBankTotal += atol(sData);
			m_lcData.SetItemText(nDate, 3, szData);

			// 다음 레코드로 이동
			g_pMainDlg->m_pStatisticDB->MoveNext();
		}

		sData.Format("%ld", lCreditTotal);
		m_lcData.SetItemText(25, 1, sData);
		sData.Format("%ld", lUseTotal);
		m_lcData.SetItemText(25, 2, sData);
		sData.Format("%ld", lBankTotal);
		m_lcData.SetItemText(25, 3, sData);
	}
	CATCH(CDBException, e)
	{
		//CLogFile logfile;
		//logfile.Writef("StatisticDB실패 - AddNewGameDB Open : %s", e->m_strError.operator LPCTSTR());
		//ErrorCnt++;	// 에러 카운트 증가
	}
	END_CATCH
}

void CSystemDlg::ShowDay()
{
	m_lcData.DeleteAllItems();

	for(int i=0; i<31; i++)
	{
		CString sDate;
	
		sDate.Format("%d", i+1);
		m_lcData.InsertItem(i, sDate);
		m_lcData.SetItemText(i, 1, "0");
		m_lcData.SetItemText(i, 2, "0");
		m_lcData.SetItemText(i, 3, "0");
	}
	m_lcData.InsertItem(i, "");
	m_lcData.InsertItem(i+1, "합계");
	m_lcData.SetItemText(i+1, 1, "0");
	m_lcData.SetItemText(i+1, 2, "0");
	m_lcData.SetItemText(i+1, 3, "0");

	char szQuery[1024];
	char szWhere[1024];
	CString sYear, sMonth;

	m_cbYear.GetWindowText(sYear);
	m_cbMonth.GetWindowText(sMonth);
	char szDate[256];
	sprintf(szDate, "%s%02s", sYear, sMonth);

	// 느린 쿼리임... 문제될듯하면 바꾸면됨...
	strcpy(szQuery, "SELECT SUM(CreditPoint), SUM(UsePoint), SUM(BankPoint),  LEFT(LogTime, 8) FROM tb_statistic");
	sprintf(szWhere, " WHERE LEFT(LogTime,6) = '%s' GROUP BY LEFT(LogTime, 8)", szDate);
	strcat(szQuery, szWhere);

	TRY
	{
		CString sData;
		long lCreditTotal = 0, lUseTotal = 0, lBankTotal = 0;

		// 레코드셋이 열려있으면 닫는다
		if(g_pMainDlg->m_pStatisticDB->IsOpen()) 
			g_pMainDlg->m_pStatisticDB->Close();

		g_pMainDlg->m_pStatisticDB->m_strFilter = "";
		g_pMainDlg->m_pStatisticDB->m_strSort = "";
		g_pMainDlg->m_pStatisticDB->Open(CRecordset::forwardOnly, szQuery);

		while(!g_pMainDlg->m_pStatisticDB->IsEOF())
		{
			CString sDate;
			char szData[256];
				
			g_pMainDlg->m_pStatisticDB->GetFieldValue((short)3, sDate);
			sDate = sDate.Mid(6,2);
			int nDate = atoi(sDate);
			
			g_pMainDlg->m_pStatisticDB->GetFieldValue((short)0, sData);
			sprintf(szData, "%ld", atol(sData));
			lCreditTotal += atol(sData);
			m_lcData.SetItemText(nDate-1, 1, szData);

			g_pMainDlg->m_pStatisticDB->GetFieldValue((short)1, sData);
			sprintf(szData, "%ld", atol(sData));
			lUseTotal += atol(sData);
			m_lcData.SetItemText(nDate-1, 2, szData);
			
			g_pMainDlg->m_pStatisticDB->GetFieldValue((short)2, sData);
			sprintf(szData, "%ld", atol(sData));
			lBankTotal += atol(sData);
			m_lcData.SetItemText(nDate-1, 3, szData);

			// 다음 레코드로 이동
			g_pMainDlg->m_pStatisticDB->MoveNext();
		}

		sData.Format("%ld", lCreditTotal);
		m_lcData.SetItemText(32, 1, sData);
		sData.Format("%ld", lUseTotal);
		m_lcData.SetItemText(32, 2, sData);
		sData.Format("%ld", lBankTotal);
		m_lcData.SetItemText(32, 3, sData);

	}
	CATCH(CDBException, e)
	{
		//CLogFile logfile;
		//logfile.Writef("StatisticDB실패 - AddNewGameDB Open : %s", e->m_strError.operator LPCTSTR());
		//ErrorCnt++;	// 에러 카운트 증가
	}
	END_CATCH
}

void CSystemDlg::ShowMonth()
{
	m_lcData.DeleteAllItems();

	for(int i=0; i<12; i++)
	{
		CString sDate;
	
		sDate.Format("%d", i+1);
		m_lcData.InsertItem(i, sDate);
		m_lcData.SetItemText(i, 1, "0");
		m_lcData.SetItemText(i, 2, "0");
		m_lcData.SetItemText(i, 3, "0");
	}
	m_lcData.InsertItem(i, "");
	m_lcData.InsertItem(i+1, "합계");
	m_lcData.SetItemText(i+1, 1, "0");
	m_lcData.SetItemText(i+1, 2, "0");
	m_lcData.SetItemText(i+1, 3, "0");

	char szQuery[1024];
	char szWhere[1024];
	CString sYear;

	m_cbYear.GetWindowText(sYear);
	char szDate[256];
	sprintf(szDate, "%s", sYear);

	// 느린 쿼리임... 문제될듯하면 바꾸면됨...
	//str.Format("LogTime LIKE '%s%%'", szQuery);

	strcpy(szQuery, "SELECT SUM(CreditPoint), SUM(UsePoint), SUM(BankPoint),  LEFT(LogTime, 6) FROM tb_statistic");
	sprintf(szWhere, " WHERE LEFT(LogTime,4) = '%s' GROUP BY LEFT(LogTime, 6)", szDate);
	strcat(szQuery, szWhere);

	TRY
	{
		CString sData;
		long lCreditTotal = 0, lUseTotal = 0, lBankTotal = 0;

		// 레코드셋이 열려있으면 닫는다
		if(g_pMainDlg->m_pStatisticDB->IsOpen()) 
			g_pMainDlg->m_pStatisticDB->Close();

		g_pMainDlg->m_pStatisticDB->m_strFilter = "";
		g_pMainDlg->m_pStatisticDB->m_strSort = "";
		g_pMainDlg->m_pStatisticDB->Open(CRecordset::forwardOnly, szQuery);

		while(!g_pMainDlg->m_pStatisticDB->IsEOF())
		{
			CString sDate;
			char szData[256];
				
			g_pMainDlg->m_pStatisticDB->GetFieldValue((short)3, sDate);
			sDate = sDate.Mid(4,2);
			int nDate = atoi(sDate);
			
			g_pMainDlg->m_pStatisticDB->GetFieldValue((short)0, sData);
			sprintf(szData, "%ld", atol(sData));
			lCreditTotal += atol(sData);
			m_lcData.SetItemText(nDate-1, 1, szData);

			g_pMainDlg->m_pStatisticDB->GetFieldValue((short)1, sData);
			sprintf(szData, "%ld", atol(sData));
			lUseTotal += atol(sData);
			m_lcData.SetItemText(nDate-1, 2, szData);
			
			g_pMainDlg->m_pStatisticDB->GetFieldValue((short)2, sData);
			sprintf(szData, "%ld", atol(sData));
			lBankTotal += atol(sData);
			m_lcData.SetItemText(nDate-1, 3, szData);

			// 다음 레코드로 이동
			g_pMainDlg->m_pStatisticDB->MoveNext();
		}

		sData.Format("%ld", lCreditTotal);
		m_lcData.SetItemText(13, 1, sData);
		sData.Format("%ld", lUseTotal);
		m_lcData.SetItemText(13, 2, sData);
		sData.Format("%ld", lBankTotal);
		m_lcData.SetItemText(13, 3, sData);

	}
	CATCH(CDBException, e)
	{
		//CLogFile logfile;
		//logfile.Writef("StatisticDB실패 - AddNewGameDB Open : %s", e->m_strError.operator LPCTSTR());
		//ErrorCnt++;	// 에러 카운트 증가
	}
	END_CATCH
}

void CSystemDlg::ShowYear()
{
	m_lcData.DeleteAllItems();
	
	for(int i=0; i<10; i++)
	{
		CString sDate;
	
		sDate.Format("%d", i+2008);
		m_lcData.InsertItem(i, sDate);
		m_lcData.SetItemText(i, 1, "0");
		m_lcData.SetItemText(i, 2, "0");
		m_lcData.SetItemText(i, 3, "0");
	}
	m_lcData.InsertItem(i, "");
	m_lcData.InsertItem(i+1, "합계");
	m_lcData.SetItemText(i+1, 1, "0");
	m_lcData.SetItemText(i+1, 2, "0");
	m_lcData.SetItemText(i+1, 3, "0");

	char szQuery[1024];
	char szWhere[1024];
	SYSTEMTIME st;

	// 느린 쿼리임... 문제될듯하면 바꾸면됨...
	strcpy(szQuery, "SELECT SUM(CreditPoint), SUM(UsePoint), SUM(BankPoint),  LEFT(LogTime, 4) FROM tb_statistic");
	sprintf(szWhere, " GROUP BY LEFT(LogTime, 4)" );
	strcat(szQuery, szWhere);

	TRY
	{
		CString sData;
		long lCreditTotal = 0, lUseTotal = 0, lBankTotal = 0;

		// 레코드셋이 열려있으면 닫는다
		if(g_pMainDlg->m_pStatisticDB->IsOpen()) 
			g_pMainDlg->m_pStatisticDB->Close();

		g_pMainDlg->m_pStatisticDB->m_strFilter = "";
		g_pMainDlg->m_pStatisticDB->m_strSort = "";
		g_pMainDlg->m_pStatisticDB->Open(CRecordset::forwardOnly, szQuery);

		int iCnt = 0;

		while(!g_pMainDlg->m_pStatisticDB->IsEOF())
		{
			CString sDate;
			char szData[256];
				
			g_pMainDlg->m_pStatisticDB->GetFieldValue((short)3, sDate);
			int nDate = atoi(sDate);

			if(nDate >= 2008 && nDate < 2018)
			{
				g_pMainDlg->m_pStatisticDB->GetFieldValue((short)0, sData);
				sprintf(szData, "%ld", atol(sData));
				lCreditTotal += atol(sData);
				m_lcData.SetItemText(nDate-2008, 1, szData);

				g_pMainDlg->m_pStatisticDB->GetFieldValue((short)1, sData);
				sprintf(szData, "%ld", atol(sData));
				lUseTotal += atol(sData);
				m_lcData.SetItemText(nDate-2008, 2, szData);
				
				g_pMainDlg->m_pStatisticDB->GetFieldValue((short)2, sData);
				sprintf(szData, "%ld", atol(sData));
				lBankTotal += atol(sData);
				m_lcData.SetItemText(nDate-2008, 3, szData);
			}

			// 다음 레코드로 이동
			g_pMainDlg->m_pStatisticDB->MoveNext();
		}

		sData.Format("%ld", lCreditTotal);
		m_lcData.SetItemText(11, 1, sData);
		sData.Format("%ld", lUseTotal);
		m_lcData.SetItemText(11, 2, sData);
		sData.Format("%ld", lBankTotal);
		m_lcData.SetItemText(11, 3, sData);

	}
	CATCH(CDBException, e)
	{
		//CLogFile logfile;
		//logfile.Writef("StatisticDB실패 - AddNewGameDB Open : %s", e->m_strError.operator LPCTSTR());
		//ErrorCnt++;	// 에러 카운트 증가
	}
	END_CATCH
}

void CSystemDlg::OnBtnShowtoday() 
{
	ShowToday();	
}

void CSystemDlg::OnBtnShowday() 
{
	ShowDay();	
}

void CSystemDlg::OnBtnShowmonth() 
{
	ShowMonth();	
}

void CSystemDlg::OnBtnShowyear() 
{
	ShowYear();	
}

void CSystemDlg::OnBtnToday() 
{
	SYSTEMTIME st;
	GetLocalTime( &st );

	if(st.wYear >= 2008 && st.wYear < 2018)
		m_cbYear.SetCurSel(st.wYear - 2008);
	else
		m_cbYear.SetCurSel(0);
	m_cbMonth.SetCurSel(st.wMonth - 1);
	m_cbDay.SetCurSel(st.wDay - 1);
}

void CSystemDlg::OnBtnApplytime() 
{
	SYSTEMTIME stDate;
	SYSTEMTIME stTime;
	SYSTEMTIME st;

	m_dtcDate.GetTime(&stDate);
	m_dtcTime.GetTime(&stTime);

	st.wYear = stDate.wYear;
	st.wMonth = stDate.wMonth;
	st.wDay = stDate.wDay;
	st.wHour = stTime.wHour;
	st.wMinute = stTime.wMinute;
	st.wSecond = stTime.wSecond;
	st.wMilliseconds = 0;

	SetLocalTime(&st);
}

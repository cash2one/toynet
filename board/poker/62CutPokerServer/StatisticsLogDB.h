#if !defined(AFX_STATISTICSLOGDB_H__AEDCD305_E8CF_47FD_9A77_AE0295150752__INCLUDED_)
#define AFX_STATISTICSLOGDB_H__AEDCD305_E8CF_47FD_9A77_AE0295150752__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatisticsLogDB.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStatisticsLogDB recordset
// [ 통계기록 ]
class CStatisticsLogDB : public CRecordset
{
public:
	static CString m_DSN;		// DSN명 정적 멤버
	static CString m_Table;		// 테이블명 정적 멤버
	static CString m_UID;		// 로그인 아이디 정적 멤버
	static CString m_Pass;		// 패스워드 정적 멤버

	CStatisticsLogDB(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CStatisticsLogDB)

// Field/Param Data
	//{{AFX_FIELD(CStatisticsLogDB, CRecordset)
	long	m_c_GameCode;
	long	m_c_ServerCode;
	long	m_c_Kind;
	CString	m_c_BigData_0;
	CString	m_c_BigData_1;
	CString	m_c_BigData_2;
	CString	m_c_BigData_3;
	CString	m_c_BigData_4;
	long	m_c_SmallData_0;
	long	m_c_SmallData_1;
	long	m_c_SmallData_2;
	long	m_c_SmallData_3;
	long	m_c_SmallData_4;
	long	m_c_SmallData_5;
	long	m_c_SmallData_6;
	long	m_c_SmallData_7;
	long	m_c_SmallData_8;
	long	m_c_SmallData_9;
	long	m_c_SmallData_10;
	long	m_c_SmallData_11;
	long	m_c_SmallData_12;
	long	m_c_SmallData_13;
	long	m_c_SmallData_14;
	long	m_c_SmallData_15;
	long	m_c_SmallData_16;
	long	m_c_SmallData_17;
	long	m_c_SmallData_18;
	long	m_c_SmallData_19;
	COleDateTime	m_Date;
	long    m_c_extra;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatisticsLogDB)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATISTICSLOGDB_H__AEDCD305_E8CF_47FD_9A77_AE0295150752__INCLUDED_)

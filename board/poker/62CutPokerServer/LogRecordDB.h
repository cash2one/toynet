#if !defined(AFX_LOGRECORDDB_H__7B8C6470_C59A_42E6_A740_00272F22D581__INCLUDED_)
#define AFX_LOGRECORDDB_H__7B8C6470_C59A_42E6_A740_00272F22D581__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogRecordDB.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLogRecordDB recordset

class CLogRecordDB : public CRecordset
{
public:
	static CString m_DSN;		// DSN명 정적 멤버
	static CString m_Table;		// 테이블명 정적 멤버
	static CString m_UID;		// 로그인 아이디 정적 멤버
	static CString m_Pass;		// 패스워드 정적 멤버


	CLogRecordDB(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CLogRecordDB)

// Field/Param Data
	//{{AFX_FIELD(CLogRecordDB, CRecordset)
	CString	m_MyLoseMoney;
	CString	m_My_IP;
	CString	m_Winner_IP;
	CString	m_WinGetMoney;
	CString	m_WinCurMoney;
	CString	m_WinnerID;
	CString	m_MyCurMoney;
	CString	m_MyID;
	long	m_No;
	int		m_Status;
	int		m_GameCode;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogRecordDB)
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

#endif // !defined(AFX_LOGRECORDDB_H__7B8C6470_C59A_42E6_A740_00272F22D581__INCLUDED_)

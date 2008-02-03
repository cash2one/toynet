#if !defined(AFX_BADUSERDB_H__A6BABDC1_BFBC_11D4_97A4_0050BF0FBE67__INCLUDED_)
#define AFX_BADUSERDB_H__A6BABDC1_BFBC_11D4_97A4_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BadUserDB.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBadUserDB recordset

class CBadUserDB : public CRecordset
{
public:
	static CString m_DSN;		// DSN명 정적 멤버
	static CString m_Table;		// 테이블명 정적 멤버
	static CString m_UID;		// 로그인 아이디 정적 멤버
	static CString m_Pass;		// 패스워드 정적 멤버

	CBadUserDB(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CBadUserDB)

// Field/Param Data
	//{{AFX_FIELD(CBadUserDB, CRecordset)
	long	m_NO;
	CString	m_MyID;
	CString	m_TargetID;
	long	m_GameCode;
	long	m_ServerCode;
	CString	m_SubmitDate;
	CString	m_SubmitReason;
	CString	m_DealDate;
	long	m_DealResult;
	long	m_DealReason;
	CString	m_WatchID;
	CString	m_Contents;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBadUserDB)
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

#endif // !defined(AFX_BADUSERDB_H__A6BABDC1_BFBC_11D4_97A4_0050BF0FBE67__INCLUDED_)

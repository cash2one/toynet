#if !defined(AFX_STATISTICDB_H__A4F45ED2_6C3F_4AA7_9C10_5007494E4EAE__INCLUDED_)
#define AFX_STATISTICDB_H__A4F45ED2_6C3F_4AA7_9C10_5007494E4EAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatisticDB.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStatisticDB recordset

class CStatisticDB : public CRecordset
{
public:
	static CString m_DSN;		// DSN명 정적 멤버
	static CString m_Table;		// 테이블명 정적 멤버
	static CString m_UID;		// 로그인 아이디 정적 멤버
	static CString m_Pass;		// 패스워드 정적 멤버

	CStatisticDB(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CStatisticDB)

// Field/Param Data
	//{{AFX_FIELD(CStatisticDB, CRecordset)
	long	m_KeyIndex;
	long	m_GameCode;
	long	m_ServerCode;
	long	m_CreditPoint;
	long	m_UsePoint;
	long	m_BankPoint;
	CString	m_LogTime;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatisticDB)
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

#endif // !defined(AFX_STATISTICDB_H__A4F45ED2_6C3F_4AA7_9C10_5007494E4EAE__INCLUDED_)

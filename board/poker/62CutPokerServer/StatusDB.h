#if !defined(AFX_STATUSDB_H__A6BABDC1_BFBC_11D4_97A4_0050BF0FBE67__INCLUDED_)
#define AFX_STATUSDB_H__A6BABDC1_BFBC_11D4_97A4_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatusDB.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStatusDB recordset

class CStatusDB : public CRecordset
{
public:
	static CString m_DSN;		// DSN명 정적 멤버
	static CString m_Table;		// 테이블명 정적 멤버
	static CString m_UID;		// 로그인 아이디 정적 멤버
	static CString m_Pass;		// 패스워드 정적 멤버

	CStatusDB(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CStatusDB)

// Field/Param Data
	//{{AFX_FIELD(CStatusDB, CRecordset)
	long	m_GameCode;
	long	m_ServerCode;
	long	m_UserNum;
	CString m_ServerIP;
	long	m_ServerPort;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatusDB)
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

#endif // !defined(AFX_STATUSDB_H__A6BABDC1_BFBC_11D4_97A4_0050BF0FBE67__INCLUDED_)

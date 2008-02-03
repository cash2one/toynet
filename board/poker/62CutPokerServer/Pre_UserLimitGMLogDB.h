#if !defined(AFX_PRE_USERLIMITGMLOGDB_H__26F4A683_4F39_4FB0_914A_918DF35058B2__INCLUDED_)
#define AFX_PRE_USERLIMITGMLOGDB_H__26F4A683_4F39_4FB0_914A_918DF35058B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Pre_UserLimitGMLogDB.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPre_UserLimitGMLogDB recordset

class CPre_UserLimitGMLogDB : public CRecordset
{
public:
	static CString m_DSN;		// DSN명 정적 멤버
	static CString m_Table;		// 테이블명 정적 멤버
	static CString m_UID;		// 로그인 아이디 정적 멤버
	static CString m_Pass;		// 패스워드 정적 멤버

	CPre_UserLimitGMLogDB(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CPre_UserLimitGMLogDB)

// Field/Param Data
	//{{AFX_FIELD(CPre_UserLimitGMLogDB, CRecordset)
	CString	m_NO;
	CString	m_ID;
	long	m_MoneyType;
	long	m_Amount;
	long	m_BeforeM;
	long	m_AfterM;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPre_UserLimitGMLogDB)
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

#endif // !defined(AFX_PRE_USERLIMITGMLOGDB_H__26F4A683_4F39_4FB0_914A_918DF35058B2__INCLUDED_)

#if !defined(AFX_EVENTTICKET_H__8499472D_9F15_4F90_8908_DA840DF2F463__INCLUDED_)
#define AFX_EVENTTICKET_H__8499472D_9F15_4F90_8908_DA840DF2F463__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EventTicket.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEventTicket recordset

class CEventTicket : public CRecordset
{
public:
	
	static CString m_DSN;		// DSN명 정적 멤버
	static CString m_Table;		// 테이블명 정적 멤버
	static CString m_UID;		// 로그인 아이디 정적 멤버
	static CString m_Pass;		// 패스워드 정적 멤버

	CEventTicket(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CEventTicket)

// Field/Param Data
	//{{AFX_FIELD(CEventTicket, CRecordset)
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEventTicket)
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

#endif // !defined(AFX_EVENTTICKET_H__8499472D_9F15_4F90_8908_DA840DF2F463__INCLUDED_)

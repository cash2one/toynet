#if !defined(AFX_ALLINCHARGE_H__284F7E7F_8329_4FD8_AE05_E5E73E75C4D8__INCLUDED_)
#define AFX_ALLINCHARGE_H__284F7E7F_8329_4FD8_AE05_E5E73E75C4D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AllinCharge.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAllinCharge recordset

class CAllinCharge : public CRecordset
{
public:
	static CString m_DSN;		// DSN명 정적 멤버
	static CString m_Table;		// 테이블명 정적 멤버
	static CString m_UID;		// 로그인 아이디 정적 멤버
	static CString m_Pass;		// 패스워드 정적 멤버

	CAllinCharge(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CAllinCharge)

// Field/Param Data
	//{{AFX_FIELD(CAllinCharge, CRecordset)
	long			m_INDEX;
	CString			m_ID;
	CString			m_OLDMONEY;
	CString			m_AMOUNTM;
	CString			m_NEWMONEY;
	COleDateTime	m_DATE;
	long			m_GAMECODE;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAllinCharge)
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

#endif // !defined(AFX_ALLINCHARGE_H__284F7E7F_8329_4FD8_AE05_E5E73E75C4D8__INCLUDED_)

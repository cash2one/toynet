#if !defined(AFX_ITEMDB_H__DD640C61_0F0D_11D4_97A2_0050BF0FBE67__INCLUDED_)
#define AFX_ITEMDB_H__DD640C61_0F0D_11D4_97A2_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ItemDB.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CItemDB recordset

class CItemDB : public CRecordset
{
public:
	static CString m_DSN;		// DSN�� ���� ���
	static CString m_Table;		// ���̺�� ���� ���
	static CString m_UID;		// �α��� ���̵� ���� ���
	static CString m_Pass;		// �н����� ���� ���

	CItemDB(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CItemDB)

	// Field/Param Data
	//{{AFX_FIELD(CItemDB, CRecordset)
	long			m_NO;
	CString			m_ID;
	CString			m_PresID;
	long			m_Code;
	long			m_GameCode;
	long			m_UsedGameCode;
	long			m_UseDay;
	long			m_UseNum;
	COleDateTime	m_StartDate;
	long			m_Use;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CItemDB)
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

#endif // !defined(AFX_ITEMDB_H__DD640C61_0F0D_11D4_97A2_0050BF0FBE67__INCLUDED_)

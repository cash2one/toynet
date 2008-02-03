#if !defined(AFX_PREMIUMDB_H__DD640C61_0F0D_11D4_97A2_0050BF0FBE67__INCLUDED_)
#define AFX_PREMIUMDB_H__DD640C61_0F0D_11D4_97A2_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PremiumDB.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPremiumDB recordset

class CPremiumDB : public CRecordset
{
public:
	static CString m_DSN;		// DSN�� ���� ���
	static CString m_Table;		// ���̺�� ���� ���
	static CString m_UID;		// �α��� ���̵� ���� ���
	static CString m_Pass;		// �н����� ���� ���

	CPremiumDB(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CPremiumDB)

// Field/Param Data
	//{{AFX_FIELD(CPremiumDB, CRecordset)
	CString			m_UniqNO;
	long			m_PremCode;
	COleDateTime	m_PremDate;
	long			m_LeadersCode; //2004.05.07 �������Ѿ� ����
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPremiumDB)
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

#endif // !defined(AFX_PREMIUMDB_H__DD640C61_0F0D_11D4_97A2_0050BF0FBE67__INCLUDED_)

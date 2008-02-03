#if !defined(AFX_UERDB_H__DD640C61_0F0D_11D4_97A2_0050BF0FBE67__INCLUDED_)
#define AFX_UERDB_H__DD640C61_0F0D_11D4_97A2_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserDB.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUserDB recordset

class CUserDB : public CRecordset
{
public:
	static CString m_DSN;		// DSN�� ���� ���
	static CString m_Table;		// ���̺�� ���� ���
	static CString m_UID;		// �α��� ���̵� ���� ���
	static CString m_Pass;		// �н����� ���� ���

	CUserDB(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CUserDB)

// Field/Param Data
	//{{AFX_FIELD(CUserDB, CRecordset)
	CString			m_UniqNO;		// [�����̾� �۾�]
	CString			m_ID;
	CString			m_Passwd;
	BOOL			m_Sex;
	CString			m_Desc;
	BYTE			m_IDState;
	CString			m_RegID;		// [���� ������ �۾�]
	BOOL			m_Certificate;	// [��������]
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserDB)
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

#endif // !defined(AFX_UERDB_H__DD640C61_0F0D_11D4_97A2_0050BF0FBE67__INCLUDED_)

#if !defined(AFX_EVENTPRIZEDB_H__E8892469_C56D_4686_A153_73553D17B423__INCLUDED_)
#define AFX_EVENTPRIZEDB_H__E8892469_C56D_4686_A153_73553D17B423__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EventPrizeDB.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEventPrizeDB recordset

// ### [�̺�Ʈ Ƽ��] ###
class CEventPrizeDB : public CRecordset
{
public:
	static CString m_DSN;		// DSN�� ���� ���
	static CString m_Table;		// ���̺�� ���� ���
	static CString m_UID;		// �α��� ���̵� ���� ���
	static CString m_Pass;		// �н����� ���� ���


	CEventPrizeDB(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CEventPrizeDB)

// Field/Param Data
	//{{AFX_FIELD(CEventPrizeDB, CRecordset)
	CString	m_ID;
	COleDateTime m_LastPlayDate;
	long	m_GameCount;
	long	m_EventCoin;
	long    m_GameCode;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEventPrizeDB)
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

#endif // !defined(AFX_EVENTPRIZEDB_H__E8892469_C56D_4686_A153_73553D17B423__INCLUDED_)

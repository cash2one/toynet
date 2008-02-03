#if !defined(AFX_GAMEDB_H__DD640C63_0F0D_11D4_97A2_0050BF0FBE67__INCLUDED_)
#define AFX_GAMEDB_H__DD640C63_0F0D_11D4_97A2_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GameDB.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGameDB recordset

class CGameDB : public CRecordset
{
public:
	static CString m_DSN;		// DSN�� ���� ���
	static CString m_Table;		// ���̺�� ���� ���
	static CString m_UID;		// �α��� ���̵� ���� ���
	static CString m_Pass;		// �н����� ���� ���

	CGameDB(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CGameDB)

// Field/Param Data
	//{{AFX_FIELD(CGameDB, CRecordset)
CString	m_ID;
	long	m_Vic;
	long	m_RSF; //�ξ� ��Ʈ����Ʈ �÷���(���Դ�) ī��Ʈ 
	long	m_SF;  //��Ʈ����Ʈ �÷��� ī��Ʈ
	long	m_FCARD;//��ī�� ī��Ʈ
	BOOL    m_CARD04;
	BOOL    m_CARD05;
	BOOL    m_CARD06;
	BOOL    m_CARD07;
	BOOL    m_CARD08;
	BOOL    m_CARD09;
	BOOL    m_CARD10;
	BOOL    m_CARD11;
	BOOL    m_CARD12;
	BOOL    m_CARD13;
	BOOL    m_CARD14;
	BOOL    m_CARD15;
	CString m_PMoney;
	CString m_LMoney;	// [��ȣõ��]
	CString	m_History1;
	CString	m_History2;
	CString	m_History3;
	BYTE	m_HistoryIndex;
	long	m_Fail;
	long    m_Fold;
	BYTE	m_IsConnected;
	BYTE	m_Level;
	BYTE	m_Level2;
	BYTE	m_WinPer;//�·� ����
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGameDB)
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

#endif // !defined(AFX_GAMEDB_H__DD640C63_0F0D_11D4_97A2_0050BF0FBE67__INCLUDED_)

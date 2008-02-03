#if !defined(AFX_AVATARDB_H__DD640C61_0F0D_11D4_97A2_0050BF0FBE67__INCLUDED_)
#define AFX_AVATARDB_H__DD640C61_0F0D_11D4_97A2_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AvatarDB.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAvatarDB recordset

class CAvatarDB : public CRecordset
{
public:
	static CString m_DSN;		// DSN명 정적 멤버
	static CString m_Table;		// 테이블명 정적 멤버
	static CString m_UID;		// 로그인 아이디 정적 멤버
	static CString m_Pass;		// 패스워드 정적 멤버

	CAvatarDB(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CAvatarDB)

// Field/Param Data
	//{{AFX_FIELD(CAvatarDB, CRecordset)
	CString	m_ID;
	long	m_layer0;
	long	m_layer1;
	long	m_layer2;
	long	m_layer3;
	long	m_layer4;
	long	m_layer5;
	long	m_layer6;
	long	m_layer7;
	long	m_layer8;
	long	m_layer9;
	long	m_layer10;
	long	m_layer11;
	long	m_layer12;
	long	m_layer13;
	long	m_layer14;
	long	m_layer15;
	long	m_layer16;
	long	m_layer17;
	long	m_layer18;
	long	m_layer19;
	long	m_layer20;
	long	m_layer21;
	long	m_layer22;
	long	m_layer23;
	long	m_layer24;
	long	m_layer25;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAvatarDB)
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

#endif // !defined(AFX_AVATARDB_H__DD640C61_0F0D_11D4_97A2_0050BF0FBE67__INCLUDED_)

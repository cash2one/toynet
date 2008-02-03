// 62CutPokerServerDoc.h : interface of the C62CutPokerServerDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TETRISSERVERDOC_H__B406646C_D2B5_4478_9CF4_140E3658FAFE__INCLUDED_)
#define AFX_TETRISSERVERDOC_H__B406646C_D2B5_4478_9CF4_140E3658FAFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class C62CutPokerServerDoc : public CDocument
{
protected: // create from serialization only
	C62CutPokerServerDoc();
	DECLARE_DYNCREATE(C62CutPokerServerDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C62CutPokerServerDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~C62CutPokerServerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(C62CutPokerServerDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TETRISSERVERDOC_H__B406646C_D2B5_4478_9CF4_140E3658FAFE__INCLUDED_)

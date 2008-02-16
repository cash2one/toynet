// 62CutPoker.h : main header file for the 62CutPoker application
//

#if !defined(AFX_62CutPoker_H__B98215C1_EBAB_435D_8AA6_61E0DE402D20__INCLUDED_)
#define AFX_62CutPoker_H__B98215C1_EBAB_435D_8AA6_61E0DE402D20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// C62CutPokerApp:
// See 62CutPoker.cpp for the implementation of this class
//

class C62CutPokerApp : public CWinApp
{
public:
	C62CutPokerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C62CutPokerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(C62CutPokerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_62CutPoker_H__B98215C1_EBAB_435D_8AA6_61E0DE402D20__INCLUDED_)

// 62CutPokerServer.h : main header file for the 62CutPokerServer application
//

#if !defined(AFX_TETRISSERVER_H__D0D0DE38_D395_45AA_9A86_BC8970D0EB74__INCLUDED_)
#define AFX_TETRISSERVER_H__D0D0DE38_D395_45AA_9A86_BC8970D0EB74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


/////////////////////////////////////////////////////////////////////////////
// C62CutPokerServerApp:
// See 62CutPokerServer.cpp for the implementation of this class
//

class C62CutPokerServerApp : public CWinApp
{
public:
	C62CutPokerServerApp();

//	void SaveCurrentDir();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C62CutPokerServerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(C62CutPokerServerApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TETRISSERVER_H__D0D0DE38_D395_45AA_9A86_BC8970D0EB74__INCLUDED_)

#if !defined(AFX_DATASAVETHREAD_H__99E53A2B_1C32_479D_BC40_F8712B41C4EB__INCLUDED_)
#define AFX_DATASAVETHREAD_H__99E53A2B_1C32_479D_BC40_F8712B41C4EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataSaveThread.h : header file
//

#define TM_THREADMSG 100

/////////////////////////////////////////////////////////////////////////////
// CDataSaveThread thread

class CDataSaveThread : public CWinThread
{
	DECLARE_DYNCREATE(CDataSaveThread)
protected:
	CDataSaveThread();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
	void SaveData();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataSaveThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
//	virtual int Run();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDataSaveThread();

	// Generated message map functions
	//{{AFX_MSG(CDataSaveThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATASAVETHREAD_H__99E53A2B_1C32_479D_BC40_F8712B41C4EB__INCLUDED_)

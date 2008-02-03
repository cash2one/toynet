#if !defined(AFX_CONFIGEVENT_H__71F35954_27ED_4895_98BE_0C330E039314__INCLUDED_)
#define AFX_CONFIGEVENT_H__71F35954_27ED_4895_98BE_0C330E039314__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigEvent.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigEvent dialog

// ### [이벤트 티켓] ###
class CConfigEvent : public CDialog
{
// Construction
public:
	CConfigEvent(CWnd* pParent = NULL);   // standard constructor

	void EnableEvent(BOOL bEnable);

// Dialog Data
	//{{AFX_DATA(CConfigEvent)
	enum { IDD = IDD_CONFIG_EVENT };
	CTime	m_StartDate;
	CTime	m_EndDate;
	BOOL	m_bRunEvent;
	int		m_EventTicketNum;
	BOOL	m_bNoEventTime;
	int		m_NoEventStartHour;
	int		m_NoEventHour;
	int		m_MinUserNum;
	CString	m_EventNotice;
	CString	m_RoomEventNotice;
	BOOL	m_bAutoEventEnd;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigEvent)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigEvent)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCheckRunevent();
	afx_msg void OnCheckNoeventtime();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGEVENT_H__71F35954_27ED_4895_98BE_0C330E039314__INCLUDED_)

#if !defined(AFX_WAITSOCK_H__65409B35_DE27_11D3_97A2_0050BF0FBE67__INCLUDED_)
#define AFX_WAITSOCK_H__65409B35_DE27_11D3_97A2_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServerSock.h : header file
//

#include "DataSock.h"

#define UM_SERV_ACCEPT (WM_USER+700)
#define UM_SERV_CLOSE (WM_USER+701)


/////////////////////////////////////////////////////////////////////////////
// CServerSock command target

class CServerSock : public CAsyncSocket
{
// Attributes
public:
	HWND hWND;									// 메세지를 Send할 윈도우 핸들
	CString m_strError;
	
// Operations
public:
	CServerSock();
	~CServerSock();

	BOOL CreateSocket(HWND hwnd, UINT lPortNum, char *lpszSocketAddress=NULL);
	BOOL AcceptSocket(HWND hwnd, CDataSock *pSock, int sid); // 연결 요청을 받아들이고 데이터 소켓을 초기화

	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);


// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerSock)
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CServerSock)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAITSOCK_H__65409B35_DE27_11D3_97A2_0050BF0FBE67__INCLUDED_)

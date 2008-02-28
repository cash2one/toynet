// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__ACA08DB3_5371_11D4_97A5_0050BF0FBE67__INCLUDED_)
#define AFX_STDAFX_H__ACA08DB3_5371_11D4_97A5_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>		// MFC socket extensions

// ### [ 대기실 리스트 관련 ] ###
#pragma warning( disable: 4786)
#include <deque>
#include <map>
#include <algorithm>
#include <functional>

#include "RxCom232Dll.h"
#include <afxdb.h>			// MFC ODBC database classes
#include <afxdao.h>			// MFC DAO database classes

WPARAM GetValFromCom(WPARAM wParam);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__ACA08DB3_5371_11D4_97A5_0050BF0FBE67__INCLUDED_)

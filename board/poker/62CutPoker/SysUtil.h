// SysUtil.h
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSUTIL_H__363BCC4C_6969_4BB5_B3E4_F4A65D94718B__INCLUDED_)
#define AFX_SYSUTIL_H__363BCC4C_6969_4BB5_B3E4_F4A65D94718B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


int   GetProfile_int(LPCTSTR strSection, LPCTSTR strKeyname, int nDefault);
float GetProfile_float(LPCTSTR strSection, LPCTSTR strKeyname, float fDefault);
char* GetProfile_string(LPCTSTR strSection, LPCTSTR strKeyname, LPCTSTR strDefault);

BOOL WriteProfile_int(LPCTSTR strSection, LPCTSTR strKeyname, int nValue);
BOOL WriteProfile_float(LPCTSTR strSection, LPCTSTR strKeyname, float fValue);
BOOL WriteProfile_string(LPCTSTR strSection, LPCTSTR strKeyname, LPCTSTR strValue);

//===================================================================

BOOL OpenNehompy(LPCTSTR szMyUniqNo, LPCTSTR szTargetID);
BOOL OpenNetmarbleWebpage(LPCTSTR szMyUniqNo, LPCTSTR szTargetURL);

//===================================================================

BOOL ScreenCapture(HWND hwnd, LPCTSTR fname);

//===================================================================

BOOL WindowResize2Bitmap(HWND hwnd, HGDIOBJ hbmp);
BOOL WindowResize2Bitmap(HWND hwnd, LPCTSTR bmpfilename);

//===================================================================

#include <winsvc.h>

class CMsgServiceControl  
{
public:
	CMsgServiceControl();
	virtual ~CMsgServiceControl();

	BOOL InitService(LPCTSTR lpServiceName);
	BOOL StopService(BOOL bStop);

public:
	BOOL			m_Inited;

	BOOL			m_bIsStart;

	SC_HANDLE		m_SCMHandle;		// COM/DCOM¿ë ÇÚµé
	CString			m_Servicename;
	SERVICE_STATUS	m_serviceStatus;
};

//===================================================================

#endif // !defined(AFX_SYSUTIL_H__363BCC4C_6969_4BB5_B3E4_F4A65D94718B__INCLUDED_)

// LogFile.h: interface for the CLogFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGFILE_H__B6E3FDA1_47B3_11D4_97A5_0050BF0FBE67__INCLUDED_)
#define AFX_LOGFILE_H__B6E3FDA1_47B3_11D4_97A5_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxmt.h>

class CLogFile  
{
public:
	static CCriticalSection Cs;
	CString strLogFile;

	CLogFile();
	CLogFile(LPCTSTR fname);
	virtual ~CLogFile();

	void SetLogFile(LPCTSTR fname);

	BOOL Write(char *pstr);
	BOOL Write(CString &str);
	BOOL Writef(char *mainmsg,...);

	BOOL WriteBackup(char *pData, UINT nSize);

};

#endif // !defined(AFX_LOGFILE_H__B6E3FDA1_47B3_11D4_97A5_0050BF0FBE67__INCLUDED_)

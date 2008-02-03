// LogFile.cpp: implementation of the CLogFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCriticalSection CLogFile::Cs;

CLogFile::CLogFile()
{
	strLogFile = "Log.txt";
}

CLogFile::CLogFile(LPCTSTR fname)
{
	strLogFile = fname;
}

CLogFile::~CLogFile()
{

}

void CLogFile::SetLogFile(LPCTSTR fname)
{
	strLogFile = fname;
}

BOOL CLogFile::Write(char *pstr)
{
	Cs.Lock();

	FILE *fp = NULL;
	char date[128]={0,};
	char time[128]={0,};

	_strdate(date);
	_strtime(time);

	fp = fopen((LPCTSTR)strLogFile, "a+" );
	
	if(fp == NULL) { Cs.Unlock(); return FALSE; }

	fseek(fp, 0, SEEK_END);
	
	if(fp == NULL) { Cs.Unlock(); return FALSE; }
	fprintf(fp, "\n⊙ Log Time : date[%s] time[%s]\n", date, time);
	fprintf(fp, "%s\n", pstr);
	fclose(fp);

	Cs.Unlock();
	return TRUE;
}

BOOL CLogFile::Write(CString &str)
{
	return Write((char*)str.operator LPCTSTR());
}

BOOL CLogFile::Writef(char *mainmsg,...)
{
	char buffer[1024];
	va_list argptr;

	va_start(argptr, mainmsg);
	vsprintf(buffer,mainmsg,argptr);
	va_end(argptr);

	return Write(buffer);
}

BOOL CLogFile::WriteBackup(char *pData, UINT nSize)
{
	Cs.Lock();

	if(pData == NULL || nSize<=0L) { Cs.Unlock(); return FALSE; }

	CString strTitle = _T("===============================================\r\n▲\"아이디\" 접속시의 보유 포커머니 (시:분:초) 서버명\r\n▽\"아이디\" 종료시의 포커머니 가감치 (시:분:초) 서버명\r\n===============================================\r\n");

	TRY
	{
		CFile fileBack((LPCTSTR)strLogFile,CFile::modeCreate|CFile::modeWrite);
		fileBack.Write(strTitle.operator LPCTSTR(),strTitle.GetLength());
		fileBack.Write((LPVOID)pData,nSize);
	}
	CATCH(CFileException,e)
	{
		Cs.Unlock(); return FALSE;
	}
	END_CATCH

	Cs.Unlock();
	return TRUE;
}


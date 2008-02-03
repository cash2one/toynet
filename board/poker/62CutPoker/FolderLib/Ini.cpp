// Ini.cpp: implementation of the CIni class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Ini.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIni::CIni()
{
	m_szFileName[0]='\0';
}

CIni::~CIni()
{

}

bool CIni::SetPath( CString &strFilename )
{
	return SetPath( strFilename.operator LPCTSTR() );
}

bool CIni::SetPath(const char *filename)
{
	char Buf[256], Path[256];
	char drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];

	::GetModuleFileName(AfxGetApp()->m_hInstance, Buf, 256);
	_splitpath(Buf,drive,dir,fname,ext);
	strcpy(Path, drive);
	strcat(Path, dir);		
	CString IniPath = Path;
	IniPath += filename;
	strcpy(m_szFileName,(LPCTSTR)IniPath);
	return true;
}

int CIni::GetProfileInt(LPCSTR lpAppName, LPCSTR lpKeyName, int nDefault )
{
	if (m_szFileName[0] == '\0')
	{
		m_nError = 0X01;
		m_strError = "File이 Set되지 않았습니다.";
		return -1;
	}

	GetPrivateProfileInt(lpAppName,lpKeyName,nDefault,m_szFileName);
	char tmpDefault[500];
	if(!GetPrivateProfileString(lpAppName,lpKeyName,"",tmpDefault,500,m_szFileName))
	{
		sprintf(tmpDefault,"%d",nDefault);
		WritePrivateProfileString(lpAppName,lpKeyName,tmpDefault,m_szFileName);
		return nDefault;
	}

	return GetPrivateProfileInt(lpAppName,lpKeyName,nDefault,m_szFileName);
}

CString& CIni::GetProfileString( LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpDefault )
{
	static CString retString;
	if(m_szFileName[0] == '\0')
	{
		m_nError = 0X01;
		m_strError = "File이 Set되지 않았습니다.";
		retString = "";
		return retString;
	}
	
	char tmpReturn[500];
	DWORD ret=GetPrivateProfileString(lpAppName,lpKeyName,"",tmpReturn,500,m_szFileName);
	if(!ret)
	{
		WritePrivateProfileString(lpAppName,lpKeyName,lpDefault,m_szFileName);
		strcpy(tmpReturn,lpDefault);
	}
	retString = tmpReturn;
	return retString;
}

int CIni::SetProfileInt( LPCSTR lpAppName, LPCSTR lpKeyName, int nDefault )
{
	if(m_szFileName[0] == '\0')
	{
		m_nError = 0X01;
		m_strError = "File이 Set되지 않았습니다.";
		return -1;
	}

	char tmpDefault[500];
	sprintf( tmpDefault, "%d", nDefault );
	if( WritePrivateProfileString( lpAppName, lpKeyName, tmpDefault, m_szFileName ) )
		return nDefault;
	return -1;
}

CString& CIni::SetProfileString( LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpDefault )
{
	static CString retString;
	retString.Empty();
	if (m_szFileName[0] == '\0')
	{
		m_nError = 0X01;
		m_strError = "File이 Set되지 않았습니다.";
		retString = "";
		return retString;
	}
	
	if( WritePrivateProfileString( lpAppName, lpKeyName, lpDefault, m_szFileName ) )
		retString = lpDefault;
	return retString;
}

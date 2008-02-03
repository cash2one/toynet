// Ini.h: interface for the CIni class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INI_H__6D347B43_569C_11D3_9538_0060979C1667__INCLUDED_)
#define AFX_INI_H__6D347B43_569C_11D3_9538_0060979C1667__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIni  
{
private:
	char		m_szFileName[200];
	int			m_nError;
	CString		m_strError;

public:
	int			SetProfileInt( LPCSTR lpAppName, LPCSTR lpKeyName, int nDefault );
	CString		&SetProfileString( LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpDefault );
	int			GetProfileInt(LPCSTR lpAppName, LPCSTR lpKeyName, int nDefault );
	CString		&GetProfileString( LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpDefault );

	bool		SetPath( CString &strFilename );
	bool		SetPath( const char* filename );
	CIni();
	virtual ~CIni();

};

#endif // !defined(AFX_INI_H__6D347B43_569C_11D3_9538_0060979C1667__INCLUDED_)

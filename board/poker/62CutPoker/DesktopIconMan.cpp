l// DesktopIconMan.cpp: implementation of the CDesktopIconMan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "62CutPoker.h"
#include "DesktopIconMan.h"
#include "ShortCut.h"
#include "Atlbase.h"
#include "Global.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDesktopIconMan::CDesktopIconMan()
{
	m_hkey = NULL;
}

CDesktopIconMan::~CDesktopIconMan()
{

}

BOOL CDesktopIconMan::Check( CString ProgName, CString url, CString ShortcutName, CString Desc )//, DWORD RecvVersion )
{
	// 프로그램 이름
	LPCTSTR strProgName = ProgName;
	// URL
	LPCSTR pszURL = url;
	// 바로가기 이름
	LPCSTR pszlnkName = ShortcutName;
	// 바로가기 설명
	LPCSTR szDescription = Desc;

	CRegKey Key;
	int ShortCutVersion = 0;
	int RegVersion = 0;

	// 바로가기 아이콘의 버전을 읽어옴
	char strkey[500]={0,};
	::wsprintf(strkey, "Software\\Netmarble\\%s", strProgName);
	int rtn = Key.Open(HKEY_LOCAL_MACHINE, strkey); 
	if(rtn == ERROR_SUCCESS) {
		char verstr[100]={0,};
		unsigned long vstrlen = 100;

		rtn = Key.QueryValue(verstr, "Version", &vstrlen);
		if(rtn == ERROR_SUCCESS){
			RegVersion = ::atoi(verstr);
		}

		rtn = Key.QueryValue(verstr, "ShortCutVersion", &vstrlen);
		if(rtn == ERROR_SUCCESS) 
		{
			ShortCutVersion = ::atoi(verstr);
		}
	}

	if( ShortCutVersion != RegVersion && RegVersion != 0 )
	{
		// 레지스트리에 바로가기 아이콘의 버전을 설정(키가 없을땐 키를 생성)
		char verstr[100]={0,};
		wsprintf(verstr, "%d", RegVersion);
		Key.SetValue(verstr, "ShortCutVersion");

		// 실행파일명을 얻음(실행파일의 아이콘을 사용하기 위함)
		char szFileName[MAX_PATH]={0,};
		GetModuleFileName(AfxGetInstanceHandle(), szFileName, MAX_PATH-1);
		
		// 인터넷 바로가기 아이콘을 바탕화면에 생성
		CreateInternetShortCutToDesktopFolder(pszURL, pszlnkName, szDescription, szFileName, 0);
	}
	return FALSE;
}

void CDesktopIconMan::MakeIconFile()
{
	// 알아서 바꿔야 할 부분..
	Copy( "굳보이7포커.url", "netmarble.ico", "http://www.goodboy7poker.com" );
	Copy( "고스톱.url", "ngostop.ico", "http://www.netmarble.net" );
	Copy( "세븐포커.url", "nsevenpoker.ico", "http://www.netmarble.net" );
}

void CDesktopIconMan::Copy( LPCTSTR str, LPCTSTR icon, LPCTSTR url )
{
/*
	[InternetShortcut]
	URL=http://www.Netmarble.net/
	IconFile=http://web.netmarble.feelamint.com/ngostop.ico
	IconIndex=1
*/
	char PathBuf[400];
	ZeroMemory( PathBuf, sizeof(PathBuf) );

	FILE* destfp = NULL;
	CString dest;

	if( SHGetSpecialFolderPath( NULL, PathBuf, CSIDL_DESKTOPDIRECTORY, 0 ) != NOERROR ) return;
	if( strlen(PathBuf) <= 0 ) return;
	dest.Format( "%s\\%s", PathBuf, str );
	destfp = fopen( (LPCTSTR)dest, "r" );
	if( destfp != NULL ) return;

	ZeroMemory( PathBuf, 400 );
	if( SHGetSpecialFolderPath( NULL, PathBuf, CSIDL_COMMON_DESKTOPDIRECTORY, 0 ) != NOERROR ) return;
	if( strlen(PathBuf) <= 0 ) return;
	dest.Format( "%s\\%s", PathBuf, str );
	destfp = fopen( (LPCTSTR)dest, "r" );
	if( destfp != NULL ) return;

	dest.Format( "%s\\%s", PathBuf, str );
	destfp = fopen( (LPCTSTR)dest, "wt" );
	if( destfp == NULL ) return;
	CString scut_str;
	GetCurrentDirectory( 400, PathBuf );
	scut_str.Format( "[InternetShortcut]\nURL=%s\nIconFile=%s\\%s\nIconIndex=0", url, PathBuf, icon );
	fprintf( destfp, "%s", (LPCTSTR)scut_str );
	fclose( destfp );
}
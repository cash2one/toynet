// GetFolder.cpp: implementation of the CSelectFolder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GetFolder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CString		g_strPath;

CGetFolder::CGetFolder()
{
}

CGetFolder::~CGetFolder()
{
}

int CALLBACK CGetFolder::BrowseCallbackProc( HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData )
{
	TCHAR szDir[ MAX_PATH ];
	switch( uMsg )
	{
		case BFFM_INITIALIZED:
		{
			if( lpData )
			{
				strcpy( szDir, g_strPath.GetBuffer( g_strPath.GetLength() ) );
				::SendMessage( hwnd, BFFM_SETSELECTION, TRUE, ( LPARAM )szDir );
			}
			break;
		}
		case BFFM_SELCHANGED:
		{
		   if( SHGetPathFromIDList( ( LPITEMIDLIST ) lParam ,szDir ) )
		   {
			   ::SendMessage( hwnd, BFFM_SETSTATUSTEXT, 0, ( LPARAM )szDir );
		   }
		   break;
		}
	}
         
	return 0;
}

//	폴더 선택하기
BOOL CGetFolder::GetFolder( CString* strSelectedFolder, const char* lpszTitle, const HWND hwndOwner, const char* strRootFolder, const char* strStartFolder )
{
	char pszDisplayName[ MAX_PATH ] = { 0, };
	LPITEMIDLIST lpID;
	BROWSEINFOA bi;
	
	bi.hwndOwner = hwndOwner;
	if( strRootFolder == NULL )
	{
		bi.pidlRoot = NULL;
	}
	else
	{
	   LPITEMIDLIST  pIdl = NULL;
	   IShellFolder* pDesktopFolder;
	   char          szPath[ MAX_PATH ];
	   OLECHAR       olePath[ MAX_PATH ];
	   ULONG         chEaten;
	   ULONG         dwAttributes;

	   strcpy( szPath, ( LPCTSTR )strRootFolder );
	   if( SUCCEEDED( SHGetDesktopFolder( &pDesktopFolder ) ) )
	   {
		   MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szPath, -1, olePath, MAX_PATH );
		   pDesktopFolder->ParseDisplayName( NULL, NULL, olePath, &chEaten, &pIdl, &dwAttributes );
		   pDesktopFolder->Release();
	   }
	   bi.pidlRoot = pIdl;
	}

	bi.pszDisplayName = pszDisplayName;
	bi.lpszTitle = lpszTitle;
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	bi.lpfn = BrowseCallbackProc;
	if( strStartFolder == NULL )
	{
		bi.lParam = FALSE;
	}
	else
	{
		g_strPath.Format( "%s", strStartFolder );
		bi.lParam = TRUE;
	}

	bi.iImage = NULL;
	lpID = SHBrowseForFolderA( &bi );
	if( lpID != NULL )
	{
		BOOL b = SHGetPathFromIDList( lpID, pszDisplayName );
		if( b == TRUE )
		{
			strSelectedFolder->Format( "%s", pszDisplayName );
			return TRUE;
		}
	}
	else
	{
		strSelectedFolder->Empty();
	}

	return FALSE;
}

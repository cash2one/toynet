// ShortCut.cpp: implementation of the CShortCut class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ShortCut.h"
#include <atlbase.h>
#include <intshcut.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// 브라우저 시작페이지 변경하기
int SetIEStartPage(char *sPage)
{
	HKEY hKeyInfo;
	LONG lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\Microsoft\\Internet Explorer\\Main",0,KEY_ALL_ACCESS,&hKeyInfo);
	if(lResult == ERROR_SUCCESS)
		RegSetValueEx(hKeyInfo,"Start Page",0,REG_SZ,(const BYTE*)sPage,strlen(sPage));
	RegCloseKey(hKeyInfo);

	lResult = RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\Microsoft\\Internet Explorer\\Main",0,KEY_ALL_ACCESS,&hKeyInfo);
	if(lResult == ERROR_SUCCESS)
		RegSetValueEx(hKeyInfo,"Start Page",0,REG_SZ,(const BYTE*)sPage,strlen(sPage));
	RegCloseKey(hKeyInfo);
	return 1;
}



///////////////////////////////////////////////////////////////////////////////
// Program Files위치 알아내기
int GetProgramFilePath(char *PathBuffer, int DefualtAll)
{  
	LPITEMIDLIST	pidl = NULL;	
	if(DefualtAll==0)
	{
		// Platform SDK 필요함
		// SHGetSpecialFolderLocation(NULL, CSIDL_PROGRAM_FILES, &pidl);	
		if (pidl == NULL)	
		{
			CRegKey Key;
			char   sPath[MAX_PATH];
			char sKey[512] = {0,};
			DWORD nPathSize = 256;
			wsprintf(sKey,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion");
			int rt = Key.Open(HKEY_LOCAL_MACHINE,sKey);
			if(rt != ERROR_SUCCESS)
			{
				sprintf(PathBuffer,"C:\\Program Files");
				return 1;
			}
			if(ERROR_SUCCESS == Key.QueryValue(sPath,"ProgramFilesDir",&nPathSize))
			{
				sprintf(PathBuffer,"C:\\Program Files");
			} else sprintf(PathBuffer,"%s",sPath);
			Key.Close();
			//SHGetSpecialFolderLocation(NULL, CSIDL_PROGRAM_FILES , &pidl);
		} 
		else 
			SHGetPathFromIDList(pidl, PathBuffer);
	} 
	else
	{
		// Platform SDK 필요함
		//SHGetSpecialFolderLocation(NULL, CSIDL_PROGRAM_FILES , &pidl);	
		//if (pidl == NULL)	SHGetSpecialFolderLocation(NULL, CSIDL_PROGRAM_FILES , &pidl);	
		if(pidl) SHGetPathFromIDList(pidl, PathBuffer);
	}

	return 1;
}


///////////////////////////////////////////////////////////////////////////////
// 데스크탑 폴더 위치 알아내기

int GetDeskTopFolder(char *PathBuffer, int DefualtAll)
{ 
	LPITEMIDLIST	pidl;	
	if(DefualtAll==0)
	{
		SHGetSpecialFolderLocation(NULL, CSIDL_COMMON_DESKTOPDIRECTORY, &pidl);	
		if (pidl == NULL)	SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOPDIRECTORY, &pidl);	
		SHGetPathFromIDList(pidl, PathBuffer);
	} 
	else
	{
		SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOPDIRECTORY, &pidl);	
		if (pidl == NULL)	SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOPDIRECTORY, &pidl);	
		SHGetPathFromIDList(pidl, PathBuffer);
	}

	return 1;
}


/////////////////////////////////////////////////////////////////////////////////
//  시작 - 프로그램폴더에 등록
//
int GetCommonStartupFolder(char *PathBuffer,int DefualtAll)
{
	LPITEMIDLIST	pidl;	
	if(DefualtAll==0)
	{
		SHGetSpecialFolderLocation(NULL, CSIDL_COMMON_STARTMENU, &pidl);	
		if (pidl == NULL)	
		{
			SHGetSpecialFolderLocation(NULL, CSIDL_STARTMENU, &pidl);	
		}
		SHGetPathFromIDList(pidl, PathBuffer);
	} else
	{
		SHGetSpecialFolderLocation(NULL, CSIDL_STARTMENU, &pidl);	
		if (pidl == NULL)	SHGetSpecialFolderLocation(NULL, CSIDL_STARTMENU, &pidl);	
		SHGetPathFromIDList(pidl, PathBuffer);
	}
	return 1;
}

HRESULT CreateInternetShortcut(LPCSTR pszURL, LPCSTR pszURLfilename, LPCSTR szDescription, LPCTSTR szIconFile, int nIndex)
{
	HRESULT hres;

	CoInitialize(NULL); 
	
	IUniformResourceLocator *pHook;

	hres = CoCreateInstance (CLSID_InternetShortcut, NULL, CLSCTX_INPROC_SERVER,
		IID_IUniformResourceLocator, (void **)&pHook);

	if (SUCCEEDED (hres))
	{
		IPersistFile *ppf;
		IShellLink *psl;

		// Query IShellLink for the IPersistFile interface for 
		hres = pHook->QueryInterface (IID_IPersistFile, (void **)&ppf);
		hres = pHook->QueryInterface (IID_IShellLink, (void **)&psl);

		if (SUCCEEDED (hres))
		{ 
			WORD wsz [MAX_PATH]; // buffer for Unicode string

			// Set the path to the shortcut target.
			pHook->SetURL(pszURL,0);

			hres = psl->SetIconLocation(szIconFile,nIndex);

			if (SUCCEEDED (hres))
			{
				// Set the description of the shortcut.
				hres = psl->SetDescription (szDescription);

				if (SUCCEEDED (hres))
				{
					// Ensure that the string consists of ANSI characters.
					MultiByteToWideChar (CP_ACP, 0, pszURLfilename, -1, wsz, MAX_PATH);

					// Save the shortcut via the IPersistFile::Save member function.
					hres = ppf->Save (wsz, TRUE);
				}

			}

			// Release the pointer to IPersistFile.
			ppf->Release ();
			psl->Release ();
		}

		// Release the pointer to IShellLink.
		pHook->Release ();
	}

	CoUninitialize();
	return hres;
}

HRESULT MakeShortCut(char* szOrigin, char* szlnkName, char* szPath, char* szDescript)
{
	int length;
	HRESULT hres;
	IShellLink *psl;
	char posdancuk[256];
	CoInitialize(NULL);
	hres=CoCreateInstance(CLSID_ShellLink, NULL,CLSCTX_INPROC_SERVER,
		IID_IShellLink, (LPVOID*)&psl);
	if(SUCCEEDED(hres))
	{
		IPersistFile *ppf;
		hres=psl->QueryInterface(IID_IPersistFile, (void **)&ppf);
		if(SUCCEEDED(hres))
		{
			WORD wsz[256];
			psl->SetPath(szOrigin);
			psl->SetArguments("");

			/*
			char Drive[MAX_PATH];
			for(length=0; length<256; length++)
			{
				if(szOrigin[length] != '\\')
					Drive[length]=szOrigin[length];
				else
				{
					Drive[length]='\0';
					break;
				}
			}
			int maxn = strlen(szOrigin)-1;
		
			psl->SetWorkingDirectory(Drive);
			*/
			
			char WorkDir[MAX_PATH] = {0};
			for(length=strlen(szOrigin); length>=0; length--) {
				if(szOrigin[length] == '\\') break;
			}
			strncpy(WorkDir, szOrigin, length);

			int maxn = strlen(szOrigin)-1;
		
			psl->SetWorkingDirectory(WorkDir);
			psl->SetIconLocation(NULL, 1);
			psl->SetHotkey(0);
			psl->SetShowCmd(0);

			//
			//char Descript[256];
			//sprintf(Descript,"%s", szDescript);
			psl->SetDescription(szDescript);
			strcpy(posdancuk,szPath);
			strcat(posdancuk,szlnkName);
			strcat (posdancuk,".lnk");
			MultiByteToWideChar(CP_ACP, 0, posdancuk, -1, (LPWSTR)wsz,MAX_PATH);
			hres = ppf->Save( (LPWSTR)wsz, TRUE);
			if (! SUCCEEDED(hres)) 
				return hres;
			ppf->Release();
		}
		psl->Release();
	}
	CoUninitialize();
	return TRUE;
}

/*
void MakeShortCutToDesktopFolder()
{
	char DeskTopPath[MAX_PATH];
	char szBuffer[512];
	// 실행된 프로그램의 실행파일명을 얻어옴
	GetModuleFileName(AfxGetInstanceHandle(), szBuffer, 512);

	if(GetDeskTopFolder(DeskTopPath, 0))
	{
		int nLen = strlen(DeskTopPath);
		if(DeskTopPath[nLen] != '\\') ::strcat(DeskTopPath,"\\");
		
		if(!SUCCEEDED(MakeShortCut(szBuffer, "노라조 게임", DeskTopPath)))
		{
			if(GetDeskTop(DeskTopPath,1))
			{
				nLen = strlen(DeskTopPath);
				if(DeskTopPath[nLen] != '\\') ::strcat(DeskTopPath,"\\");
				MakeShortCut(szBuffer, "노라조 게임", DeskTopPath);
			}
		}
	}
}
*/

BOOL CreateInternetShortCutToDesktopFolder(LPCSTR pszURL, LPCSTR pszlnkName, LPCSTR szDescription, LPCTSTR szIconFile, int nIndex)
{
	char DeskTopPath[MAX_PATH] = {0,};
	char URLfinename[MAX_PATH] = {0,};

	if(GetDeskTopFolder(DeskTopPath, 0))
	{
		int nLen = strlen(DeskTopPath);
		if(DeskTopPath[nLen] != '\\') ::strcat(DeskTopPath,"\\");
		wsprintf(URLfinename, "%s%s.url", DeskTopPath, pszlnkName);

		if(!SUCCEEDED(CreateInternetShortcut(pszURL, URLfinename, szDescription, szIconFile, nIndex)))
		{
			if(GetDeskTopFolder(DeskTopPath, 1))
			{
				nLen = strlen(DeskTopPath);
				if(DeskTopPath[nLen] != '\\') ::strcat(DeskTopPath,"\\");
				wsprintf(URLfinename, "%s%s.url", DeskTopPath, pszlnkName);
				return SUCCEEDED(CreateInternetShortcut(pszURL, URLfinename, szDescription, szIconFile, nIndex));
			}
		}
	}

	return FALSE;
}
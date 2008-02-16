// ShortCut.h: interface for the CShortCut class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHORTCUT_H__167DC44C_8774_44A4_9F41_4D00601B61AE__INCLUDED_)
#define AFX_SHORTCUT_H__167DC44C_8774_44A4_9F41_4D00601B61AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

int SetIEStartPage(char *sPage);
int GetProgramFilePath(char *PathBuffer, int DefualtAll=0);
int GetCommonStartupFolder(char *PathBuffer, int DefualtAll=0);
int GetDeskTopFolder(char *PathBuffer, int DefualtAll=0);

BOOL CreateInternetShortCutToDesktopFolder(LPCSTR pszURL, LPCSTR pszlnkName, 
										   LPCSTR szDescription, LPCTSTR szIconFile, int nIndex);


#endif // !defined(AFX_SHORTCUT_H__167DC44C_8774_44A4_9F41_4D00601B61AE__INCLUDED_)

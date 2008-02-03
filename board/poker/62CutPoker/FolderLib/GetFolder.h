// GetFolder.h: interface for the CSelectFolder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GETFOLDER_H__2606C971_B9CB_4CBE_85B8_75576417BDA9__INCLUDED_)
#define AFX_GETFOLDER_H__2606C971_B9CB_4CBE_85B8_75576417BDA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGetFolder
{
public:


private:



public:


	static int CALLBACK		BrowseCallbackProc( HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData );
	BOOL					GetFolder( CString* strSelectedFolder, const char* lpszTitle, const HWND hwndOwner, const char* strRootFolder, const char* strStartFolder );


public:
	CGetFolder();
	virtual ~CGetFolder();

};

#endif // !defined(AFX_GETFOLDER_H__2606C971_B9CB_4CBE_85B8_75576417BDA9__INCLUDED_)

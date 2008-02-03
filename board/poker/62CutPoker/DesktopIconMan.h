// DesktopIconMan.h: interface for the CDesktopIconMan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DESKTOPICONMAN_H__640CF097_BAA8_4566_A90B_C864A7D6131B__INCLUDED_)
#define AFX_DESKTOPICONMAN_H__640CF097_BAA8_4566_A90B_C864A7D6131B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDesktopIconMan  
{

public:

	CDesktopIconMan();
	virtual ~CDesktopIconMan();
	
	BOOL Check( CString ProgName, CString url, CString ShortcutName, CString Desc );//, DWORD Version );

protected:

	void MakeIconFile();
	void Copy(LPCTSTR str, LPCTSTR icon, LPCTSTR url );

	HKEY	m_hkey;

};

#endif // !defined(AFX_DESKTOPICONMAN_H__640CF097_BAA8_4566_A90B_C864A7D6131B__INCLUDED_)

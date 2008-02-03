// NoChat.h: interface for the CNoChat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NOCHAT_H__7E121B71_8609_4115_97CE_9E0DA7DB1224__INCLUDED_)
#define AFX_NOCHAT_H__7E121B71_8609_4115_97CE_9E0DA7DB1224__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNoChat  
{
public:
	DWORD NoChatTimeTick;
	BOOL bNoChat;

	int NowHPos;
	DWORD AddTime[10];
	CString History[10];

	CNoChat();
	virtual ~CNoChat();

	BOOL CheckNoChat(CString &str);
	BOOL CheckNoChat(LPCTSTR pstr);

	void AddChat(LPCTSTR pstr);

};

#endif // !defined(AFX_NOCHAT_H__7E121B71_8609_4115_97CE_9E0DA7DB1224__INCLUDED_)

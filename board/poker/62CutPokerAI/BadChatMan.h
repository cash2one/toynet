// BadChatMan.h: interface for the BadChatMan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BADCHATMAN_H__DDF826EB_D56A_4822_9AD2_20DF259F2F84__INCLUDED_)
#define AFX_BADCHATMAN_H__DDF826EB_D56A_4822_9AD2_20DF259F2F84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxcoll.h>
#include "BadChat.h"

#define  BADCHAT_HEADER ("NETMARBLE PROGRAM (www.netmarble.net)")

class CBadChatMan:public CObject  
{
protected:
	DECLARE_SERIAL(CBadChatMan)
public:
	BOOL Change(CString strPre,CString strNew);
	BOOL Compare(CString strData);
	BOOL Save(CString strFileName);
	BOOL Open(CString strFileName);

	void Clear(CString strData);
	void Clear();
	BOOL Add(CString strData);

	BOOL IsBad(CString strData);

	CObList  m_LIST;
	CString  m_strHeader;
    int  	 m_nTotalNum;

	CBadChatMan();
	virtual ~CBadChatMan();
	virtual void Serialize(CArchive &ar);
};

#endif // !defined(AFX_BADCHATMAN_H__DDF826EB_D56A_4822_9AD2_20DF259F2F84__INCLUDED_)

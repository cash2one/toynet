// BadChat.h: interface for the CBadChat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BADCHAT_H__47F7AE3B_15C9_4BCF_BAE3_FE73F2F0CEB7__INCLUDED_)
#define AFX_BADCHAT_H__47F7AE3B_15C9_4BCF_BAE3_FE73F2F0CEB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBadChat:public CObject
{
protected:
	DECLARE_SERIAL(CBadChat)
public:
	int      m_nSize;
	CString  m_strBad;

	BOOL Compare(CString strDes);
	void Code();

	CBadChat();
	virtual ~CBadChat();
	CBadChat& operator=( CBadChat &assign );
	virtual void Serialize(CArchive &ar);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

#endif // !defined(AFX_BADCHAT_H__47F7AE3B_15C9_4BCF_BAE3_FE73F2F0CEB7__INCLUDED_)

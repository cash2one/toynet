// BadChat.cpp: implementation of the CBadChat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BadChat.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CBadChat,CObject,1)

CBadChat::CBadChat()
{
	m_nSize  =      0;
	m_strBad = _T("");
}

CBadChat::~CBadChat()
{
}

void CBadChat::Serialize(CArchive &ar)
{
	CObject::Serialize(ar);

	if(ar.IsStoring()){
		Code(); ar<<m_nSize<<m_strBad; Code();
	}
	else{
		ar>>m_nSize>>m_strBad; Code();
	}
}

BOOL CBadChat::Compare(CString strDes)
{
	if(strDes==_T("")) return FALSE;
	if(m_strBad==_T("") || m_nSize<=0) return FALSE;

	if(m_strBad==strDes) return TRUE;

	return FALSE;
}

void CBadChat::Code()
{
	if(m_strBad==_T("") || m_nSize<=0) return;
	for(int i=0; i<m_strBad.GetLength(); i++){
		TCHAR ch = m_strBad.GetAt(i)^'c'^'t'^'r';
		m_strBad.SetAt(i,ch);
	}
}

#ifdef _DEBUG
void CBadChat::AssertValid() const
{
	CObject::AssertValid();
}
void CBadChat::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
}
#endif


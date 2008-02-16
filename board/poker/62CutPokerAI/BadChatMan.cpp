// BadChatMan.cpp: implementation of the BadChatMan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BadChatMan.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CBadChatMan,CObject,1)

CBadChatMan::CBadChatMan()
{
	m_strHeader = BADCHAT_HEADER; // 인식 문자.
	Clear();
}

CBadChatMan::~CBadChatMan()
{
	Clear();
}

void CBadChatMan::Serialize(CArchive &ar)
{
	CObject::Serialize(ar);

	if (ar.IsStoring())
	{
		ar<<m_strHeader<<m_nTotalNum;// 헤더 저장.
	}
	else
	{
		ar>>m_strHeader>>m_nTotalNum;// 헤더읽기.
	}
	m_LIST.Serialize(ar);
}

void CBadChatMan::Clear()
{
	while(!m_LIST.IsEmpty()){
		delete (CBadChat *)m_LIST.RemoveHead();
	}
    m_nTotalNum = 0;
}

BOOL CBadChatMan::IsBad(CString strData)
{
	if(m_nTotalNum<=0 || strData=="") return FALSE;

	POSITION pos = m_LIST.GetHeadPosition();

	BOOL    bFind  = FALSE;
	CString strBad = _T("");
	
	strData.MakeUpper();

	while(pos!=NULL){

		strBad = ((CBadChat *)m_LIST.GetAt(pos))->m_strBad;
		if(strData.Find(strBad.operator LPCTSTR(),0)>=0){
			bFind = TRUE;
			break;
		}
		(CBadChat *)m_LIST.GetNext(pos);
	}

	return bFind;
}

BOOL CBadChatMan::Add(CString strData)
{
	if(strData=="") return FALSE;

	POSITION pos = m_LIST.GetHeadPosition();

	strData.MakeUpper();

	if(Compare(strData)) return FALSE;

	CBadChat *pData;
	pData = new CBadChat;
	if(pData==NULL) return FALSE;

	pData->m_strBad = strData;
	pData->m_nSize  = strData.GetLength();

	m_LIST.AddTail((CObject *)pData);
	m_nTotalNum++;

	return TRUE;
}

BOOL CBadChatMan::Compare(CString strData)
{
	if(strData=="") return FALSE;

	POSITION pos = m_LIST.GetHeadPosition();

	BOOL    bFind  = FALSE;
	
	strData.MakeUpper();

	while(pos!=NULL){

		if(((CBadChat *)m_LIST.GetAt(pos))->Compare(strData)){
			bFind = TRUE;
			break;
		}
		(CBadChat *)m_LIST.GetNext(pos);
	}

	return bFind;
}

BOOL CBadChatMan::Change(CString strPre, CString strNew)
{
	if(strPre=="" || strNew=="") return FALSE;

	POSITION pos = m_LIST.GetHeadPosition();

	BOOL    bChange  = FALSE;
	
	strPre.MakeUpper();
	strNew.MakeUpper();

	if(strPre == strNew) return FALSE;

	if(Compare(strNew)) return FALSE;

	while(pos!=NULL){

		if(((CBadChat *)m_LIST.GetAt(pos))->Compare(strPre)){
			((CBadChat *)m_LIST.GetAt(pos))->m_strBad = strNew;
			((CBadChat *)m_LIST.GetAt(pos))->m_nSize  = strNew.GetLength();
			bChange = TRUE;
			break;
		}
		(CBadChat *)m_LIST.GetNext(pos);
	}

	return bChange;
}

void CBadChatMan::Clear(CString strData)
{
	if(strData=="") return;

	CObject* pa=NULL;
	POSITION pos1, pos2;

	strData.MakeUpper();

	for( pos1 = m_LIST.GetHeadPosition(); ( pos2 = pos1 ) != NULL; ){
		if( ((CBadChat*)m_LIST.GetNext( pos1 ))->m_strBad == strData){
			pa = m_LIST.GetAt( pos2 );
			m_LIST.RemoveAt( pos2 );
			
			if(pa) delete pa; pa = NULL;
			
			m_nTotalNum--;
			if(m_nTotalNum<0) m_nTotalNum = 0;
		}
	}
}

BOOL CBadChatMan::Open(CString strFileName)
{
	if(strFileName=="") return FALSE;

	CFile fileO;
	CFileException e;
	if( !fileO.Open( strFileName, CFile::modeRead, &e ) ) return FALSE;

	CArchive ar(&fileO,CArchive::load);
	Serialize(ar);

	fileO.Close();

	if(	m_strHeader != BADCHAT_HEADER ) return FALSE; // 인식 문자.
	if( m_nTotalNum != m_LIST.GetCount()) return FALSE;

	return TRUE;
}

BOOL CBadChatMan::Save(CString strFileName)
{
	if(strFileName=="" || m_nTotalNum<=0) return FALSE;

	CFile fileS(strFileName,CFile::modeCreate|CFile::modeWrite);

	CArchive ar(&fileS,CArchive::store);
	Serialize(ar);

	fileS.Close();

	return TRUE;
}

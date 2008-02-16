// NMStrMan.cpp : implementation file
//

#include "stdafx.h"
#include "NMStrMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNMStrMan

IMPLEMENT_SERIAL(CNMStrMan,CObject,1)

CNMStrMan::CNMStrMan()
{
	m_strHeader = STRDATA_HEADER; // 인식 문자.
	Clear();
}

CNMStrMan::~CNMStrMan()
{
	Clear();
}

/////////////////////////////////////////////////////////////////////////////
// CNMStrMan member functions

void CNMStrMan::Clear()
{
	while(!m_LIST.IsEmpty()){
		delete (CNMStr *)m_LIST.RemoveHead();
	}
    m_nTotalNum = 0;
}

void CNMStrMan::Clear(CString strID)
{
	if(strID==_T("")) return;

	CObject* pa=NULL;
	POSITION pos1, pos2;

	for( pos1 = m_LIST.GetHeadPosition(); ( pos2 = pos1 ) != NULL; ){
		if( ((CNMStr*)m_LIST.GetNext( pos1 ))->CompareID(strID)){
			pa = m_LIST.GetAt( pos2 );
			m_LIST.RemoveAt( pos2 );
			
			if(pa) delete pa; pa = NULL;
			
			m_nTotalNum--;
			if(m_nTotalNum<0) m_nTotalNum = 0;
		}
	}
}

void CNMStrMan::Serialize(CArchive &ar)
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

BOOL CNMStrMan::Open(CString strFileName)
{
	if(strFileName==_T("")) return FALSE;

	CFile fileO;
	CFileException e;
	if( !fileO.Open( strFileName, CFile::modeRead, &e ) ) return FALSE;

	CArchive ar(&fileO,CArchive::load);
	Serialize(ar);

	fileO.Close();

	if(	m_strHeader != STRDATA_HEADER ) return FALSE; // 인식 문자.
	if( m_nTotalNum != m_LIST.GetCount()) return FALSE;

	return TRUE;
}

BOOL CNMStrMan::Save(CString strFileName)
{
	if(strFileName==_T("") || m_nTotalNum<=0) return FALSE;

	CFile fileS(strFileName,CFile::modeCreate|CFile::modeWrite);

	CArchive ar(&fileS,CArchive::store);
	Serialize(ar);

	fileS.Close();

	return TRUE;
}

char * CNMStrMan::_Get(CString strID)
{
	if(m_nTotalNum<=0 || strID==_T("")) return _T("");
	return (char *)(Get(strID).operator LPCTSTR());
}

CString CNMStrMan::Get(CString strID)
{
	if(m_nTotalNum<=0 || strID==_T("")) return _T("");

	CString  strRes = _T("");
	CNMStr  *pObj   = NULL;
	
	pObj = GetObj(strID);
	if(pObj!=NULL) strRes = pObj->Get();

	return strRes;
}

CNMStr * CNMStrMan::GetObj(CString strID)
{
	CNMStr *pObj = NULL;

	if(m_nTotalNum<=0 || strID==_T("")) return NULL;

	POSITION pos = m_LIST.GetHeadPosition();

	while(pos!=NULL){

		if(((CNMStr *)m_LIST.GetAt(pos))->CompareID(strID)){
			pObj = (CNMStr *)m_LIST.GetAt(pos);
			break;
		}
		(CNMStr *)m_LIST.GetNext(pos);
	}

	return pObj;
}

// 문자열 데이타를 비교한다
BOOL CNMStrMan::Compare(CString strData)
{
	if(strData==_T("")) return FALSE;

	POSITION pos = m_LIST.GetHeadPosition();

	BOOL    bFind  = FALSE;
	
	while(pos!=NULL){

		if(((CNMStr *)m_LIST.GetAt(pos))->Compare(strData)){
			bFind = TRUE;
			break;
		}
		(CNMStr *)m_LIST.GetNext(pos);
	}

	return bFind;
}

// 문자열 아이디를 비교한다
BOOL CNMStrMan::CompareID(CString strData)
{
	if(strData==_T("")) return FALSE;

	POSITION pos = m_LIST.GetHeadPosition();

	BOOL    bFind  = FALSE;
	
	while(pos!=NULL){

		if(((CNMStr *)m_LIST.GetAt(pos))->CompareID(strData)){
			bFind = TRUE;
			break;
		}
		(CNMStr *)m_LIST.GetNext(pos);
	}

	return bFind;
}

// 새로운 문자열 데이타를 세팅한다
BOOL CNMStrMan::Add(CNMStr *pData)
{
	if(pData==NULL) return FALSE;

	return Add(pData->GetID(),pData->Get());
}

// 새로운 문자열 데이타를 세팅한다
BOOL CNMStrMan::Add(CString strID, CString strData)
{	
	BOOL bRes = FALSE;

	if(strID==_T("")) return FALSE;

	POSITION pos = m_LIST.GetHeadPosition();

	if(CompareID(strID)) return FALSE;

	CNMStr *pData = NULL;
	pData = new CNMStr;
	if(pData==NULL) return FALSE;

	pData->SetStr(strID,strData);

	m_LIST.AddTail((CObject *)pData);
	m_nTotalNum++;

	return TRUE;
}

BOOL CNMStrMan::ChangeID(CString strID, CString strNew)
{
	if(strID==_T("") || strNew==_T("") || strID.CompareNoCase(strNew.operator LPCTSTR())==0) return FALSE;

	POSITION pos = m_LIST.GetHeadPosition();

	BOOL    bChange  = FALSE;

	while(pos!=NULL){

		if(((CNMStr *)m_LIST.GetAt(pos))->CompareID(strID)){
			((CNMStr *)m_LIST.GetAt(pos))->SetID(strNew);
			bChange = TRUE;
			break;
		}
		(CNMStr *)m_LIST.GetNext(pos);
	}

	return bChange;
}

BOOL CNMStrMan::Change(CString strID, CString strData)
{
	if(strID==_T("")) return FALSE;

	POSITION pos = m_LIST.GetHeadPosition();

	BOOL    bChange  = FALSE;

	while(pos!=NULL){

		if(((CNMStr *)m_LIST.GetAt(pos))->CompareID(strID)){
			((CNMStr *)m_LIST.GetAt(pos))->SetData(strData);
			bChange = TRUE;
			break;
		}
		(CNMStr *)m_LIST.GetNext(pos);
	}

	return bChange;
}

BOOL CNMStrMan::Switch(CString strID1, CString strID2)
{
	if(strID1==_T("") || strID2==_T("") || strID1.CompareNoCase(strID2.operator LPCTSTR())==0 ) return FALSE;

	CNMStr *pStr1=NULL, *pStr2=NULL;

	pStr1 = GetObj(strID1);
	pStr2 = GetObj(strID2);

	if(pStr1==NULL || pStr2==NULL) return FALSE;

	CNMStr Data1,Data2;
	Data1 = *pStr1;
	Data2 = *pStr2;

	*pStr1 = Data2;
	*pStr2 = Data1;

	return TRUE;
}

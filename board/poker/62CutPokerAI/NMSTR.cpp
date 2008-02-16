// StringData.cpp : implementation file
//

#include "stdafx.h"
#include "NMStr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNMStr

IMPLEMENT_SERIAL(CNMStr,CObject,1)

CNMStr::CNMStr()
{
	Clear();
}

CNMStr::~CNMStr()
{
	Clear();
}

#ifdef _DEBUG
void CNMStr::AssertValid() const
{
	CObject::AssertValid();
}
void CNMStr::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
}
#endif

/////////////////////////////////////////////////////////////////////////////
// CNMStr member functions

void CNMStr::Clear()
{
	m_nIDSize   = 0;
	m_nDataSize = 0;

	m_strID     = _T("");
	m_strData   = _T("");
}

CNMStr& CNMStr::operator=( CNMStr &assign )
{
	Clear();

	m_nIDSize   = assign.m_nIDSize;
	m_nDataSize = assign.m_nDataSize;

	m_strID     = assign.m_strID;
	m_strData   = assign.m_strData;

	return *this;
}

void CNMStr::Serialize(CArchive &ar)
{
	CObject::Serialize(ar);

	if(ar.IsStoring()){
		ar<<m_nIDSize<<m_strID<<m_nDataSize<<m_strData;
	}
	else{
		ar>>m_nIDSize>>m_strID>>m_nDataSize>>m_strData;
	}
}

BOOL CNMStr::Compare(CString strData)
{
	if(strData==_T("")) return FALSE;
	if(m_strData==_T("") || m_nDataSize<=0) return FALSE;

	if(m_strData.CompareNoCase(strData.operator LPCTSTR())==0) return TRUE;

	return FALSE;
}

BOOL CNMStr::CompareID(CString strID)
{
	if(strID==_T("")) return FALSE;
	if(m_strID==_T("") || m_nIDSize<=0) return FALSE;

	if(m_strID.CompareNoCase(strID.operator LPCTSTR())==0) return TRUE;

	return FALSE;
}

void CNMStr::SetID(CString strID)
{
	m_strID   = strID;
	m_nIDSize = strID.GetLength();
}

void CNMStr::SetData(CString strData)
{
	m_strData   = strData;
	m_nDataSize = strData.GetLength();
}

void CNMStr::SetStr(CString strID, CString strData)
{
	Clear();

	SetID(strID);
	SetData(strData);
}

CString CNMStr::Get()
{
	return m_strData;
}

CString CNMStr::GetID()
{
	return m_strID;
}

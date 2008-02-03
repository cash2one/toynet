#if !defined(AFX_NMSTRMAN_H__591F95D9_101C_4CE5_AC62_4ECF78EA62DF__INCLUDED_)
#define AFX_NMSTRMAN_H__591F95D9_101C_4CE5_AC62_4ECF78EA62DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NMStrMan.h : header file
//

#include <afxcoll.h>
#include "NMStr.h"

#define  STRDATA_HEADER ("NETMARBLE PROGRAM (www.netmarble.net)")

/////////////////////////////////////////////////////////////////////////////
// CNMStrMan command target

class CNMStrMan : public CObject
{
protected:
	DECLARE_SERIAL(CNMStrMan)

public:
	BOOL Switch(CString strID1,CString strID2);
	BOOL Change(CString strID, CString strData);
	BOOL ChangeID(CString strID, CString strNew);
	BOOL Add(CString strID, CString strData);
	BOOL Add(CNMStr *pData);

	CNMStr * GetObj(CString strID);
	CString  Get(CString strID);
	char *  _Get(CString strID);

	void Clear(CString strID);
	void Clear();

	BOOL Compare(CString strData);
	BOOL CompareID(CString strData);

	BOOL Save(CString strFileName);
	BOOL Open(CString strFileName);

	CObList  m_LIST;
	CString  m_strHeader;
    int  	 m_nTotalNum;

	CNMStrMan();
	virtual ~CNMStrMan();
	virtual void Serialize(CArchive &ar);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NMSTRMAN_H__591F95D9_101C_4CE5_AC62_4ECF78EA62DF__INCLUDED_)

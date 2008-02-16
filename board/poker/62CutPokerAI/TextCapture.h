// TextCapture.h: interface for the CTextCapture class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXTCAPTURE_H__75A7DB21_752D_11D4_97A4_0050BF0FBE67__INCLUDED_)
#define AFX_TEXTCAPTURE_H__75A7DB21_752D_11D4_97A4_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct
{
	char strNotice[400];
	char strHeaderKey[8];
	char strUserID[16];
	int year, month, day;
	int hour, minute, second;
} TEXTCAPTURE_FILEHEADER;

class CTextCapture  
{
public:
	CTextCapture();
	virtual ~CTextCapture();

	BOOL Save(const char *strUserID, const char *strBuf, int length, const char *fname);
	CString Load(const char *fname);

};

#endif // !defined(AFX_TEXTCAPTURE_H__75A7DB21_752D_11D4_97A4_0050BF0FBE67__INCLUDED_)

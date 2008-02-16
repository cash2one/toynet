// MyBitmap.h: interface for the CMyBitmap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYBITMAP_H__0A94EE65_7FF3_11D4_97A4_0050BF0FBE67__INCLUDED_)
#define AFX_MYBITMAP_H__0A94EE65_7FF3_11D4_97A4_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyBitmap : public CBitmap
{

public:

	CMyBitmap();
	virtual ~CMyBitmap();
	BOOL LoadBitmapFile(LPCTSTR szFilename);
	BOOL TransDraw(HDC hdcDest, int nXDest, int nYDest, int nWidth, int nHeight, int nXSrc, int nYSrc, COLORREF colorTransparent);

	void ResizeWnd( CWnd* pWnd, int offset_x, int offset_y, BOOL bCenter, CWnd* pParaent );

	int GetWidth(){ return m_width; }
	int GetHeight(){ return m_height; }

protected:

	int m_width;
	int m_height;
};

#endif // !defined(AFX_MYBITMAP_H__0A94EE65_7FF3_11D4_97A4_0050BF0FBE67__INCLUDED_)

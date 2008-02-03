// CardMan.h: interface for the CCardMan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CARDMAN_H__2EAE02B7_C644_4013_AB20_BF5D386E2011__INCLUDED_)
#define AFX_CARDMAN_H__2EAE02B7_C644_4013_AB20_BF5D386E2011__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "CKBMPLIST.h"

#define CARD_W (71)
#define CARD_H (96)

class CCardMan  
{
public:
	void Draw(int nX,int nY,CDC *pDC,int nCard);
	void DrawDeck(int nX,int nY,CDC *pDC,int nCard,int nStyle=2);
	
	BOOL Init();
	void Clear();

	int m_nTotalCard;           // 카드데크 수
	int m_nBCardW,m_nBCardH,m_nSCardW,m_nSCardH; // 카드데크 가로세로사이즈

//	CKBMPLIST  m_Deck; // 카드데크
//	CKBMPLIST  m_Card; // 카드

	CCardMan();
	virtual ~CCardMan();

};

#endif // !defined(AFX_CARDMAN_H__2EAE02B7_C644_4013_AB20_BF5D386E2011__INCLUDED_)

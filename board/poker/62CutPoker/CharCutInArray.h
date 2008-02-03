// CharCutInArray.h: interface for the CCharCutInArray class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARCUTINARRAY_H__AF3D80E0_D48D_4152_BD7E_D82B0FBBE5C1__INCLUDED_)
#define AFX_CHARCUTINARRAY_H__AF3D80E0_D48D_4152_BD7E_D82B0FBBE5C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_PUNKA_LINE		6

class CCharCutInArray  
{
private:	//	Variable
	int				m_PunkaClass;
	int				m_PunkaChatLineCount;

public:		//	Variable
	char			m_szChat[ MAX_PUNKA_LINE ][ 100 ];


private:	//	Function
	//

	void			Release();


	void			GetShortString( char *pSource, char *pTarget, int CutSize );

public:	//	Function
	// return value 1 = 영어, 1 = 숫자, 1 - 특수문자, 2 = 한글. 3 = 한자...
	//
	int				GetCharType( BYTE ch );

	//	중간에 있는 스트링 뽑아 오기
	CString			GetStrMid( CString &strMsg, int CaretPos );

	//	
	bool			IsThisCutChat( char *pChar, int width );

	//	풍선 종류
	int				GetPunkaNumber( void );
	//	풍선에 들어갈 글자 라인수
	int				GetPunkaChatLineCount( void );

	//	게임룸에서의 채팅 처리
	void			RoomChatCutChar( char *pChatMsg );

	//	게임룸에서 문제 힌트창
	void			ChatCutChar( char *pChatMsg, int CutSize );

	//	일반 메시지 자르기
	void			MsgCutChar( char *pMsg, char *pTarget, int CutSize );

	CCharCutInArray();
	virtual ~CCharCutInArray();
};

#endif // !defined(AFX_CHARCUTINARRAY_H__AF3D80E0_D48D_4152_BD7E_D82B0FBBE5C1__INCLUDED_)

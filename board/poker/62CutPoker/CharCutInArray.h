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
	// return value 1 = ����, 1 = ����, 1 - Ư������, 2 = �ѱ�. 3 = ����...
	//
	int				GetCharType( BYTE ch );

	//	�߰��� �ִ� ��Ʈ�� �̾� ����
	CString			GetStrMid( CString &strMsg, int CaretPos );

	//	
	bool			IsThisCutChat( char *pChar, int width );

	//	ǳ�� ����
	int				GetPunkaNumber( void );
	//	ǳ���� �� ���� ���μ�
	int				GetPunkaChatLineCount( void );

	//	���ӷ뿡���� ä�� ó��
	void			RoomChatCutChar( char *pChatMsg );

	//	���ӷ뿡�� ���� ��Ʈâ
	void			ChatCutChar( char *pChatMsg, int CutSize );

	//	�Ϲ� �޽��� �ڸ���
	void			MsgCutChar( char *pMsg, char *pTarget, int CutSize );

	CCharCutInArray();
	virtual ~CCharCutInArray();
};

#endif // !defined(AFX_CHARCUTINARRAY_H__AF3D80E0_D48D_4152_BD7E_D82B0FBBE5C1__INCLUDED_)

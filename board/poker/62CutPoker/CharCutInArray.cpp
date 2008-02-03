// CharCutInArray.cpp: implementation of the CCharCutInArray class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CharCutInArray.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCharCutInArray::CCharCutInArray()
{
	m_PunkaClass = 0;
	m_PunkaChatLineCount = 0;

	memset( m_szChat, 0x00, MAX_PUNKA_LINE * 100 );
}

CCharCutInArray::~CCharCutInArray()
{
	Release();
}

void CCharCutInArray::Release()
{
	return;
}

//	풍선 종류
int CCharCutInArray::GetPunkaNumber( void )
{
	return m_PunkaClass;
}

//	풍선에 들어갈 글자 라인수
int	CCharCutInArray::GetPunkaChatLineCount( void )
{
	return m_PunkaChatLineCount;
}

//	잘라야 할부분이 한글이 깨지는지 안깨지는지
bool CCharCutInArray::IsThisCutChat( char *pChar, int width )
{
	int ch_type = 0, i = 0, one_byte_count = 0;

	for( i = 0; i < width; )
	{
		ch_type = GetCharType( pChar[ i ] );
		if( ch_type == 1 )		// 영어...
		{
			i++;
			one_byte_count++;
		}
		else if( ch_type == 2 || ch_type == 3 )
		{
			//	ch_type == 2 한글,  ch_type == 3 한자
			i += 2;
		}
		else
		{
			i += 2;
		}
	}

	if( one_byte_count % 2 == 0 )		return true;	//	잘라도 된다
	else
	{
		if( GetCharType( pChar[ width - 2 ] ) == 1 )	//	영어이거나 아스키코드
		{
			if( GetCharType( pChar[ width - 1 ] ) == 1 )		return true;	//	잘라도 된다.
			else	return false;
		}
		else
		{
			if( GetCharType( pChar[ width - 1 ] ) == 1 )		return true;	//	잘라도 된다.
			else	return false;	//	자르면 깨진다.
		}
	}
}
// ==========================================================================

//
// return value 1 = 영어, 1 = 숫자, 1 - 특수문자, 2 = 한글. 3 = 한자...
//
int CCharCutInArray::GetCharType( BYTE ch )
{
	if( isascii( ( int )ch ) != 0 )
	{
		return 1;
	}
	else if( ch >= 0xb0 && ch < 0xc9 )
	{
		return 2;			// 한글
	}
	else if( ch >= 0xca && ch < 0xfe )
	{
		return 3;			// 한자
	}

	return -1;
}

//	게임룸에서의 채팅 처리
void CCharCutInArray::RoomChatCutChar( char *pChatMsg )
{
	memset( m_szChat, 0x00, MAX_PUNKA_LINE * 100 );

	char *pChat = pChatMsg;

	m_PunkaClass = 0;
	m_PunkaChatLineCount = 0;

	int	sdWidth[ 4 ] = { 6, 8, 10, 14 };		//	글자수
	int	sdCount[ 4 ] = { 1, 2, 4, 4 };
	bool bExit = true;

	int i = 0, count = 0;

	while( bExit )
	{
		pChat = pChatMsg;

		for( int g = 0; g < sdCount[ i ]; g++ )
		{
			if( ( int )strlen( pChat ) <= sdWidth[ i ] )
			{
				strcpy( m_szChat[ g ], pChat );

				pChat += strlen( pChat );
			}
			else
			{
				if( IsThisCutChat( pChat, sdWidth[ i ] ) )
				{	//	잘라도 된다
					memcpy( m_szChat[ g ], pChat, sdWidth[ i ] );
					pChat += sdWidth[ i ];
				}
				else
				{	// 자르면 깨질때
					memcpy( m_szChat[ g ], pChat, sdWidth[ i ] - 1 );
					pChat += sdWidth[ i ] - 1;
				}
			}

			m_PunkaChatLineCount = g + 1;

			if( strlen( pChat ) == 0 )
			{
				m_PunkaClass = i;
				bExit = false;
				break;
			}
		}

		i++;
	}

	return;
}


void CCharCutInArray::GetShortString( char *pSource, char *pTarget, int CutSize )
{
	int len = strlen( pSource );
	int index = 0;

	if( len > MAX_PUNKA_LINE * CutSize )		len = MAX_PUNKA_LINE * CutSize;

	for( int i = 0; i < len; i++ )
	{
		if( pSource[ i ] != '\r' && pSource[ i ] != '\n' )
		{
			pTarget[ index ] = pSource[ i ];
			index++;

			if( index >= MAX_PUNKA_LINE * CutSize - 1 )		return;
		}
	}

	return;
}

//	게임룸에서 문제 힌트창
void CCharCutInArray::ChatCutChar( char *pChatMsg, int CutSize )
{
	int i = 0;

	char szTemp[ 301 ];
	memset( szTemp, NULL, 301 );

	GetShortString( pChatMsg, szTemp, CutSize );

	char *pChat = szTemp;
	int len = strlen( pChat );

	m_PunkaChatLineCount = ( len + CutSize - 1 ) / CutSize;
	if( m_PunkaChatLineCount > MAX_PUNKA_LINE )		m_PunkaChatLineCount = MAX_PUNKA_LINE;

	memset( m_szChat, 0x00, MAX_PUNKA_LINE * 100 );

	for( i = 0; i < m_PunkaChatLineCount; i++ )
	{
		for( int j = 0; j < CutSize; j++ )
		{
			if( pChat[ j ] == ' ' )
			{
				pChat++;
				j = 0;
			}
			else		break;
		}

		if( ( int )strlen( pChat ) <= CutSize )
		{
			strcpy( m_szChat[ i ], pChat );
			m_PunkaChatLineCount = i + 1;
			break;
		}
		else
		{
			if( IsThisCutChat( pChat, CutSize ) )
			{	//	잘라도 된다
				memcpy( m_szChat[ i ], pChat, CutSize );
				pChat += CutSize;
			}
			else
			{	// 자르면 깨질때
				memcpy( m_szChat[ i ], pChat, CutSize - 1 );
				pChat += CutSize - 1;
			}
		}
	}

	return;
}

//	일반 메시지 자르기
void CCharCutInArray::MsgCutChar( char *pMsg, char *pTarget, int CutSize )
{
	if( pMsg == NULL || strlen( pMsg ) <= 0 )		return;

	int len = strlen( pMsg );
	char *pChat = pMsg;

	strcpy( pTarget, "" );

	if( ( int )strlen( pChat ) <= CutSize )
	{
		strcpy( pTarget, pChat );
	}
	else
	{
		if( IsThisCutChat( pChat, CutSize ) )
		{	//	잘라도 된다
			memcpy( pTarget, pChat, CutSize );
			pChat += CutSize;
		}
		else
		{	// 자르면 깨질때
			memcpy( pTarget, pChat, CutSize - 1 );
			pChat += CutSize - 1;
		}
	}

	return;
}

//	중간에 있는 스트링 뽑아 오기
CString CCharCutInArray::GetStrMid( CString &strMsg, int CaretPos )
{
	int index = strMsg.GetLength() - CaretPos;
	if( index % 2 != 0 )		index++;
	char *pChat = ( char * )strMsg.operator LPCTSTR();
	if( !IsThisCutChat( pChat, index ) )		index++;
	return strMsg.Mid( index );
}

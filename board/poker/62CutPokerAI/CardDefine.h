
// CardDefine.h

#ifndef CARDDEFINE_H_
#define CARDDEFINE_H_

#include <windows.h>

enum EVENT_ENUM
{
	// [ 세븐 포커 ]
	EVENT_THREEFLYEND = 1,		   // 첫 3장을 모두 날렸다
	EVENT_ONEFLYEND,			   // 한장씩
	EVENT_ONEFLYTOTEND,			   // 한장씩 모두
	EVENT_CHANGEDEL_ONEFLYEND,		   // 62cut
	EVENT_CHANGESET_ONEFLYEND,




	EVENT_IAMREADY,				// 게임을 시작할 준비가 되었음	
								// (이 이벤트는 현재 카드가 플레이어3에게 돌아가는 카드임을 나타내는 이벤트를 내포함.)
	EVENT_DUMMYTOFLOOR,			// 카드 더미에서 한장을 가져와 바닥에 깐다
	EVENT_FLOORTOGETCARD,		// 먹은 카드를 가져옴
	EVENT_DUMMYTOMYCARD,		// 카드 더미에서 한장을 가져와 플레이어에게 넘긴다
	EVENT_ENDMYTURN,			// 내차례가 끝남
	EVENT_NOTIFYSTRING,			// 알림 메시지가 필요함..
	EVENT_DISTRIBUTETOPLAYER0,	// 
	EVENT_DISTRIBUTETOPLAYER1,	// 
	EVENT_DISTRIBUTETOPLAYER2,	// 
	EVENT_DISTRIBUTETOPLAYER3,	// 
	EVENT_DISTRIBUTETOBOARD,	// 바닥에 까는거..
	EVENT_SORTBOARDCARD,		// 바닥에 깔린 패를 정렬하기..
	EVENT_SORTMYHANDCARD,		// 내가 가진 패를 정렬하기..
	EVENT_JUMP,					// 목적지에 가서 점프~
	EVENT_SORTPEA,
//	EVENT_GETPART,			// 가서 같은 패를 가져오기..
	EVENT_ROTATECARD,			// 카드패 회전하기( Rotate.. )
	EVENT_EXECUTERESERVE,
	EVENT_DISTRIBUTETOBOARD_AND_RESERVEMYGETCARD, // 바닥에 낸 후에 내가 먹은 패로 올 것을 예약한다.
	
	EVENT_TURN,					// 패 뒤집기..
	EVENT_TURNANDGOTOMYCARD,	// 뒤집은 후에 나의 패 위치로 가기..
	EVENT_TURNANDGOTOMYHAND,	// 뒤집은 후에 내 손으로 ...
	EVENT_DUMMYTURN_GO_AND_GETPART,	// 뒤집은 후에 바닥에 깔아서 같은 패를 가져온다.
									// ( 깔린패가 2개면 물어보고 3개 이상이면 설사를 한 경우이므로 가져온다.)
	EVENT_DUMMYTURN_GO_AND_SULSA,	// 설사..
	EVENT_DUMMYTURN_GO_AND_JJOK,	// 쪽
	EVENT_DUMMYTURN_GO_AND_DDADAK	// 따닥..
};

enum RESERVE_ENUM
{
	RESERVE_GOTOGETCARD = 1,
	RESERVE_TAKEPEA,
	RESERVE_GOTOGETCARD_AND_TAKEPEA
};

enum ATTR_ENUM
{
	ATTR_NONE,					// 아무것도 아님.
	ATTR_NORMALPEA,				// 그냥 피
	ATTR_NORMALFIVEEND,			// 다섯 끗
	ATTR_NORMALTENEND,			// 열 끗
	ATTR_DOUBLEPEA,				// 쌍피
	ATTR_SERVICEDOUBLEPEA,		// 서비스 쌍피
	ATTR_SERVICETHREEPEA,		// 서비스 3피
	ATTR_RED,					// 홍단
	ATTR_BLUE,					// 청단
	ATTR_GRASS,					// 초단
	ATTR_BIRD,					// 고도리
	ATTR_MUMEND,				// 국열끗
	ATTR_RAINLIGHT,				// 비광
	ATTR_NORMALLIGHT			// 광
};

typedef struct
{
	int CardNo;			// 카드 번호
	int EventCode;		// 이벤트 코드
	int Option;			// 이벤트 옵션
	int Option2;		// 옵션 2
	int Option3;
} CARDEVENT;

typedef struct
{
	int Code;
	int Option;			// 이벤트 옵션
	int Option2;		// 옵션 2
	int Option3;
}RESERVATION;

typedef struct
{
	int Month;	// 어떠한 종류의 패인가( 1 ~ 12.. 비광 씨리즈, 월광 씨리즈 등등.. 단 서비스패는 13으로 한다. )
	int Part;	// 광(0)인가, 열끗(1)인가, 다섯끗(2)인가, 피(3)인가..( 
	int attr;	// 무엇인지 모르는 상태( 패의 속성이 알려지지 않은 경우. 즉 카드가 뒷면이 보이는 상태. 0 );
				// 그냥 피 1
				// 그냥 다섯 끗 2	
				// 그냥 열끗 3
				// 쌍피일 경우 4
				// 서비스패( 쌍피 ) 5
				// 서비스패( 3피 ) 6
				// 홍단		7
				// 청단		8 
				// 초단		9	
				// 고도리  10
				// 국열 끗 11
				// 비광	   12
				// 그냥 광 13
				// 폭탄    14
}CARD_INFO;


#endif
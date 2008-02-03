#include "StdAfx.h"
#include "UserStruct.h"

/*
int lvgap[15]=
{
	0,     330,   660,   1320,  2310,
	3630,  5280,  7620,  9570,  12210,
	15180, 18480, 22110, 26070, 30360
};

int GetLevel(int point, int winnum, int loosenum)
{
	int level = 0;
	
	// 승률 계산
	int totnum = winnum + loosenum;
	float winpro;
	if(totnum == 0) winpro = 0;
	else winpro = ((float)winnum / totnum)*100;

	int levelpoint = (int)(point * winpro/100);
	if(levelpoint < 0) levelpoint = 0;

	int totpnt=0;
	for(int i=0; i<15; i++)
	{
		totpnt+=lvgap[i];
		if(levelpoint > totpnt) level = i;
		else break;
	}

	return level;
}
*/



//INT64 Level[15] = {  50000000,	 50000100,	 50000500,	 50000800,	50001000, 
//					50001100,	50001200,	 50001300,	 50001400,	50001500, 
//					50001600,   50001700,	 50001800,	 50001900, };
/*
// 하이로우
INT64 Level[15] = {   200000000, // 2억 
					  500000000, // 5억	 
					 1000000000, // 10억
					 5000000000, // 50억
					15000000000, // 150억
					30000000000, // 300억
					50000000000, // 500억
				   100000000000, // 1000억
				   300000000000, // 3000억
				   500000000000, // 5000억 
				   700000000000, // 7000억
				  1000000000000, // 1조원
				 10000000000000, // 10조원
				100000000000000, // 100조원
};
*/

// 세븐포커2
/*
INT64 PokerLevel[15] = {    50000000, // 5천 0
					  100000000, // 1억	 1
					  200000000, // 2억   2    흑스페이드
					 1000000000, // 10억  3
					 5000000000, // 50억  4
					10000000000, // 100억 5
					20000000000, // 200억 6	  은스페이드
					50000000000, // 500억 7	   	
				   100000000000, // 1000억 8
				   200000000000, // 2000억 9
				   500000000000, // 5000억	10  금스페이드
				  1000000000000, // 1조원 11
				 10000000000000, // 10조원 12
				100000000000000, // 100조원 13
};
*/

INT64 PokerLevel[15] = {     50000000, // 5천    0
						    100000000, // 1억	 1
						   1000000000, // 10억   2    흑스페이드
						   2000000000, // 20억   3
						   5000000000, // 50억   4
						  10000000000, // 100억  5					
						  50000000000, // 500억  6	   	
						 100000000000, // 1000억 7
						 200000000000, // 2000억 8
						 500000000000, // 5000억 9  금스페이드
						1000000000000, // 1조원  10
						2000000000000, // 2조원  11
					   10000000000000, // 10조원 12
					  100000000000000, // 100조원13
						

};


int GetPokerLevel(INT64 Money )
{

	for( int i = 0; i < 14; i ++ )
	{
		if( Money < PokerLevel[i] ) break;
	}

	return i;
}


INT64 GetLevelToUserMoney(int lv)
{
	if(lv<0 || lv >= 15 ) return 0;
	if(lv == 14) return 1000000000000000;
	return PokerLevel[lv];
}

int GetCreateRoomGrade(int gr) 
{
	const static int MinGrade[6] = {0, 1, 3, 5, 7, 9};
	if(gr<0 || gr > 6 ) return 0;
	return MinGrade[gr];
}


int GetLevelPoint(int point, int winnum, int loosenum)
{
	// 승률 계산
	int totnum = winnum + loosenum;
	float winpro;
	if(totnum == 0) winpro = 0;
	else winpro = ((float)winnum / totnum)*100;

	int levelpoint = (int)(point * winpro/100);
	if(levelpoint < 0) levelpoint = 0;

	return levelpoint;
}


int GetWinPro(int winnum, int loosenum)
{
	// 승률 계산
	int totnum = winnum + loosenum;
	int winpro;
	if(totnum == 0) winpro = 0;
	else winpro = (int)(((double)winnum / totnum)*100);

	return winpro;
}



// 나의 등급 구하기 [점프 아이템 작업]
int GetMyLeveltoGrade(int lv)
{
	int grade = 0;

	if( lv > 11 ) grade = 4;
	else if( lv > 7  ) grade = 3;
	else if( lv > 3  ) grade = 2;
	else grade = 1;

	return grade;
}


/*

int GetWinPro(int nVic, int nFail, int nFold)
{
	int nTotal   = nVic + nFail + nFold;
	float fFold  = (float)nFold;
	float fTotal = (float)(nVic + nFail) + fFold/2;
	int  nWinPercent = 0;
	if(fTotal>0)
		nWinPercent = (int)(((float)nVic/fTotal)*100);

	return nWinPercent;

}
*/


//////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////                               [게임 아이템 작업]                               //////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

ItemState GameItem_GetItemState(GAMEITEM &item, SYSTEMTIME* pCurTime) 
{
	ItemState state;
	memset(&state, 0, sizeof(ItemState));

	// 올바른 데이타가 아니다
	if(item.UseDay < 0 || item.UseNum < 0) {
		state.iState = 0;	// 잘못된 아이템
		return state;
	}

	// 사용 회수로 아이템 사용 가능 여부를 판단
	if(item.UseNum > 0) {
		// 사용한 회수가 사용 가능한 회수를 초과했다면 다 쓴 아이템이다
		if(item.Use >= item.UseNum) {
			state.iState = 3;	// 사용 불가능한 아이템
		}
	}

	// 사용 기간으로 아이템 사용 가능 여부를 판단
	if(item.UseDay > 0) {

		if(item.Use > 0) { // 사용되어진 아이템이라면 시작한 날짜가 있다
			
			// 아이템의 유효 기간을 분단위로 얻기
			int period = item.UseDay * 24 * 60;
			
			// ### [ 올인극복 아이템 ] ###
			if(item.Code >= ITEM_SCODE_ALLINSUPERMASTER && item.Code <= ITEM_ECODE_ALLINSUPERMASTER)
				period = item.UseDay * 60;

			// 현재 시간 얻기
			CTime nt;
			if(pCurTime) nt = *pCurTime;		// 임의로 지정한 현재시간(클라이언트용)
			else nt = CTime::GetCurrentTime();	// 현재의 실제 시간

			// 아이템을 사용한 시작시간
			CTime st(item.StartDate); // 스타트시간

			CTimeSpan TimeSpan = nt - st;			// 현재시간과의 차이를 구한다
			int passed = TimeSpan.GetTotalMinutes();

			if(period > passed) {
				
				// 남은 시간 계산
				int leftmin = (int)(period - passed);
				CTimeSpan sp(0, 0, leftmin, 0);

				state.LeftTime = sp;
				state.EndDate = nt + sp;

				// 사용중인 아이템이다
				state.bNowUsing = TRUE;

				state.iState = 2;	// 사용중인 아이템
			}
			else {
				// 다쓴 아이템이다
				state.bNowUsing = FALSE;

				state.iState = 3;	// 사용 불가능한 아이템
			}
		}
		else {
			state.iState = 1;	// 사용 가능한 아이템
		}
	}

	return state;
}
/*
// 특정 종류의 아이템이 사용중인지 알아냄(서버/클라이언트 공용)
BOOL GameItem_IsUsingItem(GAMEITEM_LIST &itemlist, int itemcode, BOOL *pExist, SYSTEMTIME* pCurTime)
{
	int kind = 0;

	if(itemcode >= ITEM_SCODE_SECRET      && itemcode <= ITEM_ECODE_SECRET) 
		kind          = ITEM_SCODE_SECRET;
	else if(itemcode >= ITEM_SCODE_JUMP && itemcode <= ITEM_ECODE_JUMP) 
		kind = ITEM_SCODE_JUMP;
	else if(itemcode >= ITEM_SCODE_SUPERMASTER && itemcode <= ITEM_ECODE_SUPERMASTER) 
		kind = ITEM_SCODE_SUPERMASTER;
	else if(itemcode >= ITEM_SCODE_PLUSPOINT && itemcode <= ITEM_ECODE_PLUSPOINT) 
		kind = ITEM_SCODE_PLUSPOINT;
	// ### [ 올인극복 아이템 ] ###
	else if(itemcode >= ITEM_SCODE_ALLINSUPERMASTER && itemcode <= ITEM_ECODE_ALLINSUPERMASTER) 
		kind = ITEM_SCODE_ALLINSUPERMASTER;
	else if(itemcode == ITEM_SCODE_INVINCIBLE) // 천하무적 아이템 
		kind = ITEM_SCODE_INVINCIBLE;
	else 
		return FALSE;

	for(int i=0; i<itemlist.nNum; i++) {

		if( (kind == ITEM_SCODE_SECRET      && itemlist.Item[i].Code >= ITEM_SCODE_SECRET      && itemlist.Item[i].Code <= ITEM_ECODE_SECRET)      ||
			(kind == ITEM_SCODE_JUMP && itemlist.Item[i].Code >= ITEM_SCODE_JUMP && itemlist.Item[i].Code <= ITEM_ECODE_JUMP) || 
			(kind == ITEM_SCODE_SUPERMASTER && itemlist.Item[i].Code >= ITEM_SCODE_SUPERMASTER && itemlist.Item[i].Code <= ITEM_ECODE_SUPERMASTER) || 
			(kind == ITEM_SCODE_PLUSPOINT && itemlist.Item[i].Code >= ITEM_SCODE_PLUSPOINT && itemlist.Item[i].Code <= ITEM_ECODE_PLUSPOINT) ||
			// ### [ 올인극복 아이템 ] ###
			(kind == ITEM_SCODE_ALLINSUPERMASTER && itemlist.Item[i].Code >= ITEM_SCODE_ALLINSUPERMASTER && itemlist.Item[i].Code <= ITEM_ECODE_ALLINSUPERMASTER) ||
			(kind == ITEM_SCODE_INVINCIBLE && itemlist.Item[i].Code == ITEM_SCODE_INVINCIBLE) ) { // [천하무적 아이템] 
			
			// 아이템의 상태를 얻어옴(pCurTime이 NULL이라면 시스템의 현재 시간을 기준으로 판단)
			ItemState state = GameItem_GetItemState(itemlist.Item[i], pCurTime);

			// 사용 가능한 아이템이면 존재함을 표시
			if(state.iState == 1) 
				*pExist = TRUE;

			// 현재 사용중인가?
			if(state.bNowUsing) 
				return TRUE;
		}
	}

	return FALSE;
}
*/
// 특정 종류의 아이템이 사용중인지 알아냄(서버/클라이언트 공용)
BOOL GameItem_IsUsingItem(GAMEITEM_LIST &itemlist, int itemcode, BOOL *pExist, SYSTEMTIME* pCurTime)
{
	int kind = GameItem_GetItemKind(itemcode);
	if(kind == 0)
		return FALSE;

	BOOL bRtn = FALSE;

	for(int i=0; i<itemlist.nNum; i++) {
		// 요청한 아이템과 같은 종류의 아이템인지 검사
		if( kind == GameItem_GetItemKind(itemlist.Item[i].Code) ) {		
			// 아이템의 상태를 얻어옴(pCurTime이 NULL이라면 시스템의 현재 시간을 기준으로 판단)
			ItemState state = GameItem_GetItemState(itemlist.Item[i], pCurTime);

			// 사용 가능한 아이템이면 존재함을 표시
			if(state.iState == 1) 
				*pExist = TRUE;

			// 현재 사용중인가?
			if(state.bNowUsing) 
				bRtn = TRUE;
		}
	}

	return bRtn;
}

/*
CString GameItem_GetItemName(GAMEITEM& item)
{
	CString str;

	if(item.Code >= ITEM_SCODE_SECRET && item.Code <= ITEM_ECODE_SECRET) {
		str.Format("비공개방 아이템 (%d일)", item.UseDay);
	}
	else if(item.Code >= ITEM_SCODE_JUMP && item.Code <= ITEM_ECODE_JUMP) {
		str.Format("점프 입장권 아이템 (%d일)", item.UseDay);
	}
	else if(item.Code >= ITEM_SCODE_SUPERMASTER && item.Code <= ITEM_ECODE_SUPERMASTER) {
		str.Format("슈퍼 방장 아이템 (%d일)", item.UseDay);
	}
	else if(item.Code >= ITEM_SCODE_PLUSPOINT && item.Code <= ITEM_ECODE_PLUSPOINT) {
		str.Format("플러스 포인트 아이템 (%d일)", item.UseDay);
	}
	// ### [ 올인극복 아이템 ] ###
	else if(item.Code >= ITEM_SCODE_ALLINSUPERMASTER && item.Code <= ITEM_ECODE_ALLINSUPERMASTER)
	{
		str.Format("슈퍼한방 아이템 (슈퍼방 %d시간)", item.UseDay);
	}
	else if(item.Code == ITEM_SCODE_INVINCIBLE ) { // [천하무적 아이템]
		str.Format("천하무적 아이템 (%d일)", item.UseDay);
	}
	else {
		str = "알 수 없는 아이템";
	}

	return str;
}
*/

CString GameItem_GetItemName(GAMEITEM& item)
{
	CString str;
	str = GameItem_GetItemName(item.Code);
	if(item.UseDay > 0) {
		CString strday;
		strday.Format(" (%d일)", item.UseDay);
		str += strday;
	}

	return str;
}


/*
int kind = 0;

	if(itemcode >= ITEM_SCODE_SECRET      && itemcode <= ITEM_ECODE_SECRET) 
		kind          = ITEM_SCODE_SECRET;
	else if(itemcode >= ITEM_SCODE_JUMP && itemcode <= ITEM_ECODE_JUMP) 
		kind = ITEM_SCODE_JUMP;
	else if(itemcode >= ITEM_SCODE_SUPERMASTER && itemcode <= ITEM_ECODE_SUPERMASTER) 
		kind = ITEM_SCODE_SUPERMASTER;
	else if(itemcode >= ITEM_SCODE_PLUSPOINT && itemcode <= ITEM_ECODE_PLUSPOINT) 
		kind = ITEM_SCODE_PLUSPOINT;
	// ### [ 올인극복 아이템 ] ###
	else if(itemcode >= ITEM_SCODE_ALLINSUPERMASTER && itemcode <= ITEM_ECODE_ALLINSUPERMASTER) 
		kind = ITEM_SCODE_ALLINSUPERMASTER;
	else if(itemcode == ITEM_SCODE_INVINCIBLE) // 천하무적 아이템 
		kind = ITEM_SCODE_INVINCIBLE;
	else 
		return FALSE;

*/
// 아이템의 종류를 알아내는 편의 함수(아이템의 시작코드를 종류 코드로 사용)
int GameItem_GetItemKind(int itemcode)
{
	int kind = 0;

	if(itemcode >= ITEM_SCODE_SECRET      && itemcode <= ITEM_ECODE_SECRET) 
		kind          = ITEM_SCODE_SECRET;
	else if(itemcode >= ITEM_SCODE_JUMP && itemcode <= ITEM_ECODE_JUMP) 
		kind = ITEM_SCODE_JUMP;
	else if(itemcode >= ITEM_SCODE_SUPERMASTER && itemcode <= ITEM_ECODE_SUPERMASTER) 
		kind = ITEM_SCODE_SUPERMASTER;
	else if(itemcode >= ITEM_SCODE_PLUSPOINT && itemcode <= ITEM_ECODE_PLUSPOINT) 
		kind = ITEM_SCODE_PLUSPOINT;
	else if(itemcode == ITEM_SCODE_INVINCIBLE) // 천하무적 아이템 
		kind = ITEM_SCODE_INVINCIBLE;
	else if(itemcode >= ITEM_SCODE_SAFEGUARDANGEL && itemcode <= ITEM_ECODE_SAFEGUARDANGEL) // [수호천사] 2004.07.08
		kind = ITEM_SCODE_SAFEGUARDANGEL;

	return kind;
}



CString GameItem_GetItemName(int code)
{
	CString str;

	if(code >= ITEM_SCODE_SECRET && code <= ITEM_ECODE_SECRET) {
		str.Format("비공개방 아이템");
	}
	else if(code >= ITEM_SCODE_JUMP && code <= ITEM_ECODE_JUMP) {
		str.Format("점프 입장권 아이템");
	}
	else if(code >= ITEM_SCODE_SUPERMASTER && code <= ITEM_ECODE_SUPERMASTER) {
		str.Format("슈퍼 방장 아이템");
	}
	else if(code >= ITEM_SCODE_PLUSPOINT && code <= ITEM_ECODE_PLUSPOINT) {
		str.Format("플러스 포인트 아이템");
	}
	// ### [ 올인극복 아이템 ] ###
	else if(code >= ITEM_SCODE_ALLINSUPERMASTER && code <= ITEM_ECODE_ALLINSUPERMASTER)
	{
		str.Format("슈퍼한방 아이템");
	}
	else if(code == ITEM_SCODE_INVINCIBLE) // [천하무적 아이템]
	{
		str.Format("천하무적 아이템");
	}
	else if(code >= ITEM_SCODE_SAFEGUARDANGEL && code <= ITEM_ECODE_SAFEGUARDANGEL) { // [수호천사] 2004.07.08
		str.Format("수호천사 아이템");
	}
	else {
		str = "알 수 없는 아이템";
	}

	return str;
}

CString GameItem_GetItemHelp(int code)
{
	CString str;

	if(code >= ITEM_SCODE_SECRET && code <= ITEM_ECODE_SECRET) {
		str.Format("뉴포커 일반 채널에서 비공개방을 만들 때 사용되는 아이템입니다.");
	}
	else if(code >= ITEM_SCODE_JUMP && code <= ITEM_ECODE_JUMP) {
		str.Format("입장 가능한 등급보다 한 단계 더 높은 등급의 서버나 채널에 입장 할 수 있도록 해주는 아이템입니다.");
	}
	// ### [ 올인극복 아이템 ] ###
	else if(code >= ITEM_SCODE_SUPERMASTER && code <= ITEM_ECODE_SUPERMASTER ||
		code >= ITEM_SCODE_ALLINSUPERMASTER && code <= ITEM_ECODE_ALLINSUPERMASTER ) {
		str.Format("막강한 방장 권한을 부여하여 유저를 강제퇴장 시킬 수 있도록 해주고 방 리스트를 제일 위로 올려주는 아이템입니다.");
	}
	else if(code >= ITEM_SCODE_PLUSPOINT && code <= ITEM_ECODE_PLUSPOINT) {
		str.Format("게임에서 승리하였을 경우 보통때 보다 더 많은 점수를 얻을 수 있도록 해주는 아이템입니다.");
	}
	else if(code == ITEM_SCODE_INVINCIBLE) { // [천하무적 아이템]
		str.Format("슈퍼방+찬스아이템+비공개방아이템을 사용할 수 있도록 해주는 아이템입니다.");
	}
	else if(code >= ITEM_SCODE_SAFEGUARDANGEL && code <= ITEM_ECODE_SAFEGUARDANGEL) { // [수호천사] 2004.07.08
		str.Format("마지막 판에서 잃은 금액을 돌려받는 아이템입니다.");
	}
	else {
		str = "";
	}

	return str;
}

CString GameItem_GetItemImageFile(int code)
{
	CString str;
	if(code >= ITEM_SCODE_SECRET && code <= ITEM_ECODE_SECRET)   str = "image\\gameitem\\item_R10.bmp";

	// [비공개방]
	if(code == ITEM_SCODE_SECRET+ 1) {
		str = "image\\gameitem\\Item_R01.bmp";
	}
	else if(code == ITEM_SCODE_SECRET+ 5) {
		str = "image\\gameitem\\item_R05.bmp";
	}
	else if(code == ITEM_SCODE_SECRET+10) {
		str = "image\\gameitem\\item_R10.bmp";
	} 
	else if(code >= ITEM_SCODE_JUMP && code <= ITEM_ECODE_JUMP) {
		str = "image\\gameitem\\item_jump.bmp";
	}
	else if(code >= ITEM_SCODE_SUPERMASTER && code <= ITEM_ECODE_SUPERMASTER) {
		str = "image\\gameitem\\item_supermaster.bmp";
	}
	else if(code >= ITEM_SCODE_PLUSPOINT && code <= ITEM_ECODE_PLUSPOINT) {
		str = "image\\gameitem\\item_pluspoint.bmp";
	}
	// ### [ 올인극복 아이템 ] ###
	else if(code >= ITEM_SCODE_ALLINSUPERMASTER && code <= ITEM_ECODE_ALLINSUPERMASTER)
	{
		str = "image\\gameitem\\item_supermaster_1h.bmp";
	}
	else if(code == ITEM_SCODE_INVINCIBLE) // [천하무적 아이템]
	{
		 str = "image\\gameitem\\item_invincible.bmp";
	}
	else if(code >= ITEM_SCODE_SAFEGUARDANGEL && code <= ITEM_ECODE_SAFEGUARDANGEL) { // [수호천사] 2004.07.08
		str = "image\\gameitem\\item_safeangel.bmp";
	}
	

	return str;
}

int GameItem_GetItemImageIndex(int code) 
{
	int image = 0;
	if(code >= ITEM_SCODE_JUMP        && code <= ITEM_ECODE_JUMP       ) image = 1;
	if(code >= ITEM_SCODE_SUPERMASTER && code <= ITEM_ECODE_SUPERMASTER) image = 2;
	if(code >= ITEM_SCODE_PLUSPOINT   && code <= ITEM_ECODE_PLUSPOINT  ) image = 3;
	// ### [ 올인극복 아이템 ] ###
	if(code >= ITEM_SCODE_ALLINSUPERMASTER   && code <= ITEM_ECODE_ALLINSUPERMASTER ) image = 4;
	if(code >= ITEM_SCODE_SECRET      && code <= ITEM_ECODE_SECRET     ) image = 5; //비공개
	if(code == ITEM_SCODE_INVINCIBLE ) image = 6; // [천하무적 아이템]
	if(code >= ITEM_SCODE_SAFEGUARDANGEL && code <= ITEM_ECODE_SAFEGUARDANGEL) image = 7; // [수호천사] 2004.07.08
	return image;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////                               [프리미엄 작업]	                                //////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

int CheckPremiumState(USERINFO* pUI, SYSTEMTIME* pCurTime)
{
	if(!pUI) return 0;

	if(pUI->PremCode > 0) {
		// 현재 시간 얻기
		CTime nt;
		if(pCurTime) nt = *pCurTime;		// 임의로 지정한 현재시간(클라이언트용)
		else nt = CTime::GetCurrentTime();	// 현재의 실제 시간

		// 프리미엄 만료 일시
		CTime et(pUI->PremDate); // 스타트시간

		// 남은 프리미엄 기간을 구한다
		CTimeSpan TimeSpan = et - nt;
		int period = TimeSpan.GetTotalMinutes();

		if(period > 0) {
			// 프리미엄 상태 코드를 얻는다
			int stcode = pUI->PremCode % 10;

			// 프리미엄 코드의 범위에 따라서 다음과 같은 서비스로 구분됨
			//    0    : 서비스 미사용
			// 11 - 19 : 리더스 클럽
			// 21 - 29 : 쥬니어 클럽
			// 31 - 39 : 추가될 서비스를 위하여 예약됨

			
			// 프리미엄 코드는 10으로 나눈 나머지에 따라서 다음과 같은 의미를 가짐
			// 1 정상 이용자
			// 2 해지 신청자
			// 3 일시정지(결제 미승인)
			// 4 개인 일시정지
			// 5 해지자
			// 6 영구해지자

			if(stcode == 1 || stcode==2) {		
				return 1;
			}
		}
	}

	return 0;
}


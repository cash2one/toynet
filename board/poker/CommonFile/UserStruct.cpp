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
	
	// �·� ���
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
// ���̷ο�
INT64 Level[15] = {   200000000, // 2�� 
					  500000000, // 5��	 
					 1000000000, // 10��
					 5000000000, // 50��
					15000000000, // 150��
					30000000000, // 300��
					50000000000, // 500��
				   100000000000, // 1000��
				   300000000000, // 3000��
				   500000000000, // 5000�� 
				   700000000000, // 7000��
				  1000000000000, // 1����
				 10000000000000, // 10����
				100000000000000, // 100����
};
*/

// ������Ŀ2
/*
INT64 PokerLevel[15] = {    50000000, // 5õ 0
					  100000000, // 1��	 1
					  200000000, // 2��   2    �潺���̵�
					 1000000000, // 10��  3
					 5000000000, // 50��  4
					10000000000, // 100�� 5
					20000000000, // 200�� 6	  �������̵�
					50000000000, // 500�� 7	   	
				   100000000000, // 1000�� 8
				   200000000000, // 2000�� 9
				   500000000000, // 5000��	10  �ݽ����̵�
				  1000000000000, // 1���� 11
				 10000000000000, // 10���� 12
				100000000000000, // 100���� 13
};
*/

INT64 PokerLevel[15] = {     50000000, // 5õ    0
						    100000000, // 1��	 1
						   1000000000, // 10��   2    �潺���̵�
						   2000000000, // 20��   3
						   5000000000, // 50��   4
						  10000000000, // 100��  5					
						  50000000000, // 500��  6	   	
						 100000000000, // 1000�� 7
						 200000000000, // 2000�� 8
						 500000000000, // 5000�� 9  �ݽ����̵�
						1000000000000, // 1����  10
						2000000000000, // 2����  11
					   10000000000000, // 10���� 12
					  100000000000000, // 100����13
						

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
	// �·� ���
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
	// �·� ���
	int totnum = winnum + loosenum;
	int winpro;
	if(totnum == 0) winpro = 0;
	else winpro = (int)(((double)winnum / totnum)*100);

	return winpro;
}



// ���� ��� ���ϱ� [���� ������ �۾�]
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
////////////                               [���� ������ �۾�]                               //////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

ItemState GameItem_GetItemState(GAMEITEM &item, SYSTEMTIME* pCurTime) 
{
	ItemState state;
	memset(&state, 0, sizeof(ItemState));

	// �ùٸ� ����Ÿ�� �ƴϴ�
	if(item.UseDay < 0 || item.UseNum < 0) {
		state.iState = 0;	// �߸��� ������
		return state;
	}

	// ��� ȸ���� ������ ��� ���� ���θ� �Ǵ�
	if(item.UseNum > 0) {
		// ����� ȸ���� ��� ������ ȸ���� �ʰ��ߴٸ� �� �� �������̴�
		if(item.Use >= item.UseNum) {
			state.iState = 3;	// ��� �Ұ����� ������
		}
	}

	// ��� �Ⱓ���� ������ ��� ���� ���θ� �Ǵ�
	if(item.UseDay > 0) {

		if(item.Use > 0) { // ���Ǿ��� �������̶�� ������ ��¥�� �ִ�
			
			// �������� ��ȿ �Ⱓ�� �д����� ���
			int period = item.UseDay * 24 * 60;
			
			// ### [ ���αغ� ������ ] ###
			if(item.Code >= ITEM_SCODE_ALLINSUPERMASTER && item.Code <= ITEM_ECODE_ALLINSUPERMASTER)
				period = item.UseDay * 60;

			// ���� �ð� ���
			CTime nt;
			if(pCurTime) nt = *pCurTime;		// ���Ƿ� ������ ����ð�(Ŭ���̾�Ʈ��)
			else nt = CTime::GetCurrentTime();	// ������ ���� �ð�

			// �������� ����� ���۽ð�
			CTime st(item.StartDate); // ��ŸƮ�ð�

			CTimeSpan TimeSpan = nt - st;			// ����ð����� ���̸� ���Ѵ�
			int passed = TimeSpan.GetTotalMinutes();

			if(period > passed) {
				
				// ���� �ð� ���
				int leftmin = (int)(period - passed);
				CTimeSpan sp(0, 0, leftmin, 0);

				state.LeftTime = sp;
				state.EndDate = nt + sp;

				// ������� �������̴�
				state.bNowUsing = TRUE;

				state.iState = 2;	// ������� ������
			}
			else {
				// �پ� �������̴�
				state.bNowUsing = FALSE;

				state.iState = 3;	// ��� �Ұ����� ������
			}
		}
		else {
			state.iState = 1;	// ��� ������ ������
		}
	}

	return state;
}
/*
// Ư�� ������ �������� ��������� �˾Ƴ�(����/Ŭ���̾�Ʈ ����)
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
	// ### [ ���αغ� ������ ] ###
	else if(itemcode >= ITEM_SCODE_ALLINSUPERMASTER && itemcode <= ITEM_ECODE_ALLINSUPERMASTER) 
		kind = ITEM_SCODE_ALLINSUPERMASTER;
	else if(itemcode == ITEM_SCODE_INVINCIBLE) // õ�Ϲ��� ������ 
		kind = ITEM_SCODE_INVINCIBLE;
	else 
		return FALSE;

	for(int i=0; i<itemlist.nNum; i++) {

		if( (kind == ITEM_SCODE_SECRET      && itemlist.Item[i].Code >= ITEM_SCODE_SECRET      && itemlist.Item[i].Code <= ITEM_ECODE_SECRET)      ||
			(kind == ITEM_SCODE_JUMP && itemlist.Item[i].Code >= ITEM_SCODE_JUMP && itemlist.Item[i].Code <= ITEM_ECODE_JUMP) || 
			(kind == ITEM_SCODE_SUPERMASTER && itemlist.Item[i].Code >= ITEM_SCODE_SUPERMASTER && itemlist.Item[i].Code <= ITEM_ECODE_SUPERMASTER) || 
			(kind == ITEM_SCODE_PLUSPOINT && itemlist.Item[i].Code >= ITEM_SCODE_PLUSPOINT && itemlist.Item[i].Code <= ITEM_ECODE_PLUSPOINT) ||
			// ### [ ���αغ� ������ ] ###
			(kind == ITEM_SCODE_ALLINSUPERMASTER && itemlist.Item[i].Code >= ITEM_SCODE_ALLINSUPERMASTER && itemlist.Item[i].Code <= ITEM_ECODE_ALLINSUPERMASTER) ||
			(kind == ITEM_SCODE_INVINCIBLE && itemlist.Item[i].Code == ITEM_SCODE_INVINCIBLE) ) { // [õ�Ϲ��� ������] 
			
			// �������� ���¸� ����(pCurTime�� NULL�̶�� �ý����� ���� �ð��� �������� �Ǵ�)
			ItemState state = GameItem_GetItemState(itemlist.Item[i], pCurTime);

			// ��� ������ �������̸� �������� ǥ��
			if(state.iState == 1) 
				*pExist = TRUE;

			// ���� ������ΰ�?
			if(state.bNowUsing) 
				return TRUE;
		}
	}

	return FALSE;
}
*/
// Ư�� ������ �������� ��������� �˾Ƴ�(����/Ŭ���̾�Ʈ ����)
BOOL GameItem_IsUsingItem(GAMEITEM_LIST &itemlist, int itemcode, BOOL *pExist, SYSTEMTIME* pCurTime)
{
	int kind = GameItem_GetItemKind(itemcode);
	if(kind == 0)
		return FALSE;

	BOOL bRtn = FALSE;

	for(int i=0; i<itemlist.nNum; i++) {
		// ��û�� �����۰� ���� ������ ���������� �˻�
		if( kind == GameItem_GetItemKind(itemlist.Item[i].Code) ) {		
			// �������� ���¸� ����(pCurTime�� NULL�̶�� �ý����� ���� �ð��� �������� �Ǵ�)
			ItemState state = GameItem_GetItemState(itemlist.Item[i], pCurTime);

			// ��� ������ �������̸� �������� ǥ��
			if(state.iState == 1) 
				*pExist = TRUE;

			// ���� ������ΰ�?
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
		str.Format("������� ������ (%d��)", item.UseDay);
	}
	else if(item.Code >= ITEM_SCODE_JUMP && item.Code <= ITEM_ECODE_JUMP) {
		str.Format("���� ����� ������ (%d��)", item.UseDay);
	}
	else if(item.Code >= ITEM_SCODE_SUPERMASTER && item.Code <= ITEM_ECODE_SUPERMASTER) {
		str.Format("���� ���� ������ (%d��)", item.UseDay);
	}
	else if(item.Code >= ITEM_SCODE_PLUSPOINT && item.Code <= ITEM_ECODE_PLUSPOINT) {
		str.Format("�÷��� ����Ʈ ������ (%d��)", item.UseDay);
	}
	// ### [ ���αغ� ������ ] ###
	else if(item.Code >= ITEM_SCODE_ALLINSUPERMASTER && item.Code <= ITEM_ECODE_ALLINSUPERMASTER)
	{
		str.Format("�����ѹ� ������ (���۹� %d�ð�)", item.UseDay);
	}
	else if(item.Code == ITEM_SCODE_INVINCIBLE ) { // [õ�Ϲ��� ������]
		str.Format("õ�Ϲ��� ������ (%d��)", item.UseDay);
	}
	else {
		str = "�� �� ���� ������";
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
		strday.Format(" (%d��)", item.UseDay);
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
	// ### [ ���αغ� ������ ] ###
	else if(itemcode >= ITEM_SCODE_ALLINSUPERMASTER && itemcode <= ITEM_ECODE_ALLINSUPERMASTER) 
		kind = ITEM_SCODE_ALLINSUPERMASTER;
	else if(itemcode == ITEM_SCODE_INVINCIBLE) // õ�Ϲ��� ������ 
		kind = ITEM_SCODE_INVINCIBLE;
	else 
		return FALSE;

*/
// �������� ������ �˾Ƴ��� ���� �Լ�(�������� �����ڵ带 ���� �ڵ�� ���)
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
	else if(itemcode == ITEM_SCODE_INVINCIBLE) // õ�Ϲ��� ������ 
		kind = ITEM_SCODE_INVINCIBLE;
	else if(itemcode >= ITEM_SCODE_SAFEGUARDANGEL && itemcode <= ITEM_ECODE_SAFEGUARDANGEL) // [��ȣõ��] 2004.07.08
		kind = ITEM_SCODE_SAFEGUARDANGEL;

	return kind;
}



CString GameItem_GetItemName(int code)
{
	CString str;

	if(code >= ITEM_SCODE_SECRET && code <= ITEM_ECODE_SECRET) {
		str.Format("������� ������");
	}
	else if(code >= ITEM_SCODE_JUMP && code <= ITEM_ECODE_JUMP) {
		str.Format("���� ����� ������");
	}
	else if(code >= ITEM_SCODE_SUPERMASTER && code <= ITEM_ECODE_SUPERMASTER) {
		str.Format("���� ���� ������");
	}
	else if(code >= ITEM_SCODE_PLUSPOINT && code <= ITEM_ECODE_PLUSPOINT) {
		str.Format("�÷��� ����Ʈ ������");
	}
	// ### [ ���αغ� ������ ] ###
	else if(code >= ITEM_SCODE_ALLINSUPERMASTER && code <= ITEM_ECODE_ALLINSUPERMASTER)
	{
		str.Format("�����ѹ� ������");
	}
	else if(code == ITEM_SCODE_INVINCIBLE) // [õ�Ϲ��� ������]
	{
		str.Format("õ�Ϲ��� ������");
	}
	else if(code >= ITEM_SCODE_SAFEGUARDANGEL && code <= ITEM_ECODE_SAFEGUARDANGEL) { // [��ȣõ��] 2004.07.08
		str.Format("��ȣõ�� ������");
	}
	else {
		str = "�� �� ���� ������";
	}

	return str;
}

CString GameItem_GetItemHelp(int code)
{
	CString str;

	if(code >= ITEM_SCODE_SECRET && code <= ITEM_ECODE_SECRET) {
		str.Format("����Ŀ �Ϲ� ä�ο��� ��������� ���� �� ���Ǵ� �������Դϴ�.");
	}
	else if(code >= ITEM_SCODE_JUMP && code <= ITEM_ECODE_JUMP) {
		str.Format("���� ������ ��޺��� �� �ܰ� �� ���� ����� ������ ä�ο� ���� �� �� �ֵ��� ���ִ� �������Դϴ�.");
	}
	// ### [ ���αغ� ������ ] ###
	else if(code >= ITEM_SCODE_SUPERMASTER && code <= ITEM_ECODE_SUPERMASTER ||
		code >= ITEM_SCODE_ALLINSUPERMASTER && code <= ITEM_ECODE_ALLINSUPERMASTER ) {
		str.Format("������ ���� ������ �ο��Ͽ� ������ �������� ��ų �� �ֵ��� ���ְ� �� ����Ʈ�� ���� ���� �÷��ִ� �������Դϴ�.");
	}
	else if(code >= ITEM_SCODE_PLUSPOINT && code <= ITEM_ECODE_PLUSPOINT) {
		str.Format("���ӿ��� �¸��Ͽ��� ��� ���붧 ���� �� ���� ������ ���� �� �ֵ��� ���ִ� �������Դϴ�.");
	}
	else if(code == ITEM_SCODE_INVINCIBLE) { // [õ�Ϲ��� ������]
		str.Format("���۹�+����������+�������������� ����� �� �ֵ��� ���ִ� �������Դϴ�.");
	}
	else if(code >= ITEM_SCODE_SAFEGUARDANGEL && code <= ITEM_ECODE_SAFEGUARDANGEL) { // [��ȣõ��] 2004.07.08
		str.Format("������ �ǿ��� ���� �ݾ��� �����޴� �������Դϴ�.");
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

	// [�������]
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
	// ### [ ���αغ� ������ ] ###
	else if(code >= ITEM_SCODE_ALLINSUPERMASTER && code <= ITEM_ECODE_ALLINSUPERMASTER)
	{
		str = "image\\gameitem\\item_supermaster_1h.bmp";
	}
	else if(code == ITEM_SCODE_INVINCIBLE) // [õ�Ϲ��� ������]
	{
		 str = "image\\gameitem\\item_invincible.bmp";
	}
	else if(code >= ITEM_SCODE_SAFEGUARDANGEL && code <= ITEM_ECODE_SAFEGUARDANGEL) { // [��ȣõ��] 2004.07.08
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
	// ### [ ���αغ� ������ ] ###
	if(code >= ITEM_SCODE_ALLINSUPERMASTER   && code <= ITEM_ECODE_ALLINSUPERMASTER ) image = 4;
	if(code >= ITEM_SCODE_SECRET      && code <= ITEM_ECODE_SECRET     ) image = 5; //�����
	if(code == ITEM_SCODE_INVINCIBLE ) image = 6; // [õ�Ϲ��� ������]
	if(code >= ITEM_SCODE_SAFEGUARDANGEL && code <= ITEM_ECODE_SAFEGUARDANGEL) image = 7; // [��ȣõ��] 2004.07.08
	return image;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////                               [�����̾� �۾�]	                                //////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

int CheckPremiumState(USERINFO* pUI, SYSTEMTIME* pCurTime)
{
	if(!pUI) return 0;

	if(pUI->PremCode > 0) {
		// ���� �ð� ���
		CTime nt;
		if(pCurTime) nt = *pCurTime;		// ���Ƿ� ������ ����ð�(Ŭ���̾�Ʈ��)
		else nt = CTime::GetCurrentTime();	// ������ ���� �ð�

		// �����̾� ���� �Ͻ�
		CTime et(pUI->PremDate); // ��ŸƮ�ð�

		// ���� �����̾� �Ⱓ�� ���Ѵ�
		CTimeSpan TimeSpan = et - nt;
		int period = TimeSpan.GetTotalMinutes();

		if(period > 0) {
			// �����̾� ���� �ڵ带 ��´�
			int stcode = pUI->PremCode % 10;

			// �����̾� �ڵ��� ������ ���� ������ ���� ���񽺷� ���е�
			//    0    : ���� �̻��
			// 11 - 19 : ������ Ŭ��
			// 21 - 29 : ��Ͼ� Ŭ��
			// 31 - 39 : �߰��� ���񽺸� ���Ͽ� �����

			
			// �����̾� �ڵ�� 10���� ���� �������� ���� ������ ���� �ǹ̸� ����
			// 1 ���� �̿���
			// 2 ���� ��û��
			// 3 �Ͻ�����(���� �̽���)
			// 4 ���� �Ͻ�����
			// 5 ������
			// 6 ����������

			if(stcode == 1 || stcode==2) {		
				return 1;
			}
		}
	}

	return 0;
}


// Global....


#include "StdAfx.h"
#include "Global.h"

// �ƹ�Ÿ ���÷��� ��� ���̺귯�� ��ũ
#pragma comment (lib, "Avatar.lib")

int g_Max_Player    = MAX_PLAYER - 1;
int g_Max_NewPlayer = g_Max_Player + MAX_OBSERVER;

const int TOTAL_CARDNUM  = 52; //�� ī���
const int DUMY_CARD      = 52; //ī�� �޸�
const int EMPTY_CARD     = -1;


C62CutPokerDlg *g_pMainDlg=NULL;		// ���� ���̾�α� ������(��������)
CTitleDlg *g_pTitleDlg=NULL;		// Ÿ��Ʋ ���̾�α� ������(��������)
CLobyDlg *g_pLobyDlg=NULL;			// ���� ���̾�α� ������(��������)
CGameDlg *g_pGameDlg=NULL;			// ���� ���̾�α� ������(��������)
CGameView *g_pGameView=NULL;		// ���� �� ���� ������(��������)
CWebProfileDlg g_WebProfileDlg;

CSockMan SockMan;			// ���� �޴��� Ŭ����
CSound Sound;
xHan g_Han;
CNoChat NoChat;
CBadChatMan BadChat;		// �弳 ���͸� Ŭ����

CAdminMessageDlg *g_pAMes  = NULL; // [������ ��� �۾�]
/////////////////////////////////////////////////////////////////
xSprite GameBackSpr;		// ���� ���ȭ�� ��������Ʈ
xSprite GameBackSpr_7;		// ���� ���ȭ�� ��������Ʈ

xSprite LevelSpr;			// ���� ��������Ʈ
xSprite CharBackSpr;		// ĳ���� �޹��
//////////////////////////////////////////////////////////////////

xSprite InfoBoxSpr;			// ����â ��������Ʈ
xSprite ChatBoxSpr;			// ä�� ��ȭâ ��������Ʈ

xSprite ExitBtnSpr;			// �������ư ��������Ʈ
xSprite InviteBtnSpr;		// �ʴ��ư ��������Ʈ
xSprite FullScreenBtnSpr;	// Ǯ��ũ�� ��ư ��������Ʈ
xSprite WindowBtnSpr;		// �������� ��ư ��������Ʈ
xSprite OptionBtnSpr;		// �ɼǹ�ư ��������Ʈ
xSprite BadUserBtnSpr;		// ��ȭ���� �Ű� ��ư ��������Ʈ
xSprite MasterBtnSpr;    // ����
xSprite MinimizeBtnSpr;  // �ּ�ȭ

// �������� ���� ��ư
xSprite GameStartBtnSpr;
xSprite AllinBtnSpr;
xSprite DieBtnSpr;
xSprite CheckBtnSpr;
xSprite CallBtnSpr;
xSprite PingBtnSpr;
xSprite DadangkBtnSpr;
xSprite MaxBtnSpr;
xSprite HighBtnSpr;
xSprite LowBtnSpr;
xSprite SwingBtnSpr;
xSprite EtcBtnSpr;
////////////////////////////////////////////////////////////////[62]
xSprite ChangeOKBtnSpr;
xSprite ChangeCancelBtnSpr;
// [62]ü���� ī�� �ȳ�
xSprite ChangeCardInfoSpr; // ī�� �ٲٱ� �ȳ� �޼���
xSprite RoomMasterSpr;
xSprite BanishVoteSpr;
xSprite SortSpr;
xSprite AllinAniSpr;
/////////////////////////////////////////////////////////////////
xSprite EtcSpr;				// ��Ÿ
xSprite OringCharSpr;	// �Ļ�
//////////////////////////////////////////////////////////////////////
xSprite BigCardSpr;		// 
xSprite SmallCardSpr;		// ī��

xSprite FocusSpr;		// ���� ��
xSprite TimerSpr;		// Ÿ�̸� ����


// [ ###����### ] 
xSprite JackPotBackSpr;	// ���� ���
xSprite JackPotSpr;		// ���� ����
xSprite ChipSpr;		// Ĩ
/////////////////////////////////////////////////////////////////
xSprite ItemBagSpr;		// 
xSprite WarningSpr;		//  ### [ �ߺ��� IP�� ] ###
xSprite CaptureSpr;     // [ȭ��ĸ��]
xSprite ObserverSpr;    // ### [ ������� ] ###

//////////////////////////////////////////////////
// ### [ ���� ����Ʈ ���� ] ###
xSprite			LobyListBackSpr;
xSprite			LobyListButtonSpr;
xSprite			LobyRoomImgSpr;		//	������ �̹���
//////////////////////////////////////////////////




CString g_ServIP("127.0.0.1");		// ���� ������ IP
UINT g_ServPort=8003;					// ���� ������ ��Ʈ��ȣ
//CString g_ServIP("218.153.10.161");		// ���� ������ IP
//UINT g_ServPort=24201;					// ���� ������ ��Ʈ��ȣ
// ���� 2
//CString g_ServIP("218.153.10.238");		// ���� ������ IP
//UINT g_ServPort=24202;					// ���� ������ ��Ʈ��ȣ
//CString g_ServIP("127.0.0.1");		// ���� ������ IP
//CString g_ServIP("211.59.93.175");		// ���� ������ IP
//CString g_ServIP("218.153.10.226");		// ���� ������ IP
//UINT g_ServPort=25899;					// ���� ������ ��Ʈ��ȣ

//CString g_ServIP("218.153.10.164");		// ���� ������ IP
//UINT g_ServPort=24299;					// ���� ������ ��Ʈ��ȣ

CString g_AuthCookie;					// ���� ��Ű						[SSO �۾�]
CString g_DataCookie;					// ������ ��Ű						[SSO �۾�]
CString g_CpCookie;						// CP��Ű							[SSO �۾�]
CString g_SpareParam;					// ������ ����(CP���ӵ�� ���)	[SSO �۾�]
CString g_LoginID;						// �ڵ� �α��� ID
CString g_LoginPass("1111");			// �ڵ� �α��� �н�����
//CString g_Extra;						// ����Ʈ�� ����					[SSO �۾�] - ����
CString g_MyUniqNo;						// ����Ʈ�� ����

CString g_ServerName;					// ������ ���� �̸�
CString g_ChannelName;					// ������ ä�� �̸�
CString g_NoticeURL;					// �������� URL
CString g_BannerURL1;					// �κ� ���� URL
CString g_BannerURL2;					// ���� ���� URL
CString g_ProfileURL;					// ������ URL
CString g_ItemURL;						// ������ ���� URL	[���� ������ �۾�]
CString g_ItemHelpURL;					// ������ ���� URL	[���� ������ �۾�]
CString g_CashURL;						// ĳ������ URL

CString g_strSrvInfo;					// ä�� ����

int        g_nChannelStyle = 0;  // ���� ��Ÿ��
int        g_nchannel_usernum=0; // ���� 5, 7�ο� ����

CNMStrMan    g_StrMan;
int        g_MSCode = 0; // 0: Korean 1: English 2:Japanese 3:Chinese

CString g_LocationURL;					// ��ġ�˸�(ģ��ã��)�� URL

int g_GameCode = 0;
int g_BgMode = 0;			// ��Ų �÷���

CRect g_OrgMainDlgRect;					// ���� ���̾�α��� ���� ũ��
int g_MainSrvSID = 0;					// ���� ������ ���� ��ȣ
int g_Where = WH_FIRST;					// ���� ��ġ�� ��(0:�α����ϱ� ��, 1:�α�����, 2:����, 3:������)
int g_MyPN = 0;							// ���� �÷��̾� ��ȣ(0-2)
int g_MastPN = 0;						// ������ �÷��̾� ��ȣ(0-2)
BOOL g_Wait = FALSE;					// ������ ��ٸ��� ���ΰ�?


BOOL g_bChannel = FALSE;


//////////// [���� ������ �۾�] ////////////
GAMEITEM_LIST g_MyGameItem;				// �� ���� ������
CTime   g_ServerTime = CTime::GetCurrentTime(); // ������ �ð�
CTime   g_ClientTime = CTime::GetCurrentTime();	// Ŭ���̾�Ʈ�� �ð� (������ Ŭ���̾�Ʈ���� �ð����� ���Ҷ� ���)
CTimeSpan g_CSTimeGap;					// ������ Ŭ���̾�Ʈ�� �ð���
////////////////////////////////////////////




ROOMINFO g_RI={0,};			// �� ���� ����ü
// ### [ ������� ] ###
CPlayer Play[MAX_NEWPLAYER];// �÷��̾� ����
CPlayer g_MyInfo;
int  g_MyIndex;
BOOL g_MyObserver;			//�������ΰ�
BOOL g_bAdminUser = FALSE;  // [������ ��� �۾�]

CPlayScr PlayScr[MAX_PLAYER];			// �÷��̾� ��ũ�� Ŭ����
CDisplay Display;						// ���÷��� ��� ���� Ŭ����
BOOL g_bChangeCardStage;		// ī�� �ٲٱ�
////////////////


POKERGAME g_Poker;
CHAT_MACRO g_ChatMacro; // ��ũ��(ä�� ����)



// [������� ������] 
void g_GetNowSrvTime(COleDateTime &NowSrvTime)
{
//	COleDateTime ClntTime     = g_ClntTime;
//	COleDateTime SrvTime      = g_SrvTime;
//	COleDateTime NowClntTime  = COleDateTime::GetCurrentTime();

//	COleDateTimeSpan TimeSpan = NowClntTime - ClntTime; // �������� �ð��� ���Ѵ�
//	NowSrvTime = SrvTime+TimeSpan; // ������ ����ð��� �����Ѵ�
}


// [ ���� ��Ŀ ] ����
// [ ###����### ] 
INT64 g_JackPotMoney = 0;     // Ŭ���̾�Ʈ
INT64 g_TmpJackPotMoney = 0;  // ����			==> ����ġ �����ٶ� ���


/////////////////////////////////////////

// ���� ��Ʈ��
char *StrArea[17]={
	"��������", "����",     "�λ�",     "����",     "�뱸",
	"����",     "��õ",     "���",     "��⵵",   "������",
	"��û�ϵ�", "��û����", "���ϵ�", "��󳲵�", "����ϵ�",
	"���󳲵�", "���ֵ�"};	

// ���� ��Ʈ��
char* StrJob[21]={
	"��������", "�ʵ��л�", "�߰��", "����(��)��", "ȸ���",
	"������",   "������",   "�ֺ�",   "����",       "�Ƿ���",
	"������",   "������",   "�����", "������",     "�ڿ���",
	"�󸲾�",   "����",   "�����", "�����",     "����",
	"��Ÿ"};

// ���� ��Ʈ���� ����
char* GetStrArea(int num)
{
	if(num<0 || num>=17) return NULL;
	return StrArea[num];
}

// ���� ��Ʈ���� ����
char* GetStrJob(int num)
{
	if(num<0 || num>=21) return NULL;
	return StrJob[num];
}

CGame Game;
CWhisperDlg g_WhisperDlg;


// [������Ŀ] ���� �Ӵ�����
//POKERSET g_POKERSET;

CString g_MakeCommaMoney(INT64 nMoney)
{
	CString strVal("0");

	bool minus = false;

	if(nMoney<0)
	{		
		minus = true;
		nMoney = -nMoney;
	}

	strVal.Format("%I64d",nMoney);


	for(int i=18; i>0; i-=3){
		if(strVal.GetLength()>i){
			strVal.Insert(strVal.GetLength()-i,",");
		}
	}
	if(strVal=="") strVal= "0";

	if( minus ) strVal.Insert(0,"-");
	
	return strVal;
/*
	CString strVal("0");

	if(nMoney<0) return strVal;
	strVal.Format("%I64d",nMoney);

	for(int i=18; i>0; i-=3){
		if(strVal.GetLength()>i){
			strVal.Insert(strVal.GetLength()-i,",");
		}
	}
	if(strVal=="") strVal= "0";
	return strVal;
	*/
}


CString NumberToOrientalString( INT64 number )
{
	CString strrtn;
	CString strtemp;
	INT64 t = number; 

	BOOL bMinus = FALSE;
	if( t < 0 ) {
		bMinus = TRUE;
		t *= -1;
	}

	INT64 v = t / (INT64)10000000000000000;	// �� ����
	if(v > 0) {
		t = t - v*(INT64)10000000000000000;
		strtemp.Format("%I64d��", v);
		strrtn += strtemp;
	}

	v = t / (INT64)1000000000000;	// �� ����
	if(v > 0) {
		t = t - v*(INT64)1000000000000;
		strtemp.Format("%I64d��", v);
		strrtn += strtemp;
	}

	v = t / (INT64)100000000;	// �� ����
	if(v > 0) {
		t = t - v*(INT64)100000000;
		strtemp.Format("%I64d��", v);
		strrtn += strtemp;
	}
	
	v = t / (INT64)10000;	// �� ����
	if(v > 0) {
		t = t - v*(INT64)10000;
		strtemp.Format("%I64d��", v);
		strrtn += strtemp;
	}

	if( t > 0 ) {
		strtemp.Format("%I64d", t);
		strrtn += strtemp;
	}

	if( number == 0 )
	{
		strrtn = "0";
	}

	if( bMinus )
	{
		strrtn.Insert(0,"-");
		//CString temp;
		//temp.Format( "-%s", strrtn );
		//strrtn = temp;
	}
	

   //NUMBERFMT nFmt = { 0, 0, 4, ".", ",", 0 };
   //TCHAR    szVal[32];
   //TCHAR     szOut[32];
   //sprintf ( szVal,"%I64d", number );
   //GetNumberFormat ( NULL, NULL, szVal, &nFmt, szOut, 32 );

   	return strrtn;
}

CString g_MoneyMark(INT64 nMoney)
{
	if(g_pMainDlg->CFG.MoneyMark==0) { // ���� ǥ��
		CString strVal("0");
		
		bool minus = false;
		
		if(nMoney<0)
		{		
			minus = true;
			nMoney = -nMoney;
		}
		
		strVal.Format("%I64d",nMoney);
		
		
		for(int i=18; i>0; i-=3){
			if(strVal.GetLength()>i){
				strVal.Insert(strVal.GetLength()-i,",");
			}
		}
		if(strVal=="") strVal= "0";
		
		if( minus ) strVal.Insert(0,"-");
		
		return strVal;
	}

	CString strrtn; // �ѱ�ǥ��
	CString strtemp;
	INT64 t = nMoney; 

	BOOL bMinus = FALSE;
	if( t < 0 ) {
		bMinus = TRUE;
		t *= -1;
	}

	INT64 v = t / (INT64)10000000000000000;	// �� ����
	if(v > 0) {
		t = t - v*(INT64)10000000000000000;
		strtemp.Format("%I64d��", v);
		strrtn += strtemp;
	}

	v = t / (INT64)1000000000000;	// �� ����
	if(v > 0) {
		t = t - v*(INT64)1000000000000;
		strtemp.Format("%I64d��", v);
		strrtn += strtemp;
	}

	v = t / (INT64)100000000;	// �� ����
	if(v > 0) {
		t = t - v*(INT64)100000000;
		strtemp.Format("%I64d��", v);
		strrtn += strtemp;
	}
	
	v = t / (INT64)10000;	// �� ����
	if(v > 0) {
		t = t - v*(INT64)10000;
		strtemp.Format("%I64d��", v);
		strrtn += strtemp;
	}

	if( t > 0 ) {
		strtemp.Format("%I64d", t);
		strrtn += strtemp;
	}

	if( nMoney == 0 )
	{
		strrtn = "0";
	}

	if( bMinus )
	{
		strrtn.Insert(0,"-");
		//CString temp;
		//temp.Format( "-%s", strrtn );
		//strrtn = temp;`
	}
	

   //NUMBERFMT nFmt = { 0, 0, 4, ".", ",", 0 };
   //TCHAR    szVal[32];
   //TCHAR     szOut[32];
   //sprintf ( szVal,"%I64d", number );
   //GetNumberFormat ( NULL, NULL, szVal, &nFmt, szOut, 32 );

   	return strrtn;


}

//��, ��, �� 
int NumberToParse(INT64 money, int &Num1, int &Num2, int &Num3, int &Num4) {

	INT64 t = money; 
/*	
	INT64 v = t / (INT64)10000000000000000;	// �� ����
	if(v > 0) {
		t = t - v*(INT64)10000000000000000;
		Num1 = v;
	}
*/
	INT64 v = t / (INT64)1000000000000;	// �� ����
	if(v > 0) {
		t = t - v*(INT64)1000000000000;
		Num1 = v;
	}

	v = t / (INT64)100000000;	// �� ����
	if(v > 0) {
		t = t - v*(INT64)100000000;
		Num2 = v;
	}
	
	v = t / (INT64)10000;	// �� ����
	if(v > 0) {
		t = t - v*(INT64)10000;
		Num3 = v;
	}

	if( t > 0 ) {
		Num4 = t;
	}
	return 0;
}

/*

CString g_MakeCommaMoney(INT64 nMoney)
{
	CString strVal("0");

	bool minus = false;

	if(nMoney<0)
	{		
		minus = true;
		nMoney = -nMoney;
	}

	strVal.Format("%I64d",nMoney);


	for(int i=18; i>0; i-=3){
		if(strVal.GetLength()>i){
			strVal.Insert(strVal.GetLength()-i,",");
		}
	}
	if(strVal=="") strVal= "0";

	if( minus ) strVal.Insert(0,"-");
	
	return strVal;
}
*/
/*
void BrowserMove( CString targeturl ) 
{ 
        //�ƹ�Ÿ ��⿡ ����ִ� �Լ��Դϴ�. 
        char strbuf[1024]; 
        GetBaseAvatarDir(strbuf, 1023); 

        CString exeName = strbuf; 
        exeName += "\\NMIEMoveMFC.exe"; 

        ::ShellExecute(NULL, "open" ,exeName, targeturl, NULL, SW_HIDE); 
} 
*/
CString g_GetLevelName(int nLevel)
{
	if(nLevel== 0) return g_StrMan.Get(_T("LEVEL_NAME1"));	  
	if(nLevel== 1) return g_StrMan.Get(_T("LEVEL_NAME2"));
	if(nLevel== 2) return g_StrMan.Get(_T("LEVEL_NAME3"));
	if(nLevel== 3) return g_StrMan.Get(_T("LEVEL_NAME4"));
	if(nLevel== 4) return g_StrMan.Get(_T("LEVEL_NAME5"));
	if(nLevel== 5) return g_StrMan.Get(_T("LEVEL_NAME6"));
	if(nLevel== 6) return g_StrMan.Get(_T("LEVEL_NAME7"));
	if(nLevel== 7) return g_StrMan.Get(_T("LEVEL_NAME8"));
	if(nLevel== 8) return g_StrMan.Get(_T("LEVEL_NAME9"));
	if(nLevel== 9) return g_StrMan.Get(_T("LEVEL_NAME10"));
	if(nLevel==10) return g_StrMan.Get(_T("LEVEL_NAME11"));
	if(nLevel==11) return g_StrMan.Get(_T("LEVEL_NAME12"));
	if(nLevel==12) return g_StrMan.Get(_T("LEVEL_NAME13"));
	if(nLevel==13) return g_StrMan.Get(_T("LEVEL_NAME14"));
	if(nLevel==14) return g_StrMan.Get(_T("LEVEL_NAME15"));
	if(nLevel==15) return g_StrMan.Get(_T("LEVEL_NAME15"));
	if(nLevel==16) return g_StrMan.Get(_T("LEVEL_NAME15"));
	if(nLevel==17) return g_StrMan.Get(_T("LEVEL_NAME15"));
	if(nLevel==18) return g_StrMan.Get(_T("LEVEL_NAME15"));
	if(nLevel==19) return g_StrMan.Get(_T("LEVEL_NAME15"));

	return  g_StrMan.Get(_T("LEVEL_NAME15"));
}

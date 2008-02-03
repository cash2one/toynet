// 글로벌 변수, 클래스, 함수..

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

//#include "ClientDlg.h"

#include "SockMan.h"

extern CClientDlg *g_pMainDlg;
extern CTitleDlg *g_pTitleDlg;
extern CLobyDlg *g_pLobyDlg;
extern CGameDlg *g_pGameDlg;
extern CGameView *g_pGameView;

extern CSockMan SockMan;		// 소켓 메니져 클래스

/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////

#define WH_FIRST	0			// 로그인하기전
#define WH_LOGIN	1			// 로그인중
#define WH_TITLE	2			// 타이틀 화면
#define WH_LOBY		3			// 대기실
#define WH_GAME		4			// 게임중



#endif
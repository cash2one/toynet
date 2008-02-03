#ifndef __DEFINECOMMYDATA__
#define __DEFINECOMMYDATA__

// 서버들과 서로의 인식을 위한 메시지 
extern UINT uCmmHaloMsgSC;	
extern UINT uCmmHaloMsgCS;
extern UINT uCmmGotBootMsgSC;	// 서버가 부팅된것을 인식했다고 관리기에서 보낸다.

// 서버 상태 메시지 
extern UINT uCmmGameStatusSC;
extern UINT uCmmGameStatusCS;

// 서버 메시지 출력
extern UINT uCmmServerMsgSendSC;
extern UINT uCmmServerMsgSendCS;

// 서버 신규 접속 막음 
extern UINT uCmmStopAcceptSC;
extern UINT uCmmStopAcceptCS;


// 서버 다운 메시지 출력 
extern UINT uCmmServerDownMsgSC;
extern UINT uCmmServerDownMsgCS;

// 사용자 체크 함수 
extern UINT uCmmServerUserSizeSC;
extern UINT uCmmServerUserSizeCS;

extern UINT uCmmDisbandUserANS;

typedef struct {
	int  gamecode;
	int  servercode;
	int  port;
	char name[64];
	char path[256];
} tag_ServerHalo;


typedef struct {
	int nCode;
	char strMsg[256];
}  tag_SvrMsg;

typedef struct {
	int  Code;
	char ID[16];
	char strMsg[128];
} tag_SingleMsg;

typedef struct {
	char ID[16];
} tag_DisbandUser;


typedef struct {
	int  Index;
	char ID[16];
} tag_MsgSendOK;
///////////////////////////////////////////////////////////
// Function
extern BOOL InitNetmarbleCommn(void);

extern void M_HaloGameServer(HWND myhwnd = HWND_BROADCAST);			// 서버들에게 관리기의 존재를 알린다.
extern void M_ImHereBridge(HWND myhwnd,int gamecode,int servercode,int port,char *name);
extern void M_GotServerBootMsg(HWND myhwnd,HWND svrhwnd);

extern void M_GetServerStatus(HWND myhwnd,HWND svrhandle);			// 서버들에게 상태를 물어본다.
extern void M_SendMySetatus(HWND myhwnd,int status,int usersize);	// 자신의 상태를 관리기에게 알린다.


extern void M_ServerDownNow(HWND myhwnd,HWND svrhandle);		// 서버에게 다운 메시지를 준다. 
extern void M_ImDieNow(HWND svrhandle);				// 나 주거유우우 라고 관리기에게 알린다. 3분뒤에 핸들 체크하여 강제 종료.

extern void SetBridgeHandle(HWND hwnd);
extern void SendMsgToServer(HWND svrhwnd,HWND myhwnd,int code,char *msg);
extern void SendMsgToServerSingleUser(HWND svrhwnd,HWND myhwnd,int code,char *msg,char *id);

extern void SendUserSizeToBridge(HWND svrhandle,int usersize);

// 서버 접속 허용 / 안허용 플레그 설정 
extern void M_SetStopAccept(HWND myhwnd,HWND svrhandle,int bStopAccept);
extern void M_ServerAcceptStatus(HWND svrhandle,int bStopAccept);

// 사용자 강제 종료 
extern void M_DisconnectUser(HWND svrhandle,HWND myhwnd,char *id);
extern void M_DisConnectAns(HWND svrhandle,BOOL tf);

extern void M_SingleMsgSecc(HWND  myhwnd,char *id,int index);
#endif
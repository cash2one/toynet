// SyncFPS.cpp: implementation of the CSyncFPS class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SyncFPS.h"

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSyncFPS::CSyncFPS()
{
	FPS = 75;
	PreTime = NowTime = 0;
	ElapseTime = ProcessTime = 0;
}

CSyncFPS::~CSyncFPS()
{

}

void CSyncFPS::Init(int fps)
{
	FPS = fps;
	PreTime = NowTime = timeGetTime();
	ElapseTime = ProcessTime = 0;
}

int CSyncFPS::FrameSync(BOOL bWait)
{
	int skipframe = 0;
	double spf = (double)1000 / FPS;
	
	NowTime  = timeGetTime();
	double deltatick = (double)(NowTime - PreTime);
	PreTime = NowTime;
	
	// 안전 장치
	if(deltatick > (double)2000) deltatick = (double)2000;
	
	ElapseTime += deltatick;
	ProcessTime += spf;

	if(ProcessTime > ElapseTime) {
		if(bWait) ::Sleep((int)(ProcessTime - ElapseTime));
		else ProcessTime = ElapseTime;
		return 0;
	}

	if(ProcessTime < ElapseTime && spf>0) {
		skipframe = (int)((ElapseTime - ProcessTime) / spf);
		ProcessTime += (spf * skipframe);
	}

	return skipframe;
}

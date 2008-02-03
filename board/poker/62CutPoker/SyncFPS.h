// SyncFPS.h: interface for the CSyncFPS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYNCFPS_H__E39C4638_FBB8_41F4_9536_54A974EF9A8C__INCLUDED_)
#define AFX_SYNCFPS_H__E39C4638_FBB8_41F4_9536_54A974EF9A8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSyncFPS  
{
public:
	int FPS;
	UINT PreTime;
	UINT NowTime;
	//UINT ElapseTime;
	//UINT ProcessTime;
	double ElapseTime;
	double ProcessTime;

	CSyncFPS();
	virtual ~CSyncFPS();

	void Init(int fps);
	int FrameSync(BOOL bWait = TRUE);
};

#endif // !defined(AFX_SYNCFPS_H__E39C4638_FBB8_41F4_9536_54A974EF9A8C__INCLUDED_)

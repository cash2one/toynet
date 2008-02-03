// DataSaveThread.cpp : implementation file
//

#include "stdafx.h"
#include "NMServerAgent.h"
#include "NMServerAgentDlg.h"
#include "DataSaveThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataSaveThread

IMPLEMENT_DYNCREATE(CDataSaveThread, CWinThread)

CDataSaveThread::CDataSaveThread()
{
}

CDataSaveThread::~CDataSaveThread()
{
}

BOOL CDataSaveThread::InitInstance()
{
	return TRUE;
}

int CDataSaveThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CDataSaveThread, CWinThread)
	//{{AFX_MSG_MAP(CDataSaveThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
    ON_THREAD_MESSAGE(TM_THREADMSG,SaveData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataSaveThread message handlers

BOOL bStart = FALSE;
void CDataSaveThread::SaveData()
{
//AfxMessageBox("sssssssssssssssssssssssssssssssssss!!");
	bStart = TRUE;

}
/*
int CDataSaveThread::Run() 
{
	while(1){
		if(bStart){
			AfxMessageBox("ddd");
			bStart = FALSE;
		}
		Sleep(10);
	}
	return CWinThread::Run();
}
*/

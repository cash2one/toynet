// ExHandle.cpp
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "ExHandle.h"
#include <atlbase.h>

#ifndef _COMPILELIB
// 라이브러리 컴파일 중에는 이하는 컴파일 되지 않는다

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifndef DISABLE_EXHANDLE

	// 링커 옵션에 "Generate debug info" 옵션을 설정하도록 알려준다(함수 콜스택을 남기기 위함)
	#pragma message( "================================ NOTICE BY ExHandle Lib ==============================" )
	#pragma message( "  * SET LINKER OPTION FOR '/DEBUG' and '/debugtype:both'" )
	#pragma message( "    1. Turn on 'Generate debug info' At Project->Setting->Link(Tab)->General(Category)" )
	#pragma message( "    2. Select 'Both formats' At Project->Setting->Link(Tab)->Debug(Category)" )
	#pragma message( "    If you do not turn on /DEBUG option, linking will be failed!" )
	#pragma message( "  > FOR KOREAN: 반드시 아래의 두 옵션을 활성화 할 것!" )
	#pragma message( "    1. 'Generate debug info' 옵션을 체크할 것(Project->Setting->Link(Tab)->General(Category))" )
	#pragma message( "    2. 'Both formats' 옵션을 선택할 것(Project->Setting->Link(Tab)->Debug(Category))" )
	#pragma message( "======================================================================================" )

	// 이유는 모르겠지만 메뉴에서 직접 Generate debug info을 설정하지 않으면 링크 에러가 발생한다
	// ( 아래의 옵션들은 프로젝트 세팅 메뉴에서 직접 설정하지 않으면 적용되지 않는 듯하다 )
	#pragma comment(linker, "/DEBUG")
	#pragma comment(linker, "/debugtype:both")	// 디버그 정보를 Both format으로 설정(기본값은 Microsoft format)

#endif

// 자동으로 라이브러리 링크
#pragma message( "Auto linking.... ExHandle Lib" )
#ifdef _DEBUG
#pragma comment(lib, "ExHandle_Debug.lib")
#else
#pragma comment(lib, "ExHandle_Release.lib")
#endif 

//////////////////////////////////////////////////////////////////////////////////

extern void ExHnd_SetReportHeader(PTSTR strHeader);
extern void ExHnd_SetReportExeName(PTSTR strFName);
extern void ExHnd_Disable();

CExHandle::CExHandle()
{
	#ifndef DISABLE_EXHANDLE
		char strExeName[MAX_PATH] = {0,};
		if(GetErrExeName(strExeName, sizeof(strExeName)-1)) {
			ExHnd_SetReportHeader("");
			ExHnd_SetReportExeName(strExeName);
		}
	#else
		ExHnd_Disable();
	#endif
}

BOOL CExHandle::GetErrExeName(char* strBuffer, int buffersize)
{
	CRegKey reg;
	if(ERROR_SUCCESS != reg.Open(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion")) {
		return FALSE;
	}
	char strCommonFilesDir[1024] = {0,};
	DWORD strsize = 1024;
	if(ERROR_SUCCESS != reg.QueryValue(strCommonFilesDir, "CommonFilesDir", &strsize)) {
		return FALSE;
	}

	char strBaseDir[1024] = {0,};
	wsprintf(strBaseDir, "%s\\Netmarble\\Avatar\\ErrReport.exe", strCommonFilesDir);

	memset(strBuffer, 0, buffersize);
	strncpy(strBuffer, strBaseDir, buffersize);
	return TRUE;
}

CExHandle g_ExHandle;


#endif


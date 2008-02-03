// ExHandle.cpp
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "ExHandle.h"
#include <atlbase.h>

#ifndef _COMPILELIB
// ���̺귯�� ������ �߿��� ���ϴ� ������ ���� �ʴ´�

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifndef DISABLE_EXHANDLE

	// ��Ŀ �ɼǿ� "Generate debug info" �ɼ��� �����ϵ��� �˷��ش�(�Լ� �ݽ����� ����� ����)
	#pragma message( "================================ NOTICE BY ExHandle Lib ==============================" )
	#pragma message( "  * SET LINKER OPTION FOR '/DEBUG' and '/debugtype:both'" )
	#pragma message( "    1. Turn on 'Generate debug info' At Project->Setting->Link(Tab)->General(Category)" )
	#pragma message( "    2. Select 'Both formats' At Project->Setting->Link(Tab)->Debug(Category)" )
	#pragma message( "    If you do not turn on /DEBUG option, linking will be failed!" )
	#pragma message( "  > FOR KOREAN: �ݵ�� �Ʒ��� �� �ɼ��� Ȱ��ȭ �� ��!" )
	#pragma message( "    1. 'Generate debug info' �ɼ��� üũ�� ��(Project->Setting->Link(Tab)->General(Category))" )
	#pragma message( "    2. 'Both formats' �ɼ��� ������ ��(Project->Setting->Link(Tab)->Debug(Category))" )
	#pragma message( "======================================================================================" )

	// ������ �𸣰����� �޴����� ���� Generate debug info�� �������� ������ ��ũ ������ �߻��Ѵ�
	// ( �Ʒ��� �ɼǵ��� ������Ʈ ���� �޴����� ���� �������� ������ ������� �ʴ� ���ϴ� )
	#pragma comment(linker, "/DEBUG")
	#pragma comment(linker, "/debugtype:both")	// ����� ������ Both format���� ����(�⺻���� Microsoft format)

#endif

// �ڵ����� ���̺귯�� ��ũ
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


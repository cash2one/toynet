// ExHandle.h
//
//////////////////////////////////////////////////////////////////////
#ifndef _COMPILELIB
// 라이브러리 컴파일 중에는 이하는 컴파일 되지 않는다

#pragma once

#ifndef __MSJEXHND_H__
#define __MSJEXHND_H__
///////////////////////////////

// 아래의 define을 설정하면 익셉션 핸들러 사용을 취소할 수 있다
// #define DISABLE_EXHANDLE


class CExHandle
{
public:
	CExHandle();

	BOOL GetErrExeName(char* strBuffer, int buffersize);
};


//////////////////////////////
#endif

#endif
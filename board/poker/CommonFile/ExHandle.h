// ExHandle.h
//
//////////////////////////////////////////////////////////////////////
#ifndef _COMPILELIB
// ���̺귯�� ������ �߿��� ���ϴ� ������ ���� �ʴ´�

#pragma once

#ifndef __MSJEXHND_H__
#define __MSJEXHND_H__
///////////////////////////////

// �Ʒ��� define�� �����ϸ� �ͼ��� �ڵ鷯 ����� ����� �� �ִ�
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
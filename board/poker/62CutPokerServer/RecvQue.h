#ifndef __RECVQUE_H__
#define __RECVQUE_H__

#ifndef __AFXWIN_H__
#include <windows.h>
#endif

class CRecvQue
{
public:
	// ť ����
	char *m_pBuf;

	int m_nQueSize;
	int m_nSPos;
	int m_nEPos;
	int m_nNowSize;

	CRecvQue(int quesize = 0);
	~CRecvQue();

	// Define�� ����� ����ϴ� ���� �ζ��� �Լ��� ����ϸ� �ȵ�
	// �Լ��� Ŭ���� ����� ����Ͽ����� �ȵ�

	void Destroy();

	void Reset();

	void SetQueSize(int quesize);

	inline int GetQueSize() { return m_nQueSize; }

	// ť �������� ù��° �����͸� ����
	inline char* GetBufPtr() { return (m_pBuf + m_nSPos); };
	// �ѹ��� ó���� �� �ִ� ����� ���� ����� ����
	inline int GetBlockSize() { 
		if(m_nSPos + m_nNowSize > m_nQueSize) return (m_nQueSize-m_nSPos);
		else return m_nNowSize; 
	}
	// ť�� ����� �������� ��ü ����� ����
	inline int GetTotalSize() { return m_nNowSize; };
	// ť�� ������� ���� �����͸� ����
	inline char* GetFreeBufPtr() { return (m_pBuf + m_nEPos); };
	// �ѹ��� ó���� �� �ִ� ����� ������� ����� ����
	inline int GetFreeBlockSize() { return (min((m_nQueSize-m_nNowSize), m_nQueSize-m_nEPos)); }

	BOOL __fastcall EnQue(void *pData, int size);
	
	BOOL __fastcall EnQue_Size(int size);

	BOOL __fastcall DeQue_Size(int size);

	BOOL __fastcall PeekQue(void *pData, int size);

	BOOL __fastcall DeQue(void *pData, int size);

};


#endif
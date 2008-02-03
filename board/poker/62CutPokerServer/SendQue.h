#ifndef __SENDQUE_H__
#define __SENDQUE_H__

#ifndef __AFXWIN_H__
#include <windows.h>
#endif

class CSendQue
{
public:
	// 큐 버퍼
	char *m_pBuf;

	int m_nQueSize;
	int m_nSPos;
	int m_nEPos;
	int m_nNowSize;

	CSendQue(int quesize = 0);
	~CSendQue();

	// Define된 상수를 사용하는 경우는 인라인 함수를 사용하면 안됨
	// 함수를 클래스 헤더에 기술하여서도 안됨

	void Destroy();

	void Reset();

	void SetQueSize(int quesize);

	inline int GetQueSize() { return m_nQueSize; }

	// 큐 데이터의 첫번째 포인터를 구함
	inline char* GetBufPtr() { return (m_pBuf + m_nSPos); };

	// 한번에 처리될 수 있는 연결된 블럭의 사이즈를 구함
	inline int GetBlockSize() {
		if(m_nSPos + m_nNowSize > m_nQueSize) return (m_nQueSize-m_nSPos);
		else return m_nNowSize; 
	}

	// 큐에 저장된 데이터의 전체 사이즈를 구함
	inline int GetTotalSize() { return m_nNowSize; };

	BOOL __fastcall EnQue(void *pData, int size);

	BOOL __fastcall DeQue_Size(int size);
};


#endif
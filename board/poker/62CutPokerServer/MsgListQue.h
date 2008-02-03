#ifndef __MSGLISTQUE_H__
#define __MSGLISTQUE_H__

#define MAX_MSGLISTNUM 100			// 최대 메시지 리스트 수
#define MAX_MSGBUFFERLIMIT 10000000	// 모든 버퍼의 최대합 (너무 많은 메모리가 큐에 할당되는 경우를 막음)

class CMsgListQue
{
public:
	// 크리티컬 섹션
	CRITICAL_SECTION m_CriticalSection;

	char* m_pBuf[MAX_MSGLISTNUM];
	int m_nBufSize[MAX_MSGLISTNUM];
	int m_nSignal[MAX_MSGLISTNUM];
	int m_nSID[MAX_MSGLISTNUM];

	int m_nNowSPos;
	int m_nQuededNum;
	int m_nTotBufSize;

	CMsgListQue();
	~CMsgListQue();
	void Clear();

	BOOL EnQue(void *pData, int size, int signal, int sid);
	BOOL DeQue();

	inline int GetQuededNum() {return m_nQuededNum;};
	inline char* GetBufPtr() {return m_pBuf[m_nNowSPos];};
	inline int GetBufSize() {return m_nBufSize[m_nNowSPos];};
	inline int GetSignal() {return m_nSignal[m_nNowSPos];};
	inline int GetSID() {return m_nSID[m_nNowSPos];};
};


#endif
#ifndef __MSGLISTQUE_H__
#define __MSGLISTQUE_H__

#define MAX_MSGLISTNUM 100			// �ִ� �޽��� ����Ʈ ��
#define MAX_MSGBUFFERLIMIT 10000000	// ��� ������ �ִ��� (�ʹ� ���� �޸𸮰� ť�� �Ҵ�Ǵ� ��츦 ����)

class CMsgListQue
{
public:
	// ũ��Ƽ�� ����
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
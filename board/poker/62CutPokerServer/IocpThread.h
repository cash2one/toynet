#ifndef __IOCPTHREAD_H__
#define __IOCPTHREAD_H__

#define MAX_IOCP_THREAD 50

class CIocpThread
{
public:
	HANDLE m_hIOCP;
	int m_ThreadNum;
	HANDLE m_hThread[MAX_IOCP_THREAD];
	static UINT __stdcall IocpThreadProc(LPVOID pParam);

	CIocpThread();
	~CIocpThread();

	BOOL Create(int workthreadnum = 0);	// default: workthreadnum=0 �ΰ�� CPU����*2���� ��Ŀ �����带 ����
	void Destroy();
	HANDLE GetIocpHandle() {return m_hIOCP;};
};


#endif
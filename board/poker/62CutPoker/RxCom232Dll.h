#define WM_COMM_READ (WM_USER +1)

#define WM_COMM_KEYPADDOWN (WM_USER+10)
#define WM_COMM_KEYPADUP (WM_USER+11)

#define WM_COMM_COININ (WM_USER+21)
#define WM_COMM_COINOUT (WM_USER+22)
#define WM_COMM_COINEMPTY (WM_USER+23)

#define BUFF_SIZE		4192
#define MAX_RECV_BUF	4196
#define RX_LEN			1024


//	��� Ŭ����	CComm232 
class CQueue
{
public:
	BYTE buff[BUFF_SIZE];
	int m_iHead, m_iTail;
	CQueue();
	void Clear();
	int GetSize();
	BOOL PutByte(BYTE b);
	BOOL GetByte(BYTE *pb);
};

class AFX_EXT_CLASS CComm232
{
public:



	CComm232();
	~CComm232();
	
	//--------- ȯ�� ���� -----------------------------------------//
	BOOL        check;
	HANDLE		m_hComm;				// ��� ��Ʈ ���� �ڵ�
	CString		m_sPortName;			// ��Ʈ �̸� (COM1 ..)
	BOOL		m_bConnected;			// ��Ʈ�� ���ȴ��� ������ ��Ÿ��.
	OVERLAPPED	m_osRead, m_osWrite;	// ��Ʈ ���� Overlapped structure
	HANDLE		m_hThreadWatchComm;		// Watch�Լ� Thread �ڵ�.
	WORD		m_wPortID;				// WM_COMM_READ�� �Բ� ������ �μ�.
	CQueue      m_QueueRead;			//ť����

	//--------- �ܺ� ��� �Լ� ------------------------------------//
	BOOL	OpenPort(CString strPortName, 
					   DWORD dwBaud, BYTE byData, BYTE byStop, BYTE byParity );//��Ʈ ���� 
	void	ClosePort();				//��Ʈ �ݱ�
	DWORD	WriteComm(BYTE *pBuff, DWORD nToWrite);//��Ʈ�� ������ ����

	void ProcessRcvData();
	BOOL OutCoin(int nOutCredit);
	void TestOutCoin1();
	void TestOutCoin2();
	BOOL IsEmptyCoin();		// ���뿡 �ܰ� ������ ���� ( ����� �ʿ���� )
	void ConfirmEmpty();	// ���뿡 ������ ä��� Ȯ�ν� ȣ�� �Լ�

	//--------- ���� ��� �Լ� ------------------------------------//
	DWORD	ReadComm(BYTE *pBuff, DWORD nToRead);//��Ʈ���� ������ �о����

	HWND hCommWnd;

private:
	UINT m_nKeyPad;
	BOOL m_bCoinIn;
	BOOL m_bCoinOut;
	BOOL m_bCoinOutConfirm;
	BOOL m_bCoinEmpty;
	int m_nSend00Cnt;
	int m_nSend02Cnt;
	int  m_nOutCredit;	// ������ �� ũ��..
	BYTE m_cTxData[RX_LEN];
	BYTE m_byRcvBuf[MAX_RECV_BUF];

	void OutOneCoin();
	BYTE GetCheckSum(int nType);
	DWORD ProcessRxData(int &nType);

	void DwordTo4Byte(BYTE *cBuff, DWORD lVal);
	void IntTo2Byte(BYTE *cBuff, int nVal);
	DWORD Byte4ToDword(BYTE cBuff1, BYTE cBuff2, BYTE cBuff3, BYTE cBuff4);
	int Byte2ToInt(BYTE cBuff1, BYTE cBuff2);
};

// Thread�� ����� �Լ� 
DWORD	ThreadWatchComm(CComm232* pComm);


#define WM_COMM_READ (WM_USER +1)

#define WM_COMM_KEYPADDOWN (WM_USER+10)
#define WM_COMM_KEYPADUP (WM_USER+11)

#define WM_COMM_COININ (WM_USER+21)
#define WM_COMM_COINOUT (WM_USER+22)
#define WM_COMM_COINEMPTY (WM_USER+23)

#define BUFF_SIZE		4192
#define MAX_RECV_BUF	4196
#define RX_LEN			1024


//	통신 클래스	CComm232 
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
	
	//--------- 환경 변수 -----------------------------------------//
	BOOL        check;
	HANDLE		m_hComm;				// 통신 포트 파일 핸들
	CString		m_sPortName;			// 포트 이름 (COM1 ..)
	BOOL		m_bConnected;			// 포트가 열렸는지 유무를 나타냄.
	OVERLAPPED	m_osRead, m_osWrite;	// 포트 파일 Overlapped structure
	HANDLE		m_hThreadWatchComm;		// Watch함수 Thread 핸들.
	WORD		m_wPortID;				// WM_COMM_READ와 함께 보내는 인수.
	CQueue      m_QueueRead;			//큐버퍼

	//--------- 외부 사용 함수 ------------------------------------//
	BOOL	OpenPort(CString strPortName, 
					   DWORD dwBaud, BYTE byData, BYTE byStop, BYTE byParity );//포트 열기 
	void	ClosePort();				//포트 닫기
	DWORD	WriteComm(BYTE *pBuff, DWORD nToWrite);//포트에 데이터 쓰기

	void ProcessRcvData();
	BOOL OutCoin(int nOutCredit);
	void TestOutCoin1();
	void TestOutCoin2();
	BOOL IsEmptyCoin();		// 돈통에 잔고가 없는지 여부 ( 사용할 필요없음 )
	void ConfirmEmpty();	// 돈통에 코인을 채우고 확인시 호출 함수

	//--------- 내부 사용 함수 ------------------------------------//
	DWORD	ReadComm(BYTE *pBuff, DWORD nToRead);//포트에서 데이터 읽어오기

	HWND hCommWnd;

private:
	UINT m_nKeyPad;
	BOOL m_bCoinIn;
	BOOL m_bCoinOut;
	BOOL m_bCoinOutConfirm;
	BOOL m_bCoinEmpty;
	int m_nSend00Cnt;
	int m_nSend02Cnt;
	int  m_nOutCredit;	// 내보낼 돈 크기..
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

// Thread로 사용할 함수 
DWORD	ThreadWatchComm(CComm232* pComm);


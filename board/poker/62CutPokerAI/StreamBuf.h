// 스트림 버퍼 관리 클래스 헤더

class CStreamBuf
{
public:
	char *pBuf;
	int nBufSize;
	int nUsedSize;

	CStreamBuf();
	~CStreamBuf();
	
	BOOL Init(int size);
	void Destroy();

	int GetBufSize() {return nBufSize;}
	int GetUsedSize() {return nUsedSize;}
	int GetFreeSize() {return (nBufSize - nUsedSize);}
	
	char* GetStartPointer() {return pBuf;}
	char* GetEndPointer() {return (pBuf+nUsedSize);}

	BOOL AddUsedSize(int size); // 사용된 사이즈를 증가시킨다(단순히 카운터만 증가시킴)
	BOOL DelData(int size);     // 버퍼의 첫머리부터 지정한 사이즈까지의 내용을 삭제하고
	                            // 다시 버퍼의 처음으로 올 수 있도록 복사한다.

	int AddData(void* pData, int size);			// 버퍼에 데이터를 저장
	int AddData_Encode(void* pData, int size);	// 버퍼에 데이터를 저장(암호화)
	int Data_Decode(int start, int size);		// 버퍼에 데이터를 저장된 암호화 데이터를 디코딩
	int GetData(void *pTarget, int tarsize);	// 버퍼에서 데이터를 읽어들인후 내용을 삭제
	int PeekData(void *pTarget, int peeksize, int tarsize);	// 데이터는 읽되 내용은 삭제하지 않음
};


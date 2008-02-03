// ��Ʈ�� ���� ���� Ŭ���� ���

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

	BOOL AddUsedSize(int size); // ���� ����� ������Ų��(�ܼ��� ī���͸� ������Ŵ)
	BOOL DelData(int size);     // ������ ù�Ӹ����� ������ ����������� ������ �����ϰ�
	                            // �ٽ� ������ ó������ �� �� �ֵ��� �����Ѵ�.

	int AddData(void* pData, int size);			// ���ۿ� �����͸� ����
	int AddData_Encode(void* pData, int size);	// ���ۿ� �����͸� ����(��ȣȭ)
	int Data_Decode(int start, int size);		// ���ۿ� �����͸� ����� ��ȣȭ �����͸� ���ڵ�
	int GetData(void *pTarget, int tarsize);	// ���ۿ��� �����͸� �о������ ������ ����
	int PeekData(void *pTarget, int peeksize, int tarsize);	// �����ʹ� �е� ������ �������� ����
};


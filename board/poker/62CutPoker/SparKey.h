/****************FunctionCode ********************/
#define SPARKFIND	      		 1   				//  Find SparKey
#define SPARKCLOSE	  		 2				// Close SparKey
#define SPARKREAD	 		 3   				//Read SparKey
#define SPARKWRITE	   		 4  			 	//Write SparKey
#define SPARKSETID	    		 5				//Set user ID
#define SPARKGETID	  		 6				//Get user ID
#define SPARKRANDOM	       		 7				//Random data
#define SPARKSEED	       		 8				//Seed code
#define SPARKARITHMETIC    	 9				//Write algorithm
#define SPARKCALCULATE1    	10				// Check algorithm1
#define SPARKCALCULATE2    	11				//Check algorithm2
#define SPARKCALCULATE3	   	12				//Check algorithm3
#define SPARKSETMODULE	   	13				//Set Module
#define SPARKCHECKMODULE   	14				// Check Module
#define SPARKDEC		 	15				//Descending
#define SPARKENCODE	       	16				//Encode
#define SPARKDECODE	       	17				//Decode

/****************ErrorCode ********************/
#define    ERR_SPKSUCCEED		0x0000 		//Return correct 
#define    ERR_SPKOPEN 			0x0100  		//Open SparKey error
#define    ERR_SPKBASICPSW		0x0101		//Basic passwords error	
#define    ERR_SPKNOTFOUND		0x0102 		//SparKey not found  	
#define    ERR_SPKCLOSE			0x0200		//Close SparKey error
#define    ERR_SPKR_INVALID_ADDR_OR_LEN	0x0300	//Read length or Start address error
#define    ERR_SPKREAD			0x0301 		//Read data error
#define    ERR_SPKW_INVALID_ADDR_OR_LEN	0x0400 	//Write length or Start address error
#define    ERR_SPKWRITE			0x0401 		//Write data error
#define    ERR_SPKSETID			0x0500   		//Write ID error
#define    ERR_SPKGETID			0x0600 		//Read ID error		
#define    ERR_SPKRANDOM 		0x0700  		//Get radome data error	
#define    ERR_SPKGETSEED		0x0800 		//Get seed code error	
#define    ERR_SPKWRITEA_INVALID_ADDR_OR_LEN		0x0900  //Write algorithm lengh or address error	
#define    ERR_SPKWRITEA_DATA		0x0901 		 //Write algorithm error	
#define    ERR_SPKVALUEOVERFLOW	0x0902 		 //Write algorithm constant over 64
#define    ERR_SPKWRITEACOMMAND	0x0903 		//Write algorithm command error	
#define    ERR_CALCULATE1			0x1000  		//Read algorithm1 error	
#define    ERR_CALCULATE1_ADDR	0x1001  		//Read algorithm1 address error	
#define    ERR_CALCULATE2			0x1100 		//Read algorithm2 error
#define    ERR_CALCULATE2_ADDR	0x1201  		//Read algorithm2 address error	
#define    ERR_CALCULATE3			0x1200  		//Read algorithm3 error	
#define    ERR_CALCULATE3_ADDR	0x1201  		//Read algorithm3 address error		
#define    ERR_WRITEMODULE_ADDR	 0x1300  		//Write module adress error	
#define    ERR_WRITEMODULE		0x1301  		//Write module  error	
#define    ERR_CHECKMODULE_ADDR	0x1400   		//Check module address error	
#define    ERR_CHECKMODULE		0x1401  		//Check Module error			
#define    ERR_DECREASE			0x1500 		//Decrease 1 error				
#define    ERR_ENCODE				0x1600  		//Encode error				
#define    ERR_DECODE				0x1700  		//Decode error				
#define    ERR_ACCESS_LIMIT		0x2001  		//Try to perform by basic passwords, the required advanced passwords error	


#ifdef __cplusplus
extern "C"
{
#endif
DWORD __stdcall SparKey(WORD FunctionCode,WORD* Password1,WORD* Password2, 
	WORD* Password3,WORD* Password4,BYTE* IoBuffer,WORD SparKeyW,DWORD* TNum);
#ifdef __cplusplus
}
#endif



/**************************************************************
Service 1(SPARKFIND): Find SparKey

	�������������ڼ������ָ�������SparKey ��

	�﷨:
	
	
	���������
		FunctionCode =SPARKFIND;
		Password1= ����1;
		Password2= ����2;
		Password3= ����3; ����ѡ��
		Password4= ����4; ����ѡ��
		TNum= SparKey Type (1 ---- ������;2 ----USB ��;others ---- Ĭ�ϼ��)

	�������: 
		TNum=SparKey��Ӳ��ID
	
	����ֵ��
	����ֵΪ0����SparKey�ɹ�������ֵ��0ʱ�������룻

	*ע:
	�ڶ�SparKey������������֮ǰ,��������ִ�д˺�������ȷ������򿪼�����,�Լ�����������Ϻ�ʹ��SPARKCLOSE��
	�ܹر����ֻ������1������2��ȷ����SparKey������Զ�SparKey���ж�ȡ��������д��������ʧ��,�����ش�����
	0x2001(Try to perform by basic passwords, the required advanced passwords error),��ֻҪ����1������2��ȷ,
	����ֵ��Ϊ0��
	
	
Service 2(SPARKCLOSE): Close SparKey?

	�������ر������ڼ�����ϵ�SparKey��

	���������
		FunctionCode =SPARKCLOSE

	�������:
		��
		
	����ֵ��
	����ֵΪ0���ر�SparKey�ɹ�������ֵ��0ʱ�������룻

		
Service 3(SPARKREAD):     Read SparKey 
	
	������	��ȡSparKey�û�������
	
	���������
		FunctionCode=SPARKREAD;
		Password1=SparKey�û�����λ��;
		Password2=��ȡ���ݳ���(�ֽ�λ��λ);
	
	�������:
		IoBuffer=��Ŷ������������ݵ�ָ��;
	
	����ֵ:
	����ֵΪ0����ȡSparKey�û������ݳɹ�������ֵ��0ʱ�������룻
	�����ɹ�ʱ��*IoBuffer=	�������ݵ�����

	*ע:
	�����̿����ô˹��ܴ�ָ�����û�����ȡ���ݣ�Password1��SparKey�û�����
	λ�ã���0��ʼ��д�����ݵ���󳤶ȸ��ݲ�ͬ�汾�ļ�������ͬ��
	������ִ�����鿴����ֵ������˵������������?


Service 4(SPARKWRITE)	:	Write SparKey

	������	��SparKey�û���д����

	���������	
		FunctionCode=SPARKWRITE;
		Password1=��SparKey��λ��;
		Password2=д�����ݳ���(�ֽ�λ��λ);
		IoBuffer=Ҫд��SparKey������;
	
	�������:
		�� 
		
	����ֵ:
	����ֵΪ0��дSparKey�û������ݳɹ�������ֵ��0ʱ�������룻

	*ע:
	�����̿����ô˹������û���д������е���Ҫ���ݴ洢��SparKey���SPARKREAD��ȡ��
	Password1��SparKey�û�����λ�ã���0��ʼ��д�����ݵ���󳤶ȸ��ݲ�ͬ�汾�ļ�������ͬ��
	������ִ�����鿴����ֵ������˵��������������
	
	
Service 5(SPARKSETID)��	Set SparKey ID
	
	������	����SparKey��ID

	���������
		FunctionCode=SPARKSETID;
		TNum= Ҫ���õ�SparKey IDֵ��

	�������:
		��
		
	����ֵ:
	����ֵΪ0������ SparKey ID �ɹ�������ֵ��0ʱ�������룻


Service 6(SPARKGETID):   	Get SparKey ID
	
	������	���SparKey ��ID ֵ
	
	���������
		FunctionCode=SPARKGETID;
	�������:
		TNum=SparKey ID
		
	����ֵ:
	����ֵΪ0���õ�SparKey ID �ɹ�������ֵ��0ʱ�������룻
	

Service 7(SPARKRANDOM):  Random Number
	
	����������һ�������
	
	���������
	FunctionCode=SPARKRANDOM;
	
	�������:
		*Password1=�����������;
		
	����ֵ:
	����ֵΪ0������ �ɹ�������ֵ��0ʱ�������룻


Service 8(SPARKSEED):	GetSeed

	����������һ��ָ����DWORD�����ݣ������룩,���������ָ���������ɵ��ĸ�WORD������
	
	���������
		FunctionCode=SPARKSEED;
		TNum=������;
	
	���������
	  	Password1=�����1;
		Password2=�����2;
		Password3=�����3;
		Password4=�����4;

	����ֵ:
	����ֵΪ0������ �ɹ�������ֵ��0ʱ�������룻
	
	*ע��
	ÿ����������ͬ�������뷵�صĽ������ͬ����ͬ���ļ�������ͬ��ͬһ������ÿ���������Ӧ�Ľ���벻ͬ��
	����ܿ���������Ƿ���ָ���ļ���������һ��ֵ�������룬���ô˹��ܿ����صĽ�����Ƿ�ȫ��������ȷ��
	����룬����������ǿ�����ָ���ļ�������
	

Service 9(SPARKARITHMETIC):	Write Arithmetic

	��������SparKeyָ���Ŀ�д������д��һ����ʽ

	���������	
		FunctionCode= SPARKARITHMETIC ;
		Password1=ָ��д�㷨�����λ��;
		IoBuffer=�㷨ָ������ݴ�;
	
	�������:
		��
		
	����ֵ:
	����ֵΪ0������ �ɹ�������ֵ��0ʱ�������룻


Service 10(SPARKCALCULATE1):	Calculate1

	������	��SparKey��ģ��ŷ�ʽ��������

	���������	
	FunctionCode= SPARKCALCULATE1;
	SparKeyW=�������ʼ��ַ;
	TNum=ģ���;
	Password1=������1;
	Password2=������2;
	Password3=������3;
	Password4=������4;

	�������:
  	Password1=���1;
	Password2=���2;
	Password3=���3;
	Password4=���4;
	
	����ֵ:
	����ֵΪ0������ �ɹ�������ֵ��0ʱ�������룻

	 
Service 11(SPARKCALCULATE2):	Calculate2

	����:		��SparKey�������뷽ʽ��������

	���������
		FunctionCode= SPARKCALCULATE2
		SparKeyW=�������ʼ��ַ;
		TNum=������;
		Password1=������1;
		Password2=������2;
		Password3=������3;
		Password4=������4;
	
	�������:
	  	Password1=���1;
		Password2=���2;
		Password3=���3;
		Password4=���4;
	
	����ֵ:
	����ֵΪ0������ �ɹ�������ֵ��0ʱ�������룻
	
		 
Service 12(SPARKCALCULATE3):	Calculate3

	������	��SparKey��ģ���ַ��ʽ��������

	���������
		FunctionCode= SPARKCALCULATE3;
		SparKeyW=�������ʼ��ַ;
		TNum=ģ���ַ;
		Password1=������1;
		Password2=������2;
		Password3=������3;
		Password4=������4;
		
	�������:
	  	Password1=���1;
		Password2=���2;
		Password3=���3;
		Password4=���4;
	
	����ֵ:
	����ֵΪ0������ �ɹ�������ֵ��0ʱ�������룻
	 

Service 13(SPARKSETMODULE):	Set Module	
	
	����:		���洢��SparKey���㷨��������

	���������	
		FunctionCode=SPARKSETMODULE;
		Password1=ģ��ID;
		Password2=�û�ģ����;
		Password3=1 or 0 (1:���Եݼ�;0:�����Եݼ�)


	����ֵ:
	����ֵΪ0������ �ɹ�������ֵ��0ʱ�������룻


Service 14(SPARKCHECKMODULE): 	��ģ����
	
	����:		���ģ��������

	���������
		FunctionCode=SPARKCHECKMODULE;
		Password1=ģ��ID;
	
	�������:
		Password2=��ģ���Ƿ���Ч
		Password3=��ģ���Ƿ���Եݼ�
		
	����ֵ:
	����ֵΪ0������ �ɹ�������ֵ��0ʱ�������룻

	*ע��
		Password2==1��ģ����Ч��Password2==0 ��ģ����Ч
		Password3==1��ģ����Եݼ���Password2==0 �˲����Եݼ�


Service 15(SPARKDEC): 		Decrease
	
	������	��ָ��ģ�����ݼ�1
	
	���������
		FunctionCode=SPARKDEC;
		Password1=ģ��ID;

	�������:
		��

	����ֵ:
	����ֵΪ0������ �ɹ�������ֵ��0ʱ�������룻


Service 16(SPARKENCODE): 		EnCode
	
	������	�������ָ�����ݼ���
	
	���������
		FunctionCode=SPARKENCODE;
		IoBuffer=��Ҫ�������ݵ�ָ��;
		TNum=��Ҫ���ܵ����ݳ���;

	�������:
		IoBuffer=���ܺ������ָ��;
	
	����ֵ:
	����ֵΪ0������ �ɹ�������ֵ��0ʱ�������룻

	*ע��
	

Service 17(SPARKDECODE): 		DeCode
	
	������	�������ָ�����ܵ����ݽ���
	
	���������
		FunctionCode=SPARKDECODE;
		IoBuffer=�������ݵ�ָ��;
		*TNum=�������ݵĳ���;

	�������:
		IoBuffer=���ܺ����ݵ�ָ��;
	
	����ֵ:
	����ֵΪ0������ �ɹ�������ֵ��0ʱ�������룻
	
********************************************************/
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

	描述：打开连接在计算机上指定密码的SparKey 。

	语法:
	
	
	输入参数：
		FunctionCode =SPARKFIND;
		Password1= 密码1;
		Password2= 密码2;
		Password3= 密码3; （可选）
		Password4= 密码4; （可选）
		TNum= SparKey Type (1 ---- 并口锁;2 ----USB 锁;others ---- 默认检测)

	输出参数: 
		TNum=SparKey的硬件ID
	
	返回值：
	返回值为0：打开SparKey成功；返回值非0时，错误码；

	*注:
	在对SparKey进行其他操作之前,必须首先执行此函数以正确的密码打开加密锁,对加密所操作完毕后使用SPARKCLOSE功
	能关闭如果只有密码1和密码2正确，打开SparKey后仅可以对SparKey进行读取操作，而写操作将会失败,并返回错误码
	0x2001(Try to perform by basic passwords, the required advanced passwords error),但只要密码1和密码2正确,
	返回值就为0。
	
	
Service 2(SPARKCLOSE): Close SparKey?

	描述：关闭连接在计算机上的SparKey。

	输入参数：
		FunctionCode =SPARKCLOSE

	输出参数:
		无
		
	返回值：
	返回值为0：关闭SparKey成功；返回值非0时，错误码；

		
Service 3(SPARKREAD):     Read SparKey 
	
	描述：	读取SparKey用户区内容
	
	输入参数：
		FunctionCode=SPARKREAD;
		Password1=SparKey用户区的位置;
		Password2=读取数据长度(字节位单位);
	
	输出参数:
		IoBuffer=存放读出数据区数据的指针;
	
	返回值:
	返回值为0：读取SparKey用户区数据成功；返回值非0时，错误码；
	函数成功时，*IoBuffer=	读出数据的内容

	*注:
	开发商可以用此功能从指定的用户区读取数据，Password1（SparKey用户区的
	位置）从0开始，写入数据的最大长度根据不同版本的加密锁不同，
	如果出现错误请查看返回值错误码说明，加以修?


Service 4(SPARKWRITE)	:	Write SparKey

	描述：	向SparKey用户区写数据

	输入参数：	
		FunctionCode=SPARKWRITE;
		Password1=在SparKey的位置;
		Password2=写入数据长度(字节位单位);
		IoBuffer=要写入SparKey的数据;
	
	输出参数:
		无 
		
	返回值:
	返回值为0：写SparKey用户区数据成功；返回值非0时，错误码；

	*注:
	开发商可以用此功能向用户区写入程序中的重要数据存储到SparKey里，用SPARKREAD读取。
	Password1（SparKey用户区的位置）从0开始，写入数据的最大长度根据不同版本的加密锁不同，
	如果出现错误请查看返回值错误码说明，加以修正。
	
	
Service 5(SPARKSETID)：	Set SparKey ID
	
	描述：	设置SparKey的ID

	输入参数：
		FunctionCode=SPARKSETID;
		TNum= 要设置的SparKey ID值；

	输出参数:
		无
		
	返回值:
	返回值为0：设置 SparKey ID 成功；返回值非0时，错误码；


Service 6(SPARKGETID):   	Get SparKey ID
	
	描述：	获得SparKey 的ID 值
	
	输入参数：
		FunctionCode=SPARKGETID;
	输出参数:
		TNum=SparKey ID
		
	返回值:
	返回值为0：得到SparKey ID 成功；返回值非0时，错误码；
	

Service 7(SPARKRANDOM):  Random Number
	
	描述：产生一个随机数
	
	输入参数：
	FunctionCode=SPARKRANDOM;
	
	输出参数:
		*Password1=产生的随机数;
		
	返回值:
	返回值为0：函数 成功；返回值非0时，错误码；


Service 8(SPARKSEED):	GetSeed

	描述：输入一个指定的DWORD型数据（种子码）,返回由这个指定数据生成的四个WORD型数据
	
	输入参数：
		FunctionCode=SPARKSEED;
		TNum=种子码;
	
	输出参数：
	  	Password1=结果码1;
		Password2=结果码2;
		Password3=结果码3;
		Password4=结果码4;

	返回值:
	返回值为0：函数 成功；返回值非0时，错误码；
	
	*注：
	每批加密锁相同的种子码返回的结果码相同，不同批的加密锁则不同，同一加密锁每个种子码对应的结果码不同，
	此项功能可用来检测是否有指定的加密锁，用一定值做种子码，调用此功能看返回的结果码是否全部等于正确的
	结果码，如果不等则不是开发商指定的加密锁。
	

Service 9(SPARKARITHMETIC):	Write Arithmetic

	描述：向SparKey指定的可写数据区写入一组算式

	输入参数：	
		FunctionCode= SPARKARITHMETIC ;
		Password1=指定写算法区域的位置;
		IoBuffer=算法指令的数据串;
	
	输出参数:
		无
		
	返回值:
	返回值为0：函数 成功；返回值非0时，错误码；


Service 10(SPARKCALCULATE1):	Calculate1

	描述：	按SparKey的模块号方式进行运算

	输入参数：	
	FunctionCode= SPARKCALCULATE1;
	SparKeyW=计算的起始地址;
	TNum=模块号;
	Password1=输入数1;
	Password2=输入数2;
	Password3=输入数3;
	Password4=输入数4;

	输出参数:
  	Password1=结果1;
	Password2=结果2;
	Password3=结果3;
	Password4=结果4;
	
	返回值:
	返回值为0：函数 成功；返回值非0时，错误码；

	 
Service 11(SPARKCALCULATE2):	Calculate2

	描述:		按SparKey的种子码方式进行运算

	输入参数：
		FunctionCode= SPARKCALCULATE2
		SparKeyW=计算的起始地址;
		TNum=种子码;
		Password1=输入数1;
		Password2=输入数2;
		Password3=输入数3;
		Password4=输入数4;
	
	输出参数:
	  	Password1=结果1;
		Password2=结果2;
		Password3=结果3;
		Password4=结果4;
	
	返回值:
	返回值为0：函数 成功；返回值非0时，错误码；
	
		 
Service 12(SPARKCALCULATE3):	Calculate3

	描述：	按SparKey的模块地址方式进行运算

	输入参数：
		FunctionCode= SPARKCALCULATE3;
		SparKeyW=计算的起始地址;
		TNum=模块地址;
		Password1=输入数1;
		Password2=输入数2;
		Password3=输入数3;
		Password4=输入数4;
		
	输出参数:
	  	Password1=结果1;
		Password2=结果2;
		Password3=结果3;
		Password4=结果4;
	
	返回值:
	返回值为0：函数 成功；返回值非0时，错误码；
	 

Service 13(SPARKSETMODULE):	Set Module	
	
	描述:		按存储在SparKey的算法进行运算

	输入参数：	
		FunctionCode=SPARKSETMODULE;
		Password1=模块ID;
		Password2=用户模块字;
		Password3=1 or 0 (1:可以递减;0:不可以递减)


	返回值:
	返回值为0：函数 成功；返回值非0时，错误码；


Service 14(SPARKCHECKMODULE): 	读模块字
	
	描述:		检测模块字属性

	输入参数：
		FunctionCode=SPARKCHECKMODULE;
		Password1=模块ID;
	
	输出参数:
		Password2=此模块是否有效
		Password3=此模块是否可以递减
		
	返回值:
	返回值为0：函数 成功；返回值非0时，错误码；

	*注：
		Password2==1此模块有效，Password2==0 此模块无效
		Password3==1此模块可以递减，Password2==0 此不可以递减


Service 15(SPARKDEC): 		Decrease
	
	描述：	将指定模快内容减1
	
	输入参数：
		FunctionCode=SPARKDEC;
		Password1=模块ID;

	输出参数:
		无

	返回值:
	返回值为0：函数 成功；返回值非0时，错误码；


Service 16(SPARKENCODE): 		EnCode
	
	描述：	给传入的指定数据加密
	
	输入参数：
		FunctionCode=SPARKENCODE;
		IoBuffer=需要加密数据的指针;
		TNum=需要加密的数据长度;

	输出参数:
		IoBuffer=加密后的数据指针;
	
	返回值:
	返回值为0：函数 成功；返回值非0时，错误码；

	*注：
	

Service 17(SPARKDECODE): 		DeCode
	
	描述：	给传入的指定加密的数据解密
	
	输入参数：
		FunctionCode=SPARKDECODE;
		IoBuffer=加密数据的指针;
		*TNum=加密数据的长度;

	输出参数:
		IoBuffer=解密后数据的指针;
	
	返回值:
	返回值为0：函数 成功；返回值非0时，错误码；
	
********************************************************/
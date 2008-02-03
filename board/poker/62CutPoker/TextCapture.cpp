// TextCapture.cpp: implementation of the CTextCapture class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SPoker2.h"
#include "TextCapture.h"

#include <io.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTextCapture::CTextCapture()
{

}

CTextCapture::~CTextCapture()
{

}

BOOL CTextCapture::Save(const char *strUserID, const char *strBuf, int length, const char *fname)
{
	if(strBuf == NULL) return FALSE;
	if(length <= 0) return FALSE;
	if(fname==NULL) return FALSE;

	FILE *fp;
	fp = fopen(fname, "wb" );
	if(fp==NULL)
	{
		AfxMessageBox("파일을 생성할 수 없습니다");
		return FALSE;
	}

	CString UserID, Date, Warn, UnderBar, Notice;

	SYSTEMTIME pSysTime;
	GetSystemTime(&pSysTime);

	char szID[16]={0,};
	if(strUserID != NULL) strncpy(szID, strUserID, 15);

	UserID.Format("신고인ID : %s\n", szID);
	Date.Format("작성일시 : %4d년 %2d월 %2d일 (%2d시 %2d분 %2d초)\n", pSysTime.wYear, pSysTime.wMonth, pSysTime.wDay, pSysTime.wHour, pSysTime.wMinute, pSysTime.wSecond);
	Warn.Format("경    고 : 이 파일은 불량 아이디 신고용 대화 내용 캡쳐 파일입니다.\n아래의 밑줄 이후로 실제 대화 내용이 기록되며 데이터는 암호화 되어있으므로 \n일반 텍스트 편집기로 내용을 수정하거나 이 파일을 다른 이름 혹은 \n덮어씌우기로 저장을 하게되면 내용이 손상됩니다.\n");
	UnderBar.Format("============================================================================\n");
	Notice = UserID + Date + Warn + UnderBar;

	TEXTCAPTURE_FILEHEADER fhead;
	ZeroMemory(&fhead, sizeof(TEXTCAPTURE_FILEHEADER));
	
	wsprintf(fhead.strHeaderKey, "TEXTCAP");
	wsprintf(fhead.strNotice, "%s", Notice.operator LPCTSTR());
	wsprintf(fhead.strUserID, "%s", szID);
	fhead.year = pSysTime.wYear;
	fhead.month = pSysTime.wMonth;
	fhead.day = pSysTime.wDay;
	fhead.hour = pSysTime.wHour;
	fhead.minute = pSysTime.wMinute;
	fhead.second = pSysTime.wSecond;

	fwrite(&fhead, sizeof(TEXTCAPTURE_FILEHEADER), 1, fp);

	char *pEncBuf = (char*)malloc(length);
	if(pEncBuf == NULL) { fclose(fp); return FALSE;}

	// 암호화 인코딩을 위한 키 생성
	char key1=0;
	char key2=(fhead.year + fhead.month + fhead.day + fhead.hour + fhead.minute + fhead.second) % 256;
	char key3=(char)0xE7;
	for(int i=0; i<(int)strlen(szID); i++) key1 ^= szID[i];

	// 텍스트 암호화 인코딩
	for(i=0; i<length; i++)
	{
		pEncBuf[i] = strBuf[i]^key1^key2^key3;
	}

	fwrite(pEncBuf, length, 1, fp);

	free(pEncBuf);
	fclose(fp);

	return TRUE;
}

CString CTextCapture::Load(const char *fname)
{
	CString RtnText;
	if(fname==NULL) return RtnText;
	FILE *fp;
	fp = fopen(fname, "rb" );
	if(fp==NULL)
	{
		AfxMessageBox("파일을 읽을 수 없습니다");
		return RtnText;
	}

	int filesize = _filelength(_fileno(fp));
	int textlength = filesize - sizeof(TEXTCAPTURE_FILEHEADER);

	if(textlength <= 0) 
	{
		AfxMessageBox("파일 사이즈가 너무 작습니다");
		fclose(fp);
		return RtnText;
	}

	TEXTCAPTURE_FILEHEADER fhead;
	ZeroMemory(&fhead, sizeof(TEXTCAPTURE_FILEHEADER));

	fread(&fhead, sizeof(TEXTCAPTURE_FILEHEADER), 1, fp);

	if(strncmp(fhead.strHeaderKey, "TEXTCAP", 7) != 0)
	{
		AfxMessageBox("텍스트 캡처 파일이 아니거나 손상된 파일입니다");
		fclose(fp);
		return RtnText;
	}

	CString UserID, Date, UnderBar, ChatText, Notice;

	UserID.Format("신고인ID : %s\n", fhead.strUserID);
	Date.Format("작성일시 : %4d년 %2d월 %2d일 (%2d시 %2d분 %2d초)\n\n", fhead.year, fhead.month, fhead.day, fhead.hour, fhead.minute, fhead.second);
	UnderBar.Format("======================== 아래부터가 신고 내용입니다 ==========================\n\n");

	RtnText = UserID + Date + UnderBar;

	char *pDecBuf = (char*)malloc(textlength);
	if(pDecBuf == NULL)
	{
		AfxMessageBox("텍스트 버퍼용 메모리 할당 실패");
		fclose(fp);
		return RtnText;
	}

	fread(pDecBuf, textlength, 1, fp);

	// 암호화 디코딩을 위한 키 생성
	char key1=0;
	char key2=(fhead.year + fhead.month + fhead.day + fhead.hour + fhead.minute + fhead.second) % 256;
	char key3=(char)0xE7;
	for(int i=0; i<(int)strlen(fhead.strUserID); i++) key1 ^= fhead.strUserID[i];

	// 텍스트 암호화 디코딩
	for( i=0; i<textlength; i++)
	{
		pDecBuf[i] = pDecBuf[i]^key1^key2^key3;
	}

	ChatText.Format("%s", pDecBuf);

	RtnText += ChatText;

	UnderBar.Format("\n\n================== 아래부터는 헤더 대조를 위한 경고문입니다 ==================\n\n");
	Notice.Format("%s", fhead.strNotice);

	RtnText += UnderBar + Notice;

	free(pDecBuf);
	fclose(fp);

	return RtnText;
}

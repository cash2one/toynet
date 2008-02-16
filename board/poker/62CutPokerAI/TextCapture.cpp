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
		AfxMessageBox("������ ������ �� �����ϴ�");
		return FALSE;
	}

	CString UserID, Date, Warn, UnderBar, Notice;

	SYSTEMTIME pSysTime;
	GetSystemTime(&pSysTime);

	char szID[16]={0,};
	if(strUserID != NULL) strncpy(szID, strUserID, 15);

	UserID.Format("�Ű���ID : %s\n", szID);
	Date.Format("�ۼ��Ͻ� : %4d�� %2d�� %2d�� (%2d�� %2d�� %2d��)\n", pSysTime.wYear, pSysTime.wMonth, pSysTime.wDay, pSysTime.wHour, pSysTime.wMinute, pSysTime.wSecond);
	Warn.Format("��    �� : �� ������ �ҷ� ���̵� �Ű�� ��ȭ ���� ĸ�� �����Դϴ�.\n�Ʒ��� ���� ���ķ� ���� ��ȭ ������ ��ϵǸ� �����ʹ� ��ȣȭ �Ǿ������Ƿ� \n�Ϲ� �ؽ�Ʈ ������� ������ �����ϰų� �� ������ �ٸ� �̸� Ȥ�� \n������� ������ �ϰԵǸ� ������ �ջ�˴ϴ�.\n");
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

	// ��ȣȭ ���ڵ��� ���� Ű ����
	char key1=0;
	char key2=(fhead.year + fhead.month + fhead.day + fhead.hour + fhead.minute + fhead.second) % 256;
	char key3=(char)0xE7;
	for(int i=0; i<(int)strlen(szID); i++) key1 ^= szID[i];

	// �ؽ�Ʈ ��ȣȭ ���ڵ�
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
		AfxMessageBox("������ ���� �� �����ϴ�");
		return RtnText;
	}

	int filesize = _filelength(_fileno(fp));
	int textlength = filesize - sizeof(TEXTCAPTURE_FILEHEADER);

	if(textlength <= 0) 
	{
		AfxMessageBox("���� ����� �ʹ� �۽��ϴ�");
		fclose(fp);
		return RtnText;
	}

	TEXTCAPTURE_FILEHEADER fhead;
	ZeroMemory(&fhead, sizeof(TEXTCAPTURE_FILEHEADER));

	fread(&fhead, sizeof(TEXTCAPTURE_FILEHEADER), 1, fp);

	if(strncmp(fhead.strHeaderKey, "TEXTCAP", 7) != 0)
	{
		AfxMessageBox("�ؽ�Ʈ ĸó ������ �ƴϰų� �ջ�� �����Դϴ�");
		fclose(fp);
		return RtnText;
	}

	CString UserID, Date, UnderBar, ChatText, Notice;

	UserID.Format("�Ű���ID : %s\n", fhead.strUserID);
	Date.Format("�ۼ��Ͻ� : %4d�� %2d�� %2d�� (%2d�� %2d�� %2d��)\n\n", fhead.year, fhead.month, fhead.day, fhead.hour, fhead.minute, fhead.second);
	UnderBar.Format("======================== �Ʒ����Ͱ� �Ű� �����Դϴ� ==========================\n\n");

	RtnText = UserID + Date + UnderBar;

	char *pDecBuf = (char*)malloc(textlength);
	if(pDecBuf == NULL)
	{
		AfxMessageBox("�ؽ�Ʈ ���ۿ� �޸� �Ҵ� ����");
		fclose(fp);
		return RtnText;
	}

	fread(pDecBuf, textlength, 1, fp);

	// ��ȣȭ ���ڵ��� ���� Ű ����
	char key1=0;
	char key2=(fhead.year + fhead.month + fhead.day + fhead.hour + fhead.minute + fhead.second) % 256;
	char key3=(char)0xE7;
	for(int i=0; i<(int)strlen(fhead.strUserID); i++) key1 ^= fhead.strUserID[i];

	// �ؽ�Ʈ ��ȣȭ ���ڵ�
	for( i=0; i<textlength; i++)
	{
		pDecBuf[i] = pDecBuf[i]^key1^key2^key3;
	}

	ChatText.Format("%s", pDecBuf);

	RtnText += ChatText;

	UnderBar.Format("\n\n================== �Ʒ����ʹ� ��� ������ ���� ����Դϴ� ==================\n\n");
	Notice.Format("%s", fhead.strNotice);

	RtnText += UnderBar + Notice;

	free(pDecBuf);
	fclose(fp);

	return RtnText;
}

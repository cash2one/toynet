#ifndef __SPRITE8_H__
#define __SPRITE8_H__

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct         // ��������Ʈ�� ���� ���
{
    char  Identifier[16];      // ��������Ʈ �νĹ���
    short Version;             // ��������Ʈ ����
    short TotalSprNum;         // �� ��������Ʈ ����
	long  ActionNum;           // �׼��� ����
} SPRFILEHEAD8;
typedef SPRFILEHEAD8* LPSPRFILEHEAD8;

typedef struct         // �浹, ������ǥ ����ü
{
	short attrib;              // ��ǥ�� �Ӽ�
    short x1,y1;               // �»��� ��ǥ
	short x2,y2;               // ������ ��ǥ
	char reserved[2];
} SPRPOS8;
typedef SPRPOS8* LPSPRPOS8;

typedef struct         // �� ��������Ʈ�� ���
{
    BYTE  compress;            // ���࿩��
	short rx, ry;              // ���� �����ǥ
    short xl, yl;              // X, Y�� ����
    short mx, my;              // �߽���ǥ
	short hitNum;              // �浹��ǥ�� ����
	short atckNum;             // ������ǥ�� ����
    unsigned long dsize;       // ��������Ʈ ����Ÿ�� ������(����� �ʿ�)
	BYTE colorkey;			   // �÷�Ű
	char  reserved[2];
} SPRHEAD8;
typedef SPRHEAD8* LPSPRHEAD8;

typedef struct         // �׼� ���
{
    BYTE  id;                  // �׼� ��ȣ
	BYTE  attrib;              // �׼��� �Ӽ�
	short frameNum;            // �׼� �������� ����
} ACTIONHEAD8;
typedef ACTIONHEAD8* LPACTIONHEAD8;

typedef struct         // �� �׼� ������ ����ü
{
    short frame;               // ������ ��ȣ
	long  delay;               // ���� ������ ī��Ʈ
	short rx, ry;              // �������� �����ǥ
	short flag;                // �������� �Ӽ� �÷���
} ACTIONFRAME8;
typedef ACTIONFRAME8* LPACTIONFRAME8;

//------------------------------------------------------------------------------

typedef struct          // ��������Ʈ ����Ÿ ����ü
{
    int compress;            // ���࿩��
	BYTE colorkey;           // �÷�Ű
	int xl, yl;              // X, Y�� ����
    int rx, ry;              // �����ǥ
    int mx, my;              // �߽���ǥ
	int hitNum;              // �浹��ǥ�� ����
	int atckNum;             // ������ǥ�� ����
	unsigned long  dsize;    // ��������Ʈ ����Ÿ�� ������(����� �ʿ�)
	SPRPOS8* hit;             // �浹��ǥ ����ü�� ������
	SPRPOS8* atck;            // ������ǥ ����ü�� ������
    char*   data;            // ��������Ʈ ����Ÿ ������
} SPRITE8;


class CSprite8
{
public:
	///// ���� ���� ����
	SPRFILEHEAD8 sfhead;                  // ��������Ʈ ���� ���
	SPRITE8 *spr;                       // ��������Ʈ ���� ����ü

	///// ������, �Ҹ���
	CSprite8();
	~CSprite8();

	//// ��������Ʈ �ε�,���̺�,�����Լ�
	BOOL Load(char *name);
	void Remove();
};



#endif


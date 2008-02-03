/*============================================================================
                        ����95/98�� �ѱ۶��̺귯��

                     ������ : �� �� �� (HiTEL : 93XYZ)


            # ���ȯ�� : 16bit Color (�ػ� Free) ���ǽ�(Ȥ�� ����������)�� ���
			# ��Ʈ ������� ��������
            # �ϼ��� �ѱ� ���
=============================================================================*/
#ifndef __XHAN_H__
#define __XHAN_H__


#include <windows.h>
#include <stdio.h>
#include <process.h>
#include <malloc.h>
#include <string.h>
#include <stdarg.h>
#include <conio.h>

//#include "xCanvas.h"
#include "Graphic.h"

#define HAN_OK 1
#define HAN_FILENOTFOUND -1
#define HAN_FONTNOTFOUND -2
#define HAN_RSFNOTOPENED -3


/*------------------ ���������� ���̴� ������ �Լ��� -------------*/
#define KMAXFONT  10    //  �ѱ� ��Ʈ�� �ִ� ���� 
#define EMAXFONT  10    //  ���� ��Ʈ�� �ִ� ���� 

// ��� ȯ������ ���� ����ü
typedef struct 
{
    int multi;
    int shadow;
    int bgr;
    int color;
    int shadowcolor;
    int bgrcolor;
    int gap;
    int hanfont;
    int engfont;
} sHAN_ENVIRENT;

// �ѱ� ��Ʈ ������ ����ü
typedef struct {
    int xl, yl;            // �ѱ� ��Ʈ�� ����, ���α���
    int size;              // �ѱ� ��Ʈ �ϳ��� ������(����Ʈ) 
    char *han[2350];       // �ѱ� ��Ʈ ����
} sKFNTDATA;

// ���� ��Ʈ �����ͱ���ü
typedef struct {
    int xl, yl;            // ���� ��Ʈ�� ����, ���α���
    int size;              // ��Ʈ �ϳ��� ������(����Ʈ)
    char *english[128];    // ���� ��Ʈ ����                  
} sEFNTDATA;



class xHan
{
public:
	WORD *p2SCREEN;
	int _nx, _ny;			// ���� ���ڸ� ���� ��ǥ 
	int _han_xl, _han_yl;	// ȭ���� X,Y ����
	DWORD _han_lPitch;		// lPitch

	char sp_env_point;
	
	
	//------- �Ʒ��� 'HAN_'�� �����ϴ� �������� ����ڰ� ������ �� �ִ� ---------
	int   HAN_MULTI;            // ��Ƽ Į�� ����  (�⺻�� : �ܻ�)     
	int   HAN_SHADOW;           // �׸��� ����     (�⺻�� : ����)     
	int   HAN_BGR;              // ���� ��� ����(�⺻�� : ��¾���) 
	int   HAN_COLOR;            // ������ ����                         
	int   HAN_SHADOWCOLOR;      // �׸����� ���� : ����Ʈ = 0����      
	int   HAN_BGRCOLOR;         // ��� ���� : ����Ʈ = 0����          
	int   HAN_GAP;              // ���� ����                           
	int   HAN_KORFONT;          // ���� ������� �ѱ� ��Ʈ ��ȣ        
	int   HAN_ENGFONT;          // ���� ������� ���� ��Ʈ ��ȣ        
	//---------------------------------------------------------------------------


	sHAN_ENVIRENT env[5];

	sKFNTDATA kfnt[KMAXFONT];			// �ѱ� ��Ʈ ����Ÿ
	sEFNTDATA efnt[EMAXFONT];			// ���� ��Ʈ ����Ÿ

	static char outline_temp1[128], outline_temp2[128];	// �ƿ����� ������ �ӽ� ����

	xHan();
	~xHan();
	
	int  Init(WORD *pscreen, int xl, int yl, DWORD lPitch);
	void SetScreenMode(WORD *pscreen, int xl, int yl, DWORD lPitch);
	void Destroy();
	void Print(int x, int y, char *fmt, ...);

	// ��� ȯ�� ����
	void SetEffect(int effect, BOOL flag); //(effect: 1=��Ƽ, 2=�׸���, 3=�ܰ���)
	void ClearEffect();
	void SetFont(int kor, int eng);
	void SetTextColor(WORD color);
	void SetBkColor(WORD color);
	void SetTrans(BOOL trans);
	void SetGap(int gap);
	
	BOOL LoadKorFont(int fnum, char *hanFile, int xl, int yl);
	BOOL LoadEngFont(int fnum, char *engFile, int xl, int yl);
	void Push();
	void Pop();


	// ������ ��� �Լ���
	void put_font( int x, int y, int xl, int yl, char *buf,
               WORD *tar, int bgr, WORD bgrColor, int type, WORD color);
	void gap_fill(int x1, int y1, int x2, int y2, WORD *tar, WORD color);
	void htextxy(int hindex, WORD *tar);
	void textxy(char text, WORD *tar);
	int  GetKsIndex(BYTE fstByte,BYTE sndByte);
	void gputs( int x, int y, WORD *tar, char *st);
};

#endif

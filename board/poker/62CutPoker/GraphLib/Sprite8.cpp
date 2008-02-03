
#include "StdAfx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Sprite8.h"


CSprite8::CSprite8()
{
	ZeroMemory(&sfhead, sizeof(SPRFILEHEAD8));
	spr=NULL;
}

CSprite8::~CSprite8()
{
	Remove();
}


/*--------------------------------------------------------------------------
  �ε��� ��������Ʈ�� ����
---------------------------------------------------------------------------*/
void CSprite8::Remove()
{
    int i;
    if(spr != NULL) 
	{
		for(i=0; i < sfhead.TotalSprNum; i++)
		{
	        if( spr[i].hit != NULL ) { free(spr[i].hit); spr[i].hit = NULL; }
			if( spr[i].atck != NULL ) { free(spr[i].atck); spr[i].atck = NULL; }
			if( spr[i].data != NULL ) { free(spr[i].data); spr[i].data = NULL; }
	    }
		free(spr); spr=NULL;
		ZeroMemory(&sfhead, sizeof(SPRFILEHEAD8));
	}
}

/*--------------------------------------------------------------------------
  ��������Ʈ ������ �ε�
---------------------------------------------------------------------------*/
BOOL CSprite8::Load(char *name)
{
    FILE *fp;

    SPRFILEHEAD8 tempsfhead;     // �ӽ� ���� ���
	SPRHEAD8 shead;            // �� ��������Ʈ�� ���
	

	if( (fp = fopen( name, "rb")) == NULL ) return FALSE;
    
	fread(&tempsfhead, sizeof(SPRFILEHEAD8), 1, fp); //�⺻ ��� �ε�

    if(strncmp("93XYZ sprite",tempsfhead.Identifier,13) != 0) //��������Ʈ ������ �ƴϸ�
    {
		fclose(fp);
        return FALSE;
    }

	Remove();

	memcpy(&sfhead, &tempsfhead, sizeof(SPRFILEHEAD8));

	spr = (SPRITE8*) malloc(sizeof(SPRITE8) * sfhead.TotalSprNum);
	if(spr==NULL) return FALSE;
	ZeroMemory(spr, sizeof(SPRITE8) * sfhead.TotalSprNum);

    for(int i=0; i < sfhead.TotalSprNum; i++)
    {
		fread(&shead, sizeof(SPRHEAD8), 1, fp); // �� ��������Ʈ ������ ��� �ε�
		
		spr[i].compress = shead.compress;
		spr[i].colorkey = shead.colorkey;
        spr[i].rx       = shead.rx;
        spr[i].ry       = shead.ry;
        spr[i].xl       = shead.xl;
        spr[i].yl       = shead.yl;
        spr[i].mx       = shead.mx;
        spr[i].my       = shead.my;
        spr[i].hitNum   = shead.hitNum;
        spr[i].atckNum  = shead.atckNum;
        spr[i].hit      = NULL;
        spr[i].atck     = NULL;
        spr[i].dsize    = shead.dsize;

		///// �浹 ��ǥ, ���� ��ǥ �б� /////
		if(spr[i].hitNum>0) spr[i].hit = (SPRPOS8*)malloc(sizeof(SPRPOS8)*spr[i].hitNum);
		if(spr[i].atckNum>0) spr[i].atck = (SPRPOS8*)malloc(sizeof(SPRPOS8)*spr[i].atckNum);
		fread(spr[i].hit, sizeof(SPRPOS8)*spr[i].hitNum, 1, fp);
		fread(spr[i].atck, sizeof(SPRPOS8)*spr[i].atckNum, 1, fp);

		///// ��������Ʈ �̹��� ����Ÿ �б�
        spr[i].data = (char *)malloc( shead.dsize );
		if(spr[i].data==NULL) return FALSE;
        ZeroMemory(spr[i].data, shead.dsize);
		fread(spr[i].data, shead.dsize, 1, fp);
    }

	fclose(fp);

    return TRUE;
}

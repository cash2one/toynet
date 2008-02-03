
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
  로딩된 스프라이트를 삭제
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
  스프라이트 파일을 로드
---------------------------------------------------------------------------*/
BOOL CSprite8::Load(char *name)
{
    FILE *fp;

    SPRFILEHEAD8 tempsfhead;     // 임시 파일 헤더
	SPRHEAD8 shead;            // 각 스프라이트의 헤더
	

	if( (fp = fopen( name, "rb")) == NULL ) return FALSE;
    
	fread(&tempsfhead, sizeof(SPRFILEHEAD8), 1, fp); //기본 헤더 로드

    if(strncmp("93XYZ sprite",tempsfhead.Identifier,13) != 0) //스프라이트 파일이 아니면
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
		fread(&shead, sizeof(SPRHEAD8), 1, fp); // 각 스프라이트 마다의 헤더 로드
		
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

		///// 충돌 좌표, 공격 좌표 읽기 /////
		if(spr[i].hitNum>0) spr[i].hit = (SPRPOS8*)malloc(sizeof(SPRPOS8)*spr[i].hitNum);
		if(spr[i].atckNum>0) spr[i].atck = (SPRPOS8*)malloc(sizeof(SPRPOS8)*spr[i].atckNum);
		fread(spr[i].hit, sizeof(SPRPOS8)*spr[i].hitNum, 1, fp);
		fread(spr[i].atck, sizeof(SPRPOS8)*spr[i].atckNum, 1, fp);

		///// 스프라이트 이미지 데이타 읽기
        spr[i].data = (char *)malloc( shead.dsize );
		if(spr[i].data==NULL) return FALSE;
        ZeroMemory(spr[i].data, shead.dsize);
		fread(spr[i].data, shead.dsize, 1, fp);
    }

	fclose(fp);

    return TRUE;
}

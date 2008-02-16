// SevenCard.cpp: implementation of the CSevenCard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SevenCard.h"

//버그를잡아라



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define VAL7 ( 10000000000000 )
#define VAL6 ( 100000000000 )
#define VAL5 ( 1000000000 )
#define VAL4 ( 10000000 )
#define VAL3 ( 100000 )
#define VAL2 ( 1000 )
#define VAL1 ( 10 )

// 세븐카드 클래스이다.
// 주어진 일곱장의 카드를 분석하여, 순위를 리턴한다.
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSevenCard::CSevenCard()
{
	for(int i=0; i<TOTAL_CARD; i++){ // ###[8포커]###
		N[i] = 0;
		P[i] = 0;
	}
}

CSevenCard::~CSevenCard()
{

}

// 7장이하의 카드를 주어 족보를 리턴한다.
// 족보의 값을 계산한다.
int CSevenCard::GetValue(int pCard[], int nNum, int *pVal)
{
	if(pCard==NULL) return 0;

	int i=0;
	for(i=0; i<TOTAL_CARD; i++){ N[i] = 0; P[i] = 0; }//먼저 카드데이타를 클리어하고..
	int  nResult = 0; int nFlushPic =-1;
	int  nSpade=0,    nHeart=0,    nDia=0,    nClov=0;
	int n2=0,n3=0,n4=0,n5=0,n6=0,n7=0,n8=0,n9=0,n10=0,n11=0,n12=0,n13=0,n14=0;
	for(i=0; i<nNum; i++)
	{
		int nCard = pCard[i];
		if(nCard == 0 || nCard == 13 || nCard == 26 || nCard == 39) 
			N[i] = 14;
		else
			N[i] = nCard - 13*(nCard/13)+1;

		if(0<=nCard && nCard<13) 
			P[i] = 0;//클로바.
		else if(13<=nCard && nCard<26) 
			P[i] = 1;//하트.
		else if(26<=nCard && nCard<39) 
			P[i] = 2;//다이아.
		else if(39<=nCard && nCard<52) 
			P[i] = 3;//스페이드.

		if(nCard==2) n2++;        else if(nCard==3) n3++;   else if(nCard==4) n4++;
		else if(nCard==5) n5++;   else if(nCard==6) n6++;   else if(nCard==7) n7++;
		else if(nCard==8) n8++;   else if(nCard==9) n9++;   else if(nCard==10) n10++;
		else if(nCard==11) n11++; else if(nCard==12) n12++; else if(nCard==13) n13++;
		else if(nCard==14) n14++;

		if(P[i]==3) nSpade++;       else if(P[i]==2) nDia++;
		else if(P[i]==1) nHeart++;  else if(P[i]==0) nClov++;
		if(nSpade>=5){ nFlushPic = 3; }
		if(nDia>=5)  { nFlushPic = 2; }
		if(nHeart>=5){ nFlushPic = 1; }
		if(nClov>=5) { nFlushPic = 0; }
	}
	BubbleSort(N,P,nNum);

		
	

	//*****************  메인결과를 분석한다. *****************//
	// 먼저 스트레이트인가?
	int nStraight = 0;
	if(IsStraight(N,nNum,&nStraight)){//일단 스트레이트.
		if(nStraight==14)      nResult = N7C_MOUNT;//마운틴
		else if(nStraight==15) nResult = N7C_BACKS;//백스트레이트
		else{
			*pVal   = nStraight;
			nResult = N7C_STRAIGHT; //걍스트레이트.
		}

		if(nFlushPic>=0){//플러시, 또는 스트레이트플러시가 예상된다.
			int nSRes = 0; BOOL bRoyal = FALSE;
			if(IsStraightFlush(N,P,nNum,nFlushPic,&nSRes,&bRoyal)){
				if(bRoyal){//로얄스트레이트 플러시!!!
					if(nFlushPic==3){ nResult = N7C_SROYAL; return nResult; }
					if(nFlushPic==2){ nResult = N7C_DROYAL; return nResult; }
 					if(nFlushPic==1){ nResult = N7C_HROYAL; return nResult; }
					if(nFlushPic==0){ nResult = N7C_CROYAL; return nResult; }
				}
				else{//스트레이트 플러시!!!
					if(nSRes==15){ nResult = N7C_BSFLUSH; return nResult; }//백스트레이트 플러시
					else {
						*pVal = nSRes; nResult = N7C_SFULSH; return nResult; 
					}
				}
			} else{
				int nFlush = GetFlushVal(N, P, nNum, nFlushPic);
				*pVal = nFlush; nResult = N7C_FLUSH;
				return nResult; //그렇지않다면  스트레이트보다 우선인 플러시다.
			}
		}else return nResult;     //그렇지않다면  스트레이트다.
	}

	//플러시인가 확인한다.
	else if(nFlushPic>=0){//플러시다.(스트레이트는 신경안써도 된다.)
		int nFlush = GetFlushVal(N, P, nNum, nFlushPic);
		*pVal = nFlush; nResult = N7C_FLUSH; return nResult;
	}
	//그렇지 않다면 페어를 분석한다.
	else{
		int resV[3],resN[3],nPairTotal =0;
		for(int i=0;i<3;i++){
			resV[i] = 0;
			resN[i] = 0;
		}
		
		// 페어가 있다.
		if(IsPair(N,resV,resN,&nPairTotal)){
			if(nPairTotal==1){//포카드,트리플,원페어예상.
				int nVal = resV[0];
				int nPairNum = resN[0];
				//포카드.
				if(nPairNum==4){ *pVal=nVal; nResult=N7C_QUADS ;  return nResult; }
				//트리플.
				else if(nPairNum==3){ *pVal=nVal; nResult=N7C_TRIPLE;  return nResult; }
				//원페어.
				else if(nPairNum==2){ *pVal=nVal; nResult=N7C_ONEPAIR; return nResult; }
			}
			else if(nPairTotal==2){//포카드,풀하우스,투페어예상.
				int nVal0 = resV[0];
				int nVal1 = resV[1];
				int nPairNum0 = resN[0];
				int nPairNum1 = resN[1];
				//포카드.
				if(nPairNum0==4){ *pVal=nVal0; nResult=N7C_QUADS ;  return nResult; }
				if(nPairNum1==4){ *pVal=nVal1; nResult=N7C_QUADS ;  return nResult; }
				//풀하우스.
				if(nPairNum1==3){ 
					*pVal=nVal1*100 + nVal0; 
					nResult=N7C_TITLE ;  return nResult; }
				if(nPairNum0==3){ 
					*pVal=nVal0*100 + nVal1; 
					nResult=N7C_TITLE ;  return nResult; }
				//투페어
				if(nPairNum1==2){ 
					*pVal=nVal1*100 + nVal0;
					nResult=N7C_TWOPAIR ;  return nResult; 
				}
			}
			else if(nPairTotal==3){//풀하우스,투페어예상.
				int nVal0 = resV[0]; int nVal1 = resV[1]; int nVal2 = resV[2];
				int nPairNum0 = resN[0]; int nPairNum1 = resN[1]; int nPairNum2 = resN[2];
				//풀하우스.
				if(nPairNum2==3){ 
					*pVal=nVal2*100 + nVal1; 
					nResult=N7C_TITLE ;  return nResult; }
				if(nPairNum1==3){ 
					*pVal=nVal1*100 + nVal2; 
					nResult=N7C_TITLE ;  return nResult; }
				if(nPairNum0==3){ 
					*pVal=nVal0*100 + nVal2; 
					nResult=N7C_TITLE ;  return nResult; }
				//투페어.
				if(nPairNum2==2){ 
					*pVal=nVal2*100 + nVal1; 
					nResult=N7C_TWOPAIR ;  return nResult; 
				}
			}
		}
		//페어가 없다(망통이다)
		else
		{
			*pVal = N[nNum-1]; 
			nResult   = N7C_NOPAIR;  
			return nResult;
		}
	}
	return nResult;
}

// 정렬된 카드의 같은 수중 큰갯수를 리턴한다.
int CSevenCard::SameNum(int a[], int *pStart,int n/*=7*/)
{

	if(a==NULL) return 0;

	int nTotal = 1;
	for(int i=0; i<n; i++)
	{
		int nTemp =1;
		int nPre  = a[i];
		for(int j=i+1;j<n;j++)
		{
			if(a[j]==nPre && a[j]!=0)
				nTemp++;
		}

		if(nTotal<=nTemp)
		{
			*pStart = i;
			nTotal = nTemp;
		}
	}
	return nTotal;
}

// 정렬된 카드의 페어를 찾아내고, 패어의 갯수를 리턴한다.
// 7장의 카드를 분석하며, 최대 3개를 찾아낼 수 있으며, 0과 1이면 아닌걸로 간주한다.
BOOL CSevenCard::IsPair(int a[],int resV[],int resN[],int *pNum)
{
	if(a==NULL || resV==NULL || resN==NULL) return FALSE;

	BOOL bPair = FALSE;
	int nIndex = 0;
	for(int i=0; i<TOTAL_CARD; i++)
	{
		int nTemp =1;
		int nPre  = a[i];
		for(int j=i+1;j<TOTAL_CARD;j++)
		{
			if(a[j]==nPre && a[j]!=0)
				nTemp++;
		}
		
		if(nTemp>1)
		{//페어가 있다.
			resV[nIndex] = nPre;
			resN[nIndex] = nTemp;
			nIndex++;
			i+=(nTemp-1);
			*pNum = nIndex;
			bPair = TRUE;
		}
	}
	return bPair;
}

//정렬된 배열의 스트레이트 여부를 판단한다.
//스트레이트라면 족보와 그값을 구한다.
BOOL CSevenCard::IsStraight(int a[], int n, int *pVal)
{
	if(n<5) return FALSE; //스트레이트는 5이상이어야 한다.
	
	if(n==5)
	{//다섯장의 카드에선 가장큰수의 그림이 세컨드다.
		if(a[0]+1==a[1] && a[1]+1==a[2] && a[2]+1==a[3] && a[3]+1==a[4])
		{
			*pVal = a[4]; 
			return TRUE;
		} 
		else if(a[0]==2 && a[1]==3 && a[2]==4 && a[3]==5 && a[4]==14)
		{//백스트레이트
			*pVal = 15  ; 
			return TRUE;
		} 
		else 
			return FALSE;
	}
	else if(n==6)
	{
		int nStart = 0;
		int nSame = SameNum(a,&nStart,n);
		if(nSame>2) return FALSE;
		else if(nSame==2)
		{//중간에 같은것이 있을경우.
			int b[5] = { 0,};
			int nS=0;
			for(int i=0;i<n;i++)
			{
				if(nS < 7) // 추가
				{
					b[i] = a[nS];
				}
				else
				{
					break;
				}
				nS++;
				if(nStart==i) nS++;
			}
			
			if(b[0]==2 && b[1]==3 && b[2]==4 && b[3]==5 && b[4]==14)
			{//백스트레이트
				*pVal=15; return TRUE;
			}
			else if(b[0]+1==b[1] && b[1]+1==b[2] && b[2]+1==b[3] && b[3]+1==b[4])
			{
				*pVal=b[4]; return TRUE;
			} 
			else return FALSE;

		}
		else
		{//모두 각패라면..
			if(a[0]==2 && a[1]==3 && a[2]==4 && a[3]==5 && a[5]==14) //?????
			{//백스트레이트
				*pVal=15; return TRUE;
			} 
			else if(a[1]+1==a[2] && a[2]+1==a[3] && a[3]+1==a[4] && a[4]+1==a[5])
			{
				*pVal=a[5]; return TRUE;
			} 
			else if(a[0]+1==a[1] && a[1]+1==a[2] && a[2]+1==a[3] && a[3]+1==a[4])
			{
				*pVal=a[4]; return TRUE;
			} 
			else return FALSE;
		}
	}
	else if(n==7)
	{
		int nStart = 0;
		int nSame = SameNum(a,&nStart,n);
		if(nSame>3) return FALSE;
		else if(nSame==3)
		{//중간에 같은것이 3개 있을경우.
			int b[5] = { 0,};
			int nS=0;
			for(int i=0;i<n;i++)//7
			{
				if(nS < 7) // 추가
					b[i] = a[nS];
				else
				{
					break;
				}

				nS++;
				if(nStart==i) nS+=2;
			}
			if(b[0]==2 && b[1]==3 && b[2]==4 && b[3]==5 && b[4]==14)
			{//백스트레이트
				*pVal=15; return TRUE;
			}
			else if(b[0]+1==b[1] && b[1]+1==b[2] && b[2]+1==b[3] && b[3]+1==b[4])
			{
				*pVal=b[4]; return TRUE;
			} 
			else return FALSE;

		}
		else if(nSame==2)
		{//중간에 같은것이 2개 있을경우.
			int resV[3] = { 0,};
			int resN[3] = { 0,};
			int nPairTotal = 0;
			
			IsPair(a,resV,resN,&nPairTotal);
			//쓰리페어? 스트레이트는 아니다.
			if(nPairTotal==3) return FALSE;
			//투페어일경우 생각한다.
			else if(nPairTotal==2)
			{
				int b[5]= { 0,};
				int nS=0;
				for(int i=0;i<n;i++)
				{
					if(nS < 7) // 추가
						b[i] = a[nS];
					else
					{
						break;
					}


					nS++;
					if(resV[0]==b[i] || resV[1]==b[i]) nS++;
				}
				if(b[0]==2 && b[1]==3 && b[2]==4 && b[3]==5 && b[4]==14)
				{//백스트레이트
					*pVal=15; return TRUE;
				}
				else if(b[0]+1==b[1] && b[1]+1==b[2] && b[2]+1==b[3] && b[3]+1==b[4])
				{
					*pVal=b[4]; return TRUE;
				} 
				else return FALSE;
			}
			//원페어일경우 (6장)생각한다.
			else if(nPairTotal==1)
			{
				int b[6]= { 0,};
				int nS=0;
				for(int i=0;i<n;i++)
				{
					if(nS < 7) // 추가
						b[i] = a[nS];
					else
					{
						break;
					}

					nS++;
					if(resV[0]==b[i]) nS++;
				}
				int nStart1 = 0;
				int nSame1 = SameNum(b,&nStart1,6);
				if(nSame1==2)
				{//중간에 같은것이 있을경우.
					int d[5] = { 0,};
					int nS=0;
					for(int i=0;i<n;i++)
					{
						if(nS<7)
							d[i] = b[nS];
						else
						{
							break;
						}

						nS++;
						if(nStart1==i) nS++;
					}
					
					if(d[0]==2 && d[1]==3 && d[2]==4 && d[3]==5 && d[4]==14)
					{
						*pVal=15; return TRUE;
					}
					else if(d[0]+1==d[1] && d[1]+1==d[2] && d[2]+1==d[3] && d[3]+1==d[4])
					{
						*pVal=d[4]; return TRUE;
					} 
					else return FALSE;

				}
				else
				{//모두 각패라면..
					if(b[0]==2 && b[1]==3 && b[2]==4 && b[3]==5 && b[5]==14)
					{
						*pVal = 15; return TRUE;
					} 
					else if(b[1]+1==b[2] && b[2]+1==b[3] && b[3]+1==b[4] && b[4]+1==b[5])
					{
						*pVal = b[5]; return TRUE;
					} 
					else if(b[0]+1==b[1] && b[1]+1==b[2] && b[2]+1==b[3] && b[3]+1==b[4])
					{
						*pVal = b[4]; return TRUE;
					} 
					else return FALSE;
				}
			}
		}
		else
		{//모두 각패라면..
			if(a[0]==2 && a[1]==3 && a[2]==4 && a[3]==5 && a[6]==14)
			{
				*pVal=15; return TRUE;
			} 
			else if(a[2]+1==a[3] && a[3]+1==a[4] && a[4]+1==a[5] && a[5]+1==a[6])
			{
				*pVal=a[6]; return TRUE;
			} 
			else if(a[1]+1==a[2] && a[2]+1==a[3] && a[3]+1==a[4] && a[4]+1==a[5])
			{
				*pVal=a[5]; return TRUE;
			} 
			else if(a[0]+1==a[1] && a[1]+1==a[2] && a[2]+1==a[3] && a[3]+1==a[4])
			{
				*pVal=a[4]; return TRUE;
			} 
			else return FALSE;
		}
	}
	else if(n == 8)
	{
		int nStart = 0;
		int nSame = SameNum(a,&nStart,n);
		if(nSame>3) return FALSE;
		else if(nSame==3)
		{//중간에 같은것이 3개 있을경우.
			int b[5] = { 0,};
			int nS=0;
			for(int i=0;i<n;i++)//7
			{
				if(nS < 7) // 추가
					b[i] = a[nS];
				else
				{
					break;
				}

				nS++;
				if(nStart==i) nS+=2;
			}
			if(b[0]==2 && b[1]==3 && b[2]==4 && b[3]==5 && b[4]==14)
			{//백스트레이트
				*pVal=15; return TRUE;
			}
			else if(b[0]+1==b[1] && b[1]+1==b[2] && b[2]+1==b[3] && b[3]+1==b[4])
			{
				*pVal=b[4]; return TRUE;
			} 
			else return FALSE;

		}
		else if(nSame==2)
		{//중간에 같은것이 2개 있을경우.
			int resV[3] = { 0,};
			int resN[3] = { 0,};
			int nPairTotal = 0;
			
			IsPair(a,resV,resN,&nPairTotal);
			//쓰리페어? 스트레이트는 아니다.
			if(nPairTotal==3) return FALSE;
			//투페어일경우 생각한다.
			else if(nPairTotal==2)
			{
				int b[5]= { 0,};
				int nS=0;
				for(int i=0;i<n;i++)
				{
					if(nS < 7) // 추가
						b[i] = a[nS];
					else
					{
						break;
					}


					nS++;
					if(resV[0]==b[i] || resV[1]==b[i]) nS++;
				}
				if(b[0]==2 && b[1]==3 && b[2]==4 && b[3]==5 && b[4]==14)
				{//백스트레이트
					*pVal=15; return TRUE;
				}
				else if(b[0]+1==b[1] && b[1]+1==b[2] && b[2]+1==b[3] && b[3]+1==b[4])
				{
					*pVal=b[4]; return TRUE;
				} 
				else return FALSE;
			}
			//원페어일경우 (6장)생각한다.
			else if(nPairTotal==1)
			{
				int b[6]= { 0,};
				int nS=0;
				for(int i=0;i<n;i++)
				{
					if(nS < 7) // 추가
						b[i] = a[nS];
					else
					{
						break;
					}

					nS++;
					if(resV[0]==b[i]) nS++;
				}
				int nStart1 = 0;
				int nSame1 = SameNum(b,&nStart1,6);
				if(nSame1==2)
				{//중간에 같은것이 있을경우.
					int d[5] = { 0,};
					int nS=0;
					for(int i=0;i<n;i++)
					{
						if(nS<7)
							d[i] = b[nS];
						else
						{
							break;
						}

						nS++;
						if(nStart1==i) nS++;
					}
					
					if(d[0]==2 && d[1]==3 && d[2]==4 && d[3]==5 && d[4]==14)
					{
						*pVal=15; return TRUE;
					}
					else if(d[0]+1==d[1] && d[1]+1==d[2] && d[2]+1==d[3] && d[3]+1==d[4])
					{
						*pVal=d[4]; return TRUE;
					} 
					else return FALSE;

				}
				else
				{//모두 각패라면..
					if(b[0]==2 && b[1]==3 && b[2]==4 && b[3]==5 && b[5]==14)
					{
						*pVal = 15; return TRUE;
					} 
					else if(b[1]+1==b[2] && b[2]+1==b[3] && b[3]+1==b[4] && b[4]+1==b[5])
					{
						*pVal = b[5]; return TRUE;
					} 
					else if(b[0]+1==b[1] && b[1]+1==b[2] && b[2]+1==b[3] && b[3]+1==b[4])
					{
						*pVal = b[4]; return TRUE;
					} 
					else return FALSE;
				}
			}
		}
		else
		{//모두 각패라면..
			if(a[0]==2 && a[1]==3 && a[2]==4 && a[3]==5 && a[6]==14)
			{
				*pVal=15; return TRUE;
			} 
			else if(a[2]+1==a[3] && a[3]+1==a[4] && a[4]+1==a[5] && a[5]+1==a[6])
			{
				*pVal=a[6]; return TRUE;
			} 
			else if(a[1]+1==a[2] && a[2]+1==a[3] && a[3]+1==a[4] && a[4]+1==a[5])
			{
				*pVal=a[5]; return TRUE;
			} 
			else if(a[0]+1==a[1] && a[1]+1==a[2] && a[2]+1==a[3] && a[3]+1==a[4])
			{
				*pVal=a[4]; return TRUE;
			} 
			else return FALSE;
		}
	}

	return FALSE;
}

//스트레이트 플러시인가?
//플러시가 예상되는 스트레이트인 카드를 주어 플러시인지 확인한다.
//여기서 로얄 스트레이트 플러시임을 안다.
BOOL CSevenCard::IsStraightFlush(int a[], int b[], int n, int nPic, int *pRes,BOOL *pRoyal)
{
	if(n==5)
	{
		if(b[0]==b[1] && b[1]==b[2] && b[2]==b[3] && b[3]==b[4])
		{
			*pRes = a[4];
			if(a[4]==14 && a[3]==13)
				 *pRoyal = TRUE;
			else 
				*pRoyal = FALSE;
			return TRUE;
		} 
		else return FALSE;
	}
	else
	{
		int c[7]= {0,};
		int nS=0;
		for(int i=0; i<n; i++)
		{
			if(b[i]==nPic)
			{
				c[nS]=a[i];
				nS++;
			}
		}
		
		int nStraight=0;
		//for(int k=0;k<3;k++) nStraight=0;
		if(IsStraight(c,nS,&nStraight))
		{//같은무늬의 배열들이 스트레이트인가?
			*pRes = nStraight;
			int nRoyal=0;
			for(int j=0;j<nS;j++)
			{
				if(c[j]==10 || c[j]==11 || c[j]==12 || c[j]==13 || c[j]==14)
					nRoyal++;
			}
			
			if(nRoyal==5)
				*pRoyal = TRUE;
			else 
				*pRoyal = FALSE;
			return TRUE;
		}
		else return FALSE;
	}
	return FALSE;
}

//(스트레이트가아닌)플러시 카드의 값을 결정한다. 정수형 리턴.
int CSevenCard::GetFlushVal(int a[], int b[], int n, int nPic)
{
	if(n<5) return 0; //플러시는 5이상이어야한다.
	int nNum=0;
	for(int i=0; i<n; i++)
		if(b[i]==nPic)
			nNum = a[i];
	return nNum;
}

// 플래시카드인 7장의 카드에서 플레시카드의 무늬를 구한다.
int CSevenCard::GetFlushPic(int pic[])
{
	
	if(pic == NULL) return 0;

	int nFlushPic = -1;
	int nSpade=0, nHeart=0, nDia=0, nClov=0;
	for(int i=0; i<TOTAL_CARD; i++)
	{
		if(pic[i]==3)      nSpade++;  else if(pic[i]==2) nDia++;
		else if(pic[i]==1) nHeart++;  else if(pic[i]==0) nClov++;
		if(nSpade>=5){ nFlushPic = 3; }
		if(nDia>=5)  { nFlushPic = 2; }
		if(nHeart>=5){ nFlushPic = 1; }
		if(nClov>=5) { nFlushPic = 0; }
	}
	return nFlushPic;
}

//(스트레이트가아닌)플러시 카드의 값을 결정한다. 64비트정수형 리턴.
//다섯자리를 작성한다. 마지막에 단자리 무늬값을 더한다.
//VAL5 + VAL4 + VAL3 + VAL2 + VAL1 + 무늬(단자리)
INT64 CSevenCard::GetFlushNum(int pCard[], int n)
{
	if(pCard == NULL) return 0;

	if(n<5 || n>TOTAL_CARD) return 0; //플러시는 5이상이어야한다.
	int a[TOTAL_CARD],b[TOTAL_CARD],i=0;
	for(i=0;i<TOTAL_CARD;i++){ a[i] = 0; b[i] = 0; }
	for(i=0; i<n; i++)
	{
		int nCard = pCard[i];
		if(nCard == 0 || nCard == 13 || nCard == 26 || nCard == 39) 
			a[i] = 14;
		else 
			a[i] = nCard - 13*(nCard/13)+1;

		if(0<=nCard && nCard<13)       b[i] = 0;//클로바.
		else if(13<=nCard && nCard<26) b[i] = 1;//하트.
		else if(26<=nCard && nCard<39) b[i] = 2;//다이아.
		else if(39<=nCard && nCard<52) b[i] = 3;//스페이드.
	}
	int nPic = GetFlushPic(b);
	BubbleSort(a,b,n);

	INT64 lNum=VAL5,nVal=0;
	for(i=n-1; i>=0; i--)
	{
		if(b[i]==nPic){
			nVal += lNum*a[i];
			lNum = (INT64)(lNum/100);
		}
	}
	nVal += nPic;//단자리 무늬값
	return nVal;

}

// 정렬된 7장의 스트레이트카드를 주어 잔여카드의 값을 계산한다.
// 잔여카드의 족보는 무시하고 높은 수로 계산한다.
INT64 CSevenCard::GetStraightNum(int pCard[], int nVal)
{
	if(pCard == NULL) return 0;

	int a[TOTAL_CARD],b[TOTAL_CARD],i=0;
	for(i=0;i<TOTAL_CARD;i++){ a[i] = 0; b[i] = 0; }
	for(i=0; i<TOTAL_CARD; i++){
		int nCard = pCard[i];
		if(nCard == 0 || nCard == 13 || nCard == 26 || nCard == 39) 
			a[i] = 14;
		else 
			a[i] = nCard - 13*(nCard/13)+1;

		if(0<=nCard && nCard<13)       b[i] = 0;//클로바.
		else if(13<=nCard && nCard<26) b[i] = 1;//하트.
		else if(26<=nCard && nCard<39) b[i] = 2;//다이아.
		else if(39<=nCard && nCard<52) b[i] = 3;//스페이드.
	}
	BubbleSort(a,b,TOTAL_CARD);

	int nPic=0;
	INT64 lNum=VAL7,nRes=0;
	for(i=TOTAL_CARD-1; i>=0; i--)
	{
		nRes += lNum*a[i];
		lNum = (INT64)(lNum/100);
		if(a[i]==nVal)
			nPic = b[i];
	}
	nRes += nPic;//단자리 무늬값
	return nRes;
}


// 노페어,원페어,투페어가 같을경우 페어값을 구한다.
INT64 CSevenCard::GetPairNum(int pCard[], int n)
{
	
	if(pCard == NULL) return 0;

	INT64 lNum=0,nRes=0;
	int a[TOTAL_CARD],b[TOTAL_CARD],i=0;
	for(i=0;i<TOTAL_CARD;i++){ a[i] = 0; b[i] = 0; }
	for(i=0; i<n; i++)
	{
		int nCard = pCard[i];
		if(nCard == 0 || nCard == 13 || nCard == 26 || nCard == 39) 
			a[i] = 14;
		else 
			a[i] = nCard - 13*(nCard/13)+1;

		if(0<=nCard && nCard<13)       b[i] = 0;//클로바.
		else if(13<=nCard && nCard<26) b[i] = 1;//하트.
		else if(26<=nCard && nCard<39) b[i] = 2;//다이아.
		else if(39<=nCard && nCard<52) b[i] = 3;//스페이드.
	}
	BubbleSort(a,b,n);
	//if(n==TOTAL_CARD) lNum=VAL8;
	if(n==7) lNum=VAL7;
	if(n==6) lNum=VAL6;
	if(n==5) lNum=VAL5;
	if(n==4) lNum=VAL4;
	if(n==3) lNum=VAL3;
	if(n==2) lNum=VAL2;

	for(i=n-1; i>=0; i--)
	{
		nRes += lNum*a[i];
		lNum = (INT64)(lNum/100);
	}
	nRes += b[n-1];//단자리 무늬값

	return nRes;
}

// 세컨드 값을 비교한다.
INT64 CSevenCard::GetSecondVal(int pCard[], int n, enum SEVEN_CARD eCard, int nVal)
{
	
	if(pCard == NULL) return 0;

	INT64 nResult = 0;
	//스트레이트 플러시,스트레이트 처리.
	if(eCard==N7C_BSFLUSH || eCard==N7C_SFULSH ||
	   eCard==N7C_MOUNT || eCard==N7C_BACKS || eCard==N7C_STRAIGHT)
	{
		nResult = GetStraightNum(pCard,nVal);
	}
	//플러시처리
	else if(eCard==N7C_FLUSH)
	{
		nResult = GetFlushNum(pCard,n);
	}
	//페어처리
	else if(eCard==N7C_NOPAIR || eCard==N7C_ONEPAIR || eCard==N7C_TWOPAIR)
	{
		nResult = GetPairNum(pCard,n);
	}
	else
		nResult = 0;
	return nResult;
}

//카드를 오름차순으로 정렬한다.
int CSevenCard::BubbleSort(int a[], int b[], int n)
{
	if(a==NULL || b==NULL) return 0;

	int i=0, j=0, temp=0,pic=0;
	for(i = 0; i < n-1; i++){
		for(j = i+1; j < n; j++){
			if(a[i] > a[j]){
				temp = a[i];//카드 정렬하구...
				a[i] = a[j];
				a[j] = temp;

				pic  = b[i];//무늬는 따라간다.
				b[i] = b[j];
				b[j] = pic;
			}
			else if(a[i] == a[j]){ //숫자가 같다면,
				if(b[i] > b[j]){
					int t=0;
					t    = b[i];//무늬 정렬한다....
					b[i] = b[j];
					b[j] = t;
				}
			}
		}
	}
	return 0;
}

// 최종 결산물이다.선 및 승리자를 구한다.
// 족보와 함께 계산된 족보값, 그리고 세컨드 값, 사용자 순서를 주어 정렬한다.
// 2명이상이어야 유효하다.
// 유니크한 승리자인 pUser[]의 값을 리턴한다.
int CSevenCard::GetWinner(int pCardVal[], INT64 pSecVal[], int pUser[], int n)
{
	if(pCardVal == NULL || pSecVal == NULL || pUser == NULL) return 0;

	int i=0, j=0, tVal=0,tUser=0;
	int a[10];
	int user[10]; //최대 7명이지만 여분으로 둔다.
	INT64 b[10];
	INT64 tSec=0;
	for(i=0;i<10;i++){ a[i] = 0; b[i] = 0; user[i] = 0; }//일단 초기화하고..
	
	for(i=0;i<n; i++)
	{ 
		a[i]    = pCardVal[i]; 
		b[i]    = pSecVal[i]; 
		user[i] = pUser[i]; 
	}
	
	for(i = 0; i < n-1; i++)
	{
		for(j = i+1; j < n; j++)
		{
			if(a[i] > a[j])
			{
				tVal = a[i];//카드값 정렬하구...
				a[i] = a[j];
				a[j] = tVal;

				tSec  = b[i];//세컨드와 사용자는 따라간다.
				b[i]  = b[j];
				b[j]  = tSec;

				tUser    = user[i];
				user[i]  = user[j];
				user[j]  = tUser;
			}
			else if(a[i] == a[j])
			{ //메인카드값이 같다면,
				if(b[i] > b[j])
				{
					int   u=0;
					INT64 t=0;

					t    = b[i];// 세컨드 정렬한다....
					b[i] = b[j];
					b[j] = t;
								
					u       = user[i];// 사용자 따라간다.
					user[i] = user[j];
					user[j] = u;
				}
			}
		}
	}
	return user[n-1]; // 최종승자이다.
}

// 족보와 점수를 주어, 카드 종류를 문자열로 리턴한다.
CString CSevenCard::GetName(enum SEVEN_CARD eName, int nVal, int *nResult)
{
	CString strName="", strVal="", strReturn="";
	*nResult = 0;
	if(eName==N7C_SROYAL)
	{
		strName = "로얄 스트레이트 플러시";
		*nResult = 3;
	}
	else if(eName==N7C_DROYAL)
	{
		strName = "다이아 로티플";
		*nResult = 3;
	}
	else if(eName==N7C_HROYAL)
	{
		strName = "하트 로티플";
		*nResult = 3;
	}
	else if(eName==N7C_CROYAL)
	{
		strName = "크로바 로티플";
		*nResult = 3;
	}
	else if(eName==N7C_BSFLUSH)
	{
		strName = "백스트레이트 플러시";
		*nResult = 2;
	}
	else if(eName==N7C_SFULSH)
	{ 
		*nResult = 2;
		strName = "스트레이트 플러쉬";
		if(nVal==14)
			strVal="Ace";
		else if(nVal==13) strVal="K";
		else if(nVal==12) strVal="Q";
		else if(nVal==11) strVal="J";
		else
			strVal.Format("%d",nVal);
	}
	else if(eName==N7C_QUADS)
	{
		*nResult = 1;
		strName = "포카드";
		if(nVal==14)
			strVal="Ace";
		else if(nVal==13) strVal="K";
		else if(nVal==12) strVal="Q";
		else if(nVal==11) strVal="J";
		else
			strVal.Format("%d",nVal);
	}
	else if(eName==N7C_TITLE)
	{
		strName = "집(풀하우스)";
		int nV1 = (int)nVal/100;
		int nV2 = (int)nVal%100;
		CString s1="",s2="";
		if(nV1==14)      s1 = "Ace";
		else if(nV1==13) s1="K";
		else if(nV1==12) s1="Q";
		else if(nV1==11) s1="J";
		else s1.Format("%d",nV1);

		if(nV2==14)      s2 = "Ace";
		else if(nV2==13) s2="K";
		else if(nV2==12) s2="Q";
		else if(nV2==11) s2="J";
		else s2.Format("%d",nV2);

		strVal.Format("%s,%s",s1,s2);
	}
	else if(eName==N7C_FLUSH)
	{
		strName = "플러시";
		if(nVal==14)
			strVal="Ace";
		else if(nVal==13) strVal="K";
		else if(nVal==12) strVal="Q";
		else if(nVal==11) strVal="J";
		else
			strVal.Format("%d",nVal);
	}
	else if(eName==N7C_MOUNT) strName = "마운틴(스트레이트)";
	else if(eName==N7C_BACKS) strName = "백스트레이트";
	else if(eName==N7C_STRAIGHT)
	{
		strName = "스트레이트";
		if(nVal==14)
			strVal="Ace";
		else if(nVal==13) strVal="K";
		else if(nVal==12) strVal="Q";
		else if(nVal==11) strVal="J";
		else
			strVal.Format("%d",nVal);
	}
	else if(eName==N7C_TRIPLE)
	{
		strName = "봉(쓰리카드)";
		if(nVal==14)
			strVal="Ace";
		else if(nVal==13) strVal="K";
		else if(nVal==12) strVal="Q";
		else if(nVal==11) strVal="J";
		else
			strVal.Format("%d",nVal);
	}
	else if(eName==N7C_TWOPAIR)
	{
		strName = "투페어";
		int nV1 = (int)nVal/100;
		int nV2 = (int)nVal%100;

		CString s1="",s2="";
		if(nV1==14)      s1 = "Ace";
		else if(nV1==13) s1="K";
		else if(nV1==12) s1="Q";
		else if(nV1==11) s1="J";
		else s1.Format("%d",nV1);

		if(nV2==14)      s2 = "Ace";
		else if(nV2==13) s2="K";
		else if(nV2==12) s2="Q";
		else if(nV2==11) s2="J";
		else s2.Format("%d",nV2);

		strVal.Format("%s,%s",s1,s2);
	}
	else if(eName==N7C_ONEPAIR)
	{
		strName = "원페어";
		if(nVal==14)
			strVal="Ace";
		else if(nVal==13) strVal="K";
		else if(nVal==12) strVal="Q";
		else if(nVal==11) strVal="J";
		else
			strVal.Format("%d",nVal);
	}
	else if(eName==N7C_NOPAIR)
	{
		strName = "";
		if(nVal==14)
			strVal="Ace";
		else if(nVal==13) strVal="K";
		else if(nVal==12) strVal="Q";
		else if(nVal==11) strVal="J";
		else
			strVal.Format("%d",nVal);
	}
	if(strVal=="")
		strReturn=strName;
	else
		strReturn.Format("%s %s",strVal,strName);
	return strReturn;
}

// 특별한 값인지 확인하자
BOOL CSevenCard::IsSpecial(enum SEVEN_CARD eName,int &nVal)
{
	nVal = 0;
	if(eName==N7C_SROYAL)
	{ // 로얄 스트레이트 플러시
		nVal = 3;
		return TRUE;
	}
	else if(eName==N7C_DROYAL)
	{ // 다이아 로티플
		nVal = 3;
		return TRUE;
	}
	else if(eName==N7C_HROYAL)
	{ // 하트 로티플
		nVal = 3;
		return TRUE;
	}
	else if(eName==N7C_CROYAL)
	{ // 크로바 로티플
		nVal = 3;
		return TRUE;
	}
	else if(eName==N7C_BSFLUSH)
	{ // 백스트레이트 플러시
		nVal = 2;
		return TRUE;
	}
	else if(eName==N7C_SFULSH)
	{ // 스트레이트 플러쉬
		nVal = 2;
		return TRUE;
	}
	else if(eName==N7C_QUADS)
	{ // 포카드
		nVal = 1;
		return TRUE;
	}
	return FALSE;
}


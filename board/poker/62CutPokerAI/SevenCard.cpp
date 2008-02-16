// SevenCard.cpp: implementation of the CSevenCard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SevenCard.h"

//���׸���ƶ�



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

// ����ī�� Ŭ�����̴�.
// �־��� �ϰ����� ī�带 �м��Ͽ�, ������ �����Ѵ�.
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSevenCard::CSevenCard()
{
	for(int i=0; i<TOTAL_CARD; i++){ // ###[8��Ŀ]###
		N[i] = 0;
		P[i] = 0;
	}
}

CSevenCard::~CSevenCard()
{

}

// 7�������� ī�带 �־� ������ �����Ѵ�.
// ������ ���� ����Ѵ�.
int CSevenCard::GetValue(int pCard[], int nNum, int *pVal)
{
	if(pCard==NULL) return 0;

	int i=0;
	for(i=0; i<TOTAL_CARD; i++){ N[i] = 0; P[i] = 0; }//���� ī�嵥��Ÿ�� Ŭ�����ϰ�..
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
			P[i] = 0;//Ŭ�ι�.
		else if(13<=nCard && nCard<26) 
			P[i] = 1;//��Ʈ.
		else if(26<=nCard && nCard<39) 
			P[i] = 2;//���̾�.
		else if(39<=nCard && nCard<52) 
			P[i] = 3;//�����̵�.

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

		
	

	//*****************  ���ΰ���� �м��Ѵ�. *****************//
	// ���� ��Ʈ����Ʈ�ΰ�?
	int nStraight = 0;
	if(IsStraight(N,nNum,&nStraight)){//�ϴ� ��Ʈ����Ʈ.
		if(nStraight==14)      nResult = N7C_MOUNT;//����ƾ
		else if(nStraight==15) nResult = N7C_BACKS;//�齺Ʈ����Ʈ
		else{
			*pVal   = nStraight;
			nResult = N7C_STRAIGHT; //����Ʈ����Ʈ.
		}

		if(nFlushPic>=0){//�÷���, �Ǵ� ��Ʈ����Ʈ�÷��ð� ����ȴ�.
			int nSRes = 0; BOOL bRoyal = FALSE;
			if(IsStraightFlush(N,P,nNum,nFlushPic,&nSRes,&bRoyal)){
				if(bRoyal){//�ξ⽺Ʈ����Ʈ �÷���!!!
					if(nFlushPic==3){ nResult = N7C_SROYAL; return nResult; }
					if(nFlushPic==2){ nResult = N7C_DROYAL; return nResult; }
 					if(nFlushPic==1){ nResult = N7C_HROYAL; return nResult; }
					if(nFlushPic==0){ nResult = N7C_CROYAL; return nResult; }
				}
				else{//��Ʈ����Ʈ �÷���!!!
					if(nSRes==15){ nResult = N7C_BSFLUSH; return nResult; }//�齺Ʈ����Ʈ �÷���
					else {
						*pVal = nSRes; nResult = N7C_SFULSH; return nResult; 
					}
				}
			} else{
				int nFlush = GetFlushVal(N, P, nNum, nFlushPic);
				*pVal = nFlush; nResult = N7C_FLUSH;
				return nResult; //�׷����ʴٸ�  ��Ʈ����Ʈ���� �켱�� �÷��ô�.
			}
		}else return nResult;     //�׷����ʴٸ�  ��Ʈ����Ʈ��.
	}

	//�÷����ΰ� Ȯ���Ѵ�.
	else if(nFlushPic>=0){//�÷��ô�.(��Ʈ����Ʈ�� �Ű�Ƚᵵ �ȴ�.)
		int nFlush = GetFlushVal(N, P, nNum, nFlushPic);
		*pVal = nFlush; nResult = N7C_FLUSH; return nResult;
	}
	//�׷��� �ʴٸ� �� �м��Ѵ�.
	else{
		int resV[3],resN[3],nPairTotal =0;
		for(int i=0;i<3;i++){
			resV[i] = 0;
			resN[i] = 0;
		}
		
		// �� �ִ�.
		if(IsPair(N,resV,resN,&nPairTotal)){
			if(nPairTotal==1){//��ī��,Ʈ����,������.
				int nVal = resV[0];
				int nPairNum = resN[0];
				//��ī��.
				if(nPairNum==4){ *pVal=nVal; nResult=N7C_QUADS ;  return nResult; }
				//Ʈ����.
				else if(nPairNum==3){ *pVal=nVal; nResult=N7C_TRIPLE;  return nResult; }
				//�����.
				else if(nPairNum==2){ *pVal=nVal; nResult=N7C_ONEPAIR; return nResult; }
			}
			else if(nPairTotal==2){//��ī��,Ǯ�Ͽ콺,������.
				int nVal0 = resV[0];
				int nVal1 = resV[1];
				int nPairNum0 = resN[0];
				int nPairNum1 = resN[1];
				//��ī��.
				if(nPairNum0==4){ *pVal=nVal0; nResult=N7C_QUADS ;  return nResult; }
				if(nPairNum1==4){ *pVal=nVal1; nResult=N7C_QUADS ;  return nResult; }
				//Ǯ�Ͽ콺.
				if(nPairNum1==3){ 
					*pVal=nVal1*100 + nVal0; 
					nResult=N7C_TITLE ;  return nResult; }
				if(nPairNum0==3){ 
					*pVal=nVal0*100 + nVal1; 
					nResult=N7C_TITLE ;  return nResult; }
				//�����
				if(nPairNum1==2){ 
					*pVal=nVal1*100 + nVal0;
					nResult=N7C_TWOPAIR ;  return nResult; 
				}
			}
			else if(nPairTotal==3){//Ǯ�Ͽ콺,������.
				int nVal0 = resV[0]; int nVal1 = resV[1]; int nVal2 = resV[2];
				int nPairNum0 = resN[0]; int nPairNum1 = resN[1]; int nPairNum2 = resN[2];
				//Ǯ�Ͽ콺.
				if(nPairNum2==3){ 
					*pVal=nVal2*100 + nVal1; 
					nResult=N7C_TITLE ;  return nResult; }
				if(nPairNum1==3){ 
					*pVal=nVal1*100 + nVal2; 
					nResult=N7C_TITLE ;  return nResult; }
				if(nPairNum0==3){ 
					*pVal=nVal0*100 + nVal2; 
					nResult=N7C_TITLE ;  return nResult; }
				//�����.
				if(nPairNum2==2){ 
					*pVal=nVal2*100 + nVal1; 
					nResult=N7C_TWOPAIR ;  return nResult; 
				}
			}
		}
		//�� ����(�����̴�)
		else
		{
			*pVal = N[nNum-1]; 
			nResult   = N7C_NOPAIR;  
			return nResult;
		}
	}
	return nResult;
}

// ���ĵ� ī���� ���� ���� ū������ �����Ѵ�.
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

// ���ĵ� ī���� �� ã�Ƴ���, �о��� ������ �����Ѵ�.
// 7���� ī�带 �м��ϸ�, �ִ� 3���� ã�Ƴ� �� ������, 0�� 1�̸� �ƴѰɷ� �����Ѵ�.
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
		{//�� �ִ�.
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

//���ĵ� �迭�� ��Ʈ����Ʈ ���θ� �Ǵ��Ѵ�.
//��Ʈ����Ʈ��� ������ �װ��� ���Ѵ�.
BOOL CSevenCard::IsStraight(int a[], int n, int *pVal)
{
	if(n<5) return FALSE; //��Ʈ����Ʈ�� 5�̻��̾�� �Ѵ�.
	
	if(n==5)
	{//�ټ����� ī�忡�� ����ū���� �׸��� �������.
		if(a[0]+1==a[1] && a[1]+1==a[2] && a[2]+1==a[3] && a[3]+1==a[4])
		{
			*pVal = a[4]; 
			return TRUE;
		} 
		else if(a[0]==2 && a[1]==3 && a[2]==4 && a[3]==5 && a[4]==14)
		{//�齺Ʈ����Ʈ
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
		{//�߰��� �������� �������.
			int b[5] = { 0,};
			int nS=0;
			for(int i=0;i<n;i++)
			{
				if(nS < 7) // �߰�
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
			{//�齺Ʈ����Ʈ
				*pVal=15; return TRUE;
			}
			else if(b[0]+1==b[1] && b[1]+1==b[2] && b[2]+1==b[3] && b[3]+1==b[4])
			{
				*pVal=b[4]; return TRUE;
			} 
			else return FALSE;

		}
		else
		{//��� ���ж��..
			if(a[0]==2 && a[1]==3 && a[2]==4 && a[3]==5 && a[5]==14) //?????
			{//�齺Ʈ����Ʈ
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
		{//�߰��� �������� 3�� �������.
			int b[5] = { 0,};
			int nS=0;
			for(int i=0;i<n;i++)//7
			{
				if(nS < 7) // �߰�
					b[i] = a[nS];
				else
				{
					break;
				}

				nS++;
				if(nStart==i) nS+=2;
			}
			if(b[0]==2 && b[1]==3 && b[2]==4 && b[3]==5 && b[4]==14)
			{//�齺Ʈ����Ʈ
				*pVal=15; return TRUE;
			}
			else if(b[0]+1==b[1] && b[1]+1==b[2] && b[2]+1==b[3] && b[3]+1==b[4])
			{
				*pVal=b[4]; return TRUE;
			} 
			else return FALSE;

		}
		else if(nSame==2)
		{//�߰��� �������� 2�� �������.
			int resV[3] = { 0,};
			int resN[3] = { 0,};
			int nPairTotal = 0;
			
			IsPair(a,resV,resN,&nPairTotal);
			//�������? ��Ʈ����Ʈ�� �ƴϴ�.
			if(nPairTotal==3) return FALSE;
			//������ϰ�� �����Ѵ�.
			else if(nPairTotal==2)
			{
				int b[5]= { 0,};
				int nS=0;
				for(int i=0;i<n;i++)
				{
					if(nS < 7) // �߰�
						b[i] = a[nS];
					else
					{
						break;
					}


					nS++;
					if(resV[0]==b[i] || resV[1]==b[i]) nS++;
				}
				if(b[0]==2 && b[1]==3 && b[2]==4 && b[3]==5 && b[4]==14)
				{//�齺Ʈ����Ʈ
					*pVal=15; return TRUE;
				}
				else if(b[0]+1==b[1] && b[1]+1==b[2] && b[2]+1==b[3] && b[3]+1==b[4])
				{
					*pVal=b[4]; return TRUE;
				} 
				else return FALSE;
			}
			//������ϰ�� (6��)�����Ѵ�.
			else if(nPairTotal==1)
			{
				int b[6]= { 0,};
				int nS=0;
				for(int i=0;i<n;i++)
				{
					if(nS < 7) // �߰�
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
				{//�߰��� �������� �������.
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
				{//��� ���ж��..
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
		{//��� ���ж��..
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
		{//�߰��� �������� 3�� �������.
			int b[5] = { 0,};
			int nS=0;
			for(int i=0;i<n;i++)//7
			{
				if(nS < 7) // �߰�
					b[i] = a[nS];
				else
				{
					break;
				}

				nS++;
				if(nStart==i) nS+=2;
			}
			if(b[0]==2 && b[1]==3 && b[2]==4 && b[3]==5 && b[4]==14)
			{//�齺Ʈ����Ʈ
				*pVal=15; return TRUE;
			}
			else if(b[0]+1==b[1] && b[1]+1==b[2] && b[2]+1==b[3] && b[3]+1==b[4])
			{
				*pVal=b[4]; return TRUE;
			} 
			else return FALSE;

		}
		else if(nSame==2)
		{//�߰��� �������� 2�� �������.
			int resV[3] = { 0,};
			int resN[3] = { 0,};
			int nPairTotal = 0;
			
			IsPair(a,resV,resN,&nPairTotal);
			//�������? ��Ʈ����Ʈ�� �ƴϴ�.
			if(nPairTotal==3) return FALSE;
			//������ϰ�� �����Ѵ�.
			else if(nPairTotal==2)
			{
				int b[5]= { 0,};
				int nS=0;
				for(int i=0;i<n;i++)
				{
					if(nS < 7) // �߰�
						b[i] = a[nS];
					else
					{
						break;
					}


					nS++;
					if(resV[0]==b[i] || resV[1]==b[i]) nS++;
				}
				if(b[0]==2 && b[1]==3 && b[2]==4 && b[3]==5 && b[4]==14)
				{//�齺Ʈ����Ʈ
					*pVal=15; return TRUE;
				}
				else if(b[0]+1==b[1] && b[1]+1==b[2] && b[2]+1==b[3] && b[3]+1==b[4])
				{
					*pVal=b[4]; return TRUE;
				} 
				else return FALSE;
			}
			//������ϰ�� (6��)�����Ѵ�.
			else if(nPairTotal==1)
			{
				int b[6]= { 0,};
				int nS=0;
				for(int i=0;i<n;i++)
				{
					if(nS < 7) // �߰�
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
				{//�߰��� �������� �������.
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
				{//��� ���ж��..
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
		{//��� ���ж��..
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

//��Ʈ����Ʈ �÷����ΰ�?
//�÷��ð� ����Ǵ� ��Ʈ����Ʈ�� ī�带 �־� �÷������� Ȯ���Ѵ�.
//���⼭ �ξ� ��Ʈ����Ʈ �÷������� �ȴ�.
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
		{//���������� �迭���� ��Ʈ����Ʈ�ΰ�?
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

//(��Ʈ����Ʈ���ƴ�)�÷��� ī���� ���� �����Ѵ�. ������ ����.
int CSevenCard::GetFlushVal(int a[], int b[], int n, int nPic)
{
	if(n<5) return 0; //�÷��ô� 5�̻��̾���Ѵ�.
	int nNum=0;
	for(int i=0; i<n; i++)
		if(b[i]==nPic)
			nNum = a[i];
	return nNum;
}

// �÷���ī���� 7���� ī�忡�� �÷���ī���� ���̸� ���Ѵ�.
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

//(��Ʈ����Ʈ���ƴ�)�÷��� ī���� ���� �����Ѵ�. 64��Ʈ������ ����.
//�ټ��ڸ��� �ۼ��Ѵ�. �������� ���ڸ� ���̰��� ���Ѵ�.
//VAL5 + VAL4 + VAL3 + VAL2 + VAL1 + ����(���ڸ�)
INT64 CSevenCard::GetFlushNum(int pCard[], int n)
{
	if(pCard == NULL) return 0;

	if(n<5 || n>TOTAL_CARD) return 0; //�÷��ô� 5�̻��̾���Ѵ�.
	int a[TOTAL_CARD],b[TOTAL_CARD],i=0;
	for(i=0;i<TOTAL_CARD;i++){ a[i] = 0; b[i] = 0; }
	for(i=0; i<n; i++)
	{
		int nCard = pCard[i];
		if(nCard == 0 || nCard == 13 || nCard == 26 || nCard == 39) 
			a[i] = 14;
		else 
			a[i] = nCard - 13*(nCard/13)+1;

		if(0<=nCard && nCard<13)       b[i] = 0;//Ŭ�ι�.
		else if(13<=nCard && nCard<26) b[i] = 1;//��Ʈ.
		else if(26<=nCard && nCard<39) b[i] = 2;//���̾�.
		else if(39<=nCard && nCard<52) b[i] = 3;//�����̵�.
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
	nVal += nPic;//���ڸ� ���̰�
	return nVal;

}

// ���ĵ� 7���� ��Ʈ����Ʈī�带 �־� �ܿ�ī���� ���� ����Ѵ�.
// �ܿ�ī���� ������ �����ϰ� ���� ���� ����Ѵ�.
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

		if(0<=nCard && nCard<13)       b[i] = 0;//Ŭ�ι�.
		else if(13<=nCard && nCard<26) b[i] = 1;//��Ʈ.
		else if(26<=nCard && nCard<39) b[i] = 2;//���̾�.
		else if(39<=nCard && nCard<52) b[i] = 3;//�����̵�.
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
	nRes += nPic;//���ڸ� ���̰�
	return nRes;
}


// �����,�����,���� ������� ���� ���Ѵ�.
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

		if(0<=nCard && nCard<13)       b[i] = 0;//Ŭ�ι�.
		else if(13<=nCard && nCard<26) b[i] = 1;//��Ʈ.
		else if(26<=nCard && nCard<39) b[i] = 2;//���̾�.
		else if(39<=nCard && nCard<52) b[i] = 3;//�����̵�.
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
	nRes += b[n-1];//���ڸ� ���̰�

	return nRes;
}

// ������ ���� ���Ѵ�.
INT64 CSevenCard::GetSecondVal(int pCard[], int n, enum SEVEN_CARD eCard, int nVal)
{
	
	if(pCard == NULL) return 0;

	INT64 nResult = 0;
	//��Ʈ����Ʈ �÷���,��Ʈ����Ʈ ó��.
	if(eCard==N7C_BSFLUSH || eCard==N7C_SFULSH ||
	   eCard==N7C_MOUNT || eCard==N7C_BACKS || eCard==N7C_STRAIGHT)
	{
		nResult = GetStraightNum(pCard,nVal);
	}
	//�÷���ó��
	else if(eCard==N7C_FLUSH)
	{
		nResult = GetFlushNum(pCard,n);
	}
	//���ó��
	else if(eCard==N7C_NOPAIR || eCard==N7C_ONEPAIR || eCard==N7C_TWOPAIR)
	{
		nResult = GetPairNum(pCard,n);
	}
	else
		nResult = 0;
	return nResult;
}

//ī�带 ������������ �����Ѵ�.
int CSevenCard::BubbleSort(int a[], int b[], int n)
{
	if(a==NULL || b==NULL) return 0;

	int i=0, j=0, temp=0,pic=0;
	for(i = 0; i < n-1; i++){
		for(j = i+1; j < n; j++){
			if(a[i] > a[j]){
				temp = a[i];//ī�� �����ϱ�...
				a[i] = a[j];
				a[j] = temp;

				pic  = b[i];//���̴� ���󰣴�.
				b[i] = b[j];
				b[j] = pic;
			}
			else if(a[i] == a[j]){ //���ڰ� ���ٸ�,
				if(b[i] > b[j]){
					int t=0;
					t    = b[i];//���� �����Ѵ�....
					b[i] = b[j];
					b[j] = t;
				}
			}
		}
	}
	return 0;
}

// ���� ��깰�̴�.�� �� �¸��ڸ� ���Ѵ�.
// ������ �Բ� ���� ������, �׸��� ������ ��, ����� ������ �־� �����Ѵ�.
// 2���̻��̾�� ��ȿ�ϴ�.
// ����ũ�� �¸����� pUser[]�� ���� �����Ѵ�.
int CSevenCard::GetWinner(int pCardVal[], INT64 pSecVal[], int pUser[], int n)
{
	if(pCardVal == NULL || pSecVal == NULL || pUser == NULL) return 0;

	int i=0, j=0, tVal=0,tUser=0;
	int a[10];
	int user[10]; //�ִ� 7�������� �������� �д�.
	INT64 b[10];
	INT64 tSec=0;
	for(i=0;i<10;i++){ a[i] = 0; b[i] = 0; user[i] = 0; }//�ϴ� �ʱ�ȭ�ϰ�..
	
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
				tVal = a[i];//ī�尪 �����ϱ�...
				a[i] = a[j];
				a[j] = tVal;

				tSec  = b[i];//������� ����ڴ� ���󰣴�.
				b[i]  = b[j];
				b[j]  = tSec;

				tUser    = user[i];
				user[i]  = user[j];
				user[j]  = tUser;
			}
			else if(a[i] == a[j])
			{ //����ī�尪�� ���ٸ�,
				if(b[i] > b[j])
				{
					int   u=0;
					INT64 t=0;

					t    = b[i];// ������ �����Ѵ�....
					b[i] = b[j];
					b[j] = t;
								
					u       = user[i];// ����� ���󰣴�.
					user[i] = user[j];
					user[j] = u;
				}
			}
		}
	}
	return user[n-1]; // ���������̴�.
}

// ������ ������ �־�, ī�� ������ ���ڿ��� �����Ѵ�.
CString CSevenCard::GetName(enum SEVEN_CARD eName, int nVal, int *nResult)
{
	CString strName="", strVal="", strReturn="";
	*nResult = 0;
	if(eName==N7C_SROYAL)
	{
		strName = "�ξ� ��Ʈ����Ʈ �÷���";
		*nResult = 3;
	}
	else if(eName==N7C_DROYAL)
	{
		strName = "���̾� ��Ƽ��";
		*nResult = 3;
	}
	else if(eName==N7C_HROYAL)
	{
		strName = "��Ʈ ��Ƽ��";
		*nResult = 3;
	}
	else if(eName==N7C_CROYAL)
	{
		strName = "ũ�ι� ��Ƽ��";
		*nResult = 3;
	}
	else if(eName==N7C_BSFLUSH)
	{
		strName = "�齺Ʈ����Ʈ �÷���";
		*nResult = 2;
	}
	else if(eName==N7C_SFULSH)
	{ 
		*nResult = 2;
		strName = "��Ʈ����Ʈ �÷���";
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
		strName = "��ī��";
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
		strName = "��(Ǯ�Ͽ콺)";
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
		strName = "�÷���";
		if(nVal==14)
			strVal="Ace";
		else if(nVal==13) strVal="K";
		else if(nVal==12) strVal="Q";
		else if(nVal==11) strVal="J";
		else
			strVal.Format("%d",nVal);
	}
	else if(eName==N7C_MOUNT) strName = "����ƾ(��Ʈ����Ʈ)";
	else if(eName==N7C_BACKS) strName = "�齺Ʈ����Ʈ";
	else if(eName==N7C_STRAIGHT)
	{
		strName = "��Ʈ����Ʈ";
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
		strName = "��(����ī��)";
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
		strName = "�����";
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
		strName = "�����";
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

// Ư���� ������ Ȯ������
BOOL CSevenCard::IsSpecial(enum SEVEN_CARD eName,int &nVal)
{
	nVal = 0;
	if(eName==N7C_SROYAL)
	{ // �ξ� ��Ʈ����Ʈ �÷���
		nVal = 3;
		return TRUE;
	}
	else if(eName==N7C_DROYAL)
	{ // ���̾� ��Ƽ��
		nVal = 3;
		return TRUE;
	}
	else if(eName==N7C_HROYAL)
	{ // ��Ʈ ��Ƽ��
		nVal = 3;
		return TRUE;
	}
	else if(eName==N7C_CROYAL)
	{ // ũ�ι� ��Ƽ��
		nVal = 3;
		return TRUE;
	}
	else if(eName==N7C_BSFLUSH)
	{ // �齺Ʈ����Ʈ �÷���
		nVal = 2;
		return TRUE;
	}
	else if(eName==N7C_SFULSH)
	{ // ��Ʈ����Ʈ �÷���
		nVal = 2;
		return TRUE;
	}
	else if(eName==N7C_QUADS)
	{ // ��ī��
		nVal = 1;
		return TRUE;
	}
	return FALSE;
}


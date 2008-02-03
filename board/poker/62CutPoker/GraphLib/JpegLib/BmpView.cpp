// BmpView.cpp : implementation file
//

#include "stdafx.h"
#include "BmpView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBmpView

IMPLEMENT_DYNCREATE(CBmpView, CView)

CBmpView::CBmpView()
{
}

CBmpView::~CBmpView()
{
}


BEGIN_MESSAGE_MAP(CBmpView, CView)
	//{{AFX_MSG_MAP(CBmpView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBmpView drawing

void CBmpView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CBmpView diagnostics

#ifdef _DEBUG
void CBmpView::AssertValid() const
{
	CView::AssertValid();
}

void CBmpView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

// BMP에서 배경을 투명하게 출력하기 위해서는 페인트나 포토샾등의 그림편집기를 이용해서
// 투명해야되는 부분을 한가지의 색(아무것이나 관계없다)으로 색칠한 다음 그색의 RGB값을 
// 기억하고 있다가 아래의 함수를 호출하면 된다. GIF89a도 같은 원리로 투명한 그림을 만든다.
//    다만 화일 속에 투명한 그림 색상값이 내장되어 있다는 점이 BMP와 다르다.
//    의문 사항이 있으면 gom99@intz.com 이나 webmaster@net-shopping.co.kr로 메일 주세요.

BOOL CBmpView::ShowBmp(CDC *pdc, int x, int y, CString filename, int maskcolor)
{
	//////////////////////////< [1] 유효성 검사 >////////////////////////////////////

	// 이미지를 읽는다.
    HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),filename,
	          IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	if( hBitmap == NULL ) {  //  실패이면 
		return FALSE;
	}


	//////////////////< [2] 전처리 작업 >////////////////////////

    // 1) 매크로 정의
	#define SRCMASK						0x00220326    // 아래에서 설명

	// 2) 읽은 이미지에 대한 정보를 가져와 bm에 저장 (폭과 높이가 필요)
	BITMAP  bm;   
	(CBitmap::FromHandle(hBitmap))->GetBitmap(&bm);
    CRect  r(0,0,bm.bmWidth,bm.bmHeight);

    // 3) buff 생성(bitmap이미지를 가질 것임)    
	CDC		 buffDC ;
			 buffDC.CreateCompatibleDC( pdc ) ;
	CBitmap  buffBitmap ;
			 buffBitmap.CreateCompatibleBitmap( pdc, bm.bmWidth, bm.bmHeight ) ;
	CBitmap* oldbuffBitmap = buffDC.SelectObject( &buffBitmap ) ;

	// 4) mask 생성(bitmap이미지에서 원하는 부분만을 추출하기위한 ) 
	CDC		 maskDC ;
			 maskDC.CreateCompatibleDC( pdc ) ;
             // 모두가 검정(0)인 흑백Bitmap을 만듬 
	CBitmap  maskBitmap ;  
			 maskBitmap.CreateBitmap( bm.bmWidth, bm.bmHeight, 1, 1, NULL ) ;
	CBitmap* oldmaskBitmap = maskDC.SelectObject( &maskBitmap ) ;


	////////////////< [3] buffDC(그림Source)와 maskDC 만들기 >/////////////////////////

    // 1) buffDC를 transparent color (maskcolor)로 채운다.
    buffDC.FillSolidRect( &r, maskcolor) ;

    // 2)임시 memory 생성(DC와 bitmap)    bitmap은 hBitmap의 핸들을 얻어와 만듬
	CDC		 memoryDC ;
			 memoryDC.CreateCompatibleDC( pdc ) ;
	CBitmap* oldmemoryBitmap = memoryDC.SelectObject( CBitmap::FromHandle( hBitmap ) ) ;
    // 3) memoryDC의 내용을 buffDC로 복사
	buffDC.BitBlt( 0, 0, bm.bmWidth, bm.bmHeight, &memoryDC, 0, 0, SRCCOPY ) ;

    // 4) 그려질 칼라마스크를 포함하는 AND마스크를 만듬
    //   바탕----흰(1), 이미지부분----검정(0)
	// buffDC의 내용을 maskDC로 복사(이때 원하는 형태의 mask비트맵이 탄생)
	maskDC.BitBlt( 0, 0, bm.bmWidth, bm.bmHeight, &buffDC, 0, 0, SRCCOPY ) ;


	/////////////////////////< [4] 그림편집 >////////////////////////////////
	/// copyDC는 현재의 그림이 뿌려질 부분을 가지고와서 최종 편집된 것을 복사한 후 
	///          pdc로 되돌려질 것임.

	// 1) copyDC 만듬.
	CDC		 copyDC ;        // 현재 DC의 복사본 
			 copyDC.CreateCompatibleDC( pdc ) ;
	CBitmap  copyBitmap ;    // 사이즈는 현재 그림만큼만   
			 copyBitmap.CreateCompatibleBitmap( pdc, bm.bmWidth, bm.bmHeight ) ;
	CBitmap* oldcopyBitmap = copyDC.SelectObject( &copyBitmap ) ;

	// 2) 나타낼 지점(x,y)에서 사이즈만큼 가져와 copyDC에 저장(원래의 배경만 있다)
	copyDC.BitBlt(   0, 0, bm.bmWidth, bm.bmHeight, pdc,  x, y, SRCCOPY  ) ;

	// 3) maskDC와 SRCAND연산하여 copyDC를 수정 (검정배경에 제대로된 그림) 
	copyDC.BitBlt(   0, 0, bm.bmWidth, bm.bmHeight, &maskDC,  0, 0, SRCAND  ) ;
	// 4) 소스Bitmap의 buffDC는 SRCMASK연산(마스크를NOT연산한것과 다시 AND연산)해둔다.
	buffDC.BitBlt(   0, 0, bm.bmWidth, bm.bmHeight, &maskDC,  0, 0, SRCMASK  ) ;
	// 5) 소스(buffDC)와 SCRPRINT연산하여 최종 copyDC만듬(원래의 배경에 제대로된 그림)
	copyDC.BitBlt( 0, 0, bm.bmWidth, bm.bmHeight, &buffDC,   0, 0, SRCPAINT  ) ;
	// 6) 만들어진 copyDC를 원래 DC의 원래위치(x,y)에 복원
    pdc->BitBlt(x,y, bm.bmWidth, bm.bmHeight, &copyDC,   0, 0, SRCCOPY  ) ;

	//////////////////////////< [5] 해제 >////////////////////////////////////
	copyDC.SelectObject( oldcopyBitmap ) ;
	memoryDC.SelectObject( oldmemoryBitmap ) ;
	maskDC.SelectObject( oldmaskBitmap ) ;
	buffDC.SelectObject( oldbuffBitmap ) ;


	return TRUE;
}

// BMP이미지 출력은 항상 다음의 5단계 
// <1> 읽기 : LoadImage() 
// <2> 생성 : DC생성 : CreateCompatibleDC()
//            비트맵 생성 부착 : CreateCompatibleBitmap() 
// <3> 선택 : SelectObject(bitmap)
// <4> 표시 : BitBlt()
// <5> 해제 : SelectObject(oldbitmap)
BOOL CBmpView::ShowBmp(CDC *pdc, int x, int y, CString filename)
{
	//////////////////< [1] 유효성 검사 >////////////////////////
	// 이미지를 읽는다.
    HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),filename,
	          IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);  // <1> 읽기 

	if( hBitmap == NULL ) {  //  실패이면 
		return FALSE;
	}


	//////////////////< [2] 전처리 작업 >////////////////////////

	// 1) 읽은 이미지에 대한 정보를 가져와 bm에 저장 (폭과 높이가 필요)
	BITMAP  bm;   
	(CBitmap::FromHandle(hBitmap))->GetBitmap(&bm);
    CRect  r(0,0,bm.bmWidth,bm.bmHeight);

    // 2) buff 생성(bitmap이미지를 가질 것임)                            // <2> DC생성 
	CDC		 buffDC ;
			 buffDC.CreateCompatibleDC( pdc ) ;
	//CBitmap  buffBitmap ;
	//		 buffBitmap.CreateCompatibleBitmap( pdc, bm.bmWidth, bm.bmHeight ) ;
    // 3) 선택(생성한 DC의 생성한 비트맵으로)
	CBitmap* oldbuffBitmap = buffDC.SelectObject(  CBitmap::FromHandle( hBitmap ) ) ;
                                                                         // <3> 선택 
	//////////////////< [3] 표시  >////////////////////////
    pdc->BitBlt(x,y, bm.bmWidth, bm.bmHeight, &buffDC,   0, 0, SRCCOPY  ) ;   // <4> 표시 

	//////////////////< [4] 해제  >////////////////////////                   // <5> 해제 
	buffDC.SelectObject( oldbuffBitmap ) ;

	return TRUE;

}

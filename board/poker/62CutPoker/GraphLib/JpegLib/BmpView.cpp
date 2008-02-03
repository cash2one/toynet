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

// BMP���� ����� �����ϰ� ����ϱ� ���ؼ��� ����Ʈ�� ����޵��� �׸������⸦ �̿��ؼ�
// �����ؾߵǴ� �κ��� �Ѱ����� ��(�ƹ����̳� �������)���� ��ĥ�� ���� �׻��� RGB���� 
// ����ϰ� �ִٰ� �Ʒ��� �Լ��� ȣ���ϸ� �ȴ�. GIF89a�� ���� ������ ������ �׸��� �����.
//    �ٸ� ȭ�� �ӿ� ������ �׸� ������ ����Ǿ� �ִٴ� ���� BMP�� �ٸ���.
//    �ǹ� ������ ������ gom99@intz.com �̳� webmaster@net-shopping.co.kr�� ���� �ּ���.

BOOL CBmpView::ShowBmp(CDC *pdc, int x, int y, CString filename, int maskcolor)
{
	//////////////////////////< [1] ��ȿ�� �˻� >////////////////////////////////////

	// �̹����� �д´�.
    HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),filename,
	          IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	if( hBitmap == NULL ) {  //  �����̸� 
		return FALSE;
	}


	//////////////////< [2] ��ó�� �۾� >////////////////////////

    // 1) ��ũ�� ����
	#define SRCMASK						0x00220326    // �Ʒ����� ����

	// 2) ���� �̹����� ���� ������ ������ bm�� ���� (���� ���̰� �ʿ�)
	BITMAP  bm;   
	(CBitmap::FromHandle(hBitmap))->GetBitmap(&bm);
    CRect  r(0,0,bm.bmWidth,bm.bmHeight);

    // 3) buff ����(bitmap�̹����� ���� ����)    
	CDC		 buffDC ;
			 buffDC.CreateCompatibleDC( pdc ) ;
	CBitmap  buffBitmap ;
			 buffBitmap.CreateCompatibleBitmap( pdc, bm.bmWidth, bm.bmHeight ) ;
	CBitmap* oldbuffBitmap = buffDC.SelectObject( &buffBitmap ) ;

	// 4) mask ����(bitmap�̹������� ���ϴ� �κи��� �����ϱ����� ) 
	CDC		 maskDC ;
			 maskDC.CreateCompatibleDC( pdc ) ;
             // ��ΰ� ����(0)�� ���Bitmap�� ���� 
	CBitmap  maskBitmap ;  
			 maskBitmap.CreateBitmap( bm.bmWidth, bm.bmHeight, 1, 1, NULL ) ;
	CBitmap* oldmaskBitmap = maskDC.SelectObject( &maskBitmap ) ;


	////////////////< [3] buffDC(�׸�Source)�� maskDC ����� >/////////////////////////

    // 1) buffDC�� transparent color (maskcolor)�� ä���.
    buffDC.FillSolidRect( &r, maskcolor) ;

    // 2)�ӽ� memory ����(DC�� bitmap)    bitmap�� hBitmap�� �ڵ��� ���� ����
	CDC		 memoryDC ;
			 memoryDC.CreateCompatibleDC( pdc ) ;
	CBitmap* oldmemoryBitmap = memoryDC.SelectObject( CBitmap::FromHandle( hBitmap ) ) ;
    // 3) memoryDC�� ������ buffDC�� ����
	buffDC.BitBlt( 0, 0, bm.bmWidth, bm.bmHeight, &memoryDC, 0, 0, SRCCOPY ) ;

    // 4) �׷��� Į�󸶽�ũ�� �����ϴ� AND����ũ�� ����
    //   ����----��(1), �̹����κ�----����(0)
	// buffDC�� ������ maskDC�� ����(�̶� ���ϴ� ������ mask��Ʈ���� ź��)
	maskDC.BitBlt( 0, 0, bm.bmWidth, bm.bmHeight, &buffDC, 0, 0, SRCCOPY ) ;


	/////////////////////////< [4] �׸����� >////////////////////////////////
	/// copyDC�� ������ �׸��� �ѷ��� �κ��� ������ͼ� ���� ������ ���� ������ �� 
	///          pdc�� �ǵ����� ����.

	// 1) copyDC ����.
	CDC		 copyDC ;        // ���� DC�� ���纻 
			 copyDC.CreateCompatibleDC( pdc ) ;
	CBitmap  copyBitmap ;    // ������� ���� �׸���ŭ��   
			 copyBitmap.CreateCompatibleBitmap( pdc, bm.bmWidth, bm.bmHeight ) ;
	CBitmap* oldcopyBitmap = copyDC.SelectObject( &copyBitmap ) ;

	// 2) ��Ÿ�� ����(x,y)���� �����ŭ ������ copyDC�� ����(������ ��游 �ִ�)
	copyDC.BitBlt(   0, 0, bm.bmWidth, bm.bmHeight, pdc,  x, y, SRCCOPY  ) ;

	// 3) maskDC�� SRCAND�����Ͽ� copyDC�� ���� (������濡 ����ε� �׸�) 
	copyDC.BitBlt(   0, 0, bm.bmWidth, bm.bmHeight, &maskDC,  0, 0, SRCAND  ) ;
	// 4) �ҽ�Bitmap�� buffDC�� SRCMASK����(����ũ��NOT�����ѰͰ� �ٽ� AND����)�صд�.
	buffDC.BitBlt(   0, 0, bm.bmWidth, bm.bmHeight, &maskDC,  0, 0, SRCMASK  ) ;
	// 5) �ҽ�(buffDC)�� SCRPRINT�����Ͽ� ���� copyDC����(������ ��濡 ����ε� �׸�)
	copyDC.BitBlt( 0, 0, bm.bmWidth, bm.bmHeight, &buffDC,   0, 0, SRCPAINT  ) ;
	// 6) ������� copyDC�� ���� DC�� ������ġ(x,y)�� ����
    pdc->BitBlt(x,y, bm.bmWidth, bm.bmHeight, &copyDC,   0, 0, SRCCOPY  ) ;

	//////////////////////////< [5] ���� >////////////////////////////////////
	copyDC.SelectObject( oldcopyBitmap ) ;
	memoryDC.SelectObject( oldmemoryBitmap ) ;
	maskDC.SelectObject( oldmaskBitmap ) ;
	buffDC.SelectObject( oldbuffBitmap ) ;


	return TRUE;
}

// BMP�̹��� ����� �׻� ������ 5�ܰ� 
// <1> �б� : LoadImage() 
// <2> ���� : DC���� : CreateCompatibleDC()
//            ��Ʈ�� ���� ���� : CreateCompatibleBitmap() 
// <3> ���� : SelectObject(bitmap)
// <4> ǥ�� : BitBlt()
// <5> ���� : SelectObject(oldbitmap)
BOOL CBmpView::ShowBmp(CDC *pdc, int x, int y, CString filename)
{
	//////////////////< [1] ��ȿ�� �˻� >////////////////////////
	// �̹����� �д´�.
    HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),filename,
	          IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);  // <1> �б� 

	if( hBitmap == NULL ) {  //  �����̸� 
		return FALSE;
	}


	//////////////////< [2] ��ó�� �۾� >////////////////////////

	// 1) ���� �̹����� ���� ������ ������ bm�� ���� (���� ���̰� �ʿ�)
	BITMAP  bm;   
	(CBitmap::FromHandle(hBitmap))->GetBitmap(&bm);
    CRect  r(0,0,bm.bmWidth,bm.bmHeight);

    // 2) buff ����(bitmap�̹����� ���� ����)                            // <2> DC���� 
	CDC		 buffDC ;
			 buffDC.CreateCompatibleDC( pdc ) ;
	//CBitmap  buffBitmap ;
	//		 buffBitmap.CreateCompatibleBitmap( pdc, bm.bmWidth, bm.bmHeight ) ;
    // 3) ����(������ DC�� ������ ��Ʈ������)
	CBitmap* oldbuffBitmap = buffDC.SelectObject(  CBitmap::FromHandle( hBitmap ) ) ;
                                                                         // <3> ���� 
	//////////////////< [3] ǥ��  >////////////////////////
    pdc->BitBlt(x,y, bm.bmWidth, bm.bmHeight, &buffDC,   0, 0, SRCCOPY  ) ;   // <4> ǥ�� 

	//////////////////< [4] ����  >////////////////////////                   // <5> ���� 
	buffDC.SelectObject( oldbuffBitmap ) ;

	return TRUE;

}

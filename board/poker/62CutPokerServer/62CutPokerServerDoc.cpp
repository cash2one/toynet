// 62CutPokerServerDoc.cpp : implementation of the C62CutPokerServerDoc class
//

#include "stdafx.h"
#include "62CutPokerServer.h"

#include "62CutPokerServerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C62CutPokerServerDoc

IMPLEMENT_DYNCREATE(C62CutPokerServerDoc, CDocument)

BEGIN_MESSAGE_MAP(C62CutPokerServerDoc, CDocument)
	//{{AFX_MSG_MAP(C62CutPokerServerDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C62CutPokerServerDoc construction/destruction

C62CutPokerServerDoc::C62CutPokerServerDoc()
{
	// TODO: add one-time construction code here

}

C62CutPokerServerDoc::~C62CutPokerServerDoc()
{
}

BOOL C62CutPokerServerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// C62CutPokerServerDoc serialization

void C62CutPokerServerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// C62CutPokerServerDoc diagnostics

#ifdef _DEBUG
void C62CutPokerServerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void C62CutPokerServerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// C62CutPokerServerDoc commands

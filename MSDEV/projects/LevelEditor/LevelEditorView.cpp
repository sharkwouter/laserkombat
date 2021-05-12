// LevelEditorView.cpp : implementation of the CLevelEditorView class
//

#include "stdafx.h"
#include "LevelEditor.h"

#include "LevelEditorDoc.h"
#include "enum.h"

#include "LevelEditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

	 BlockType current_block=WATER;
	 BlockType array[20][15]={GROUND};
	 BlockType2 groundTypeArray[20][15];
	 BlockType2 blockTypeArray[20][15];
	 int groundRotationArray[20][15];
	 int blockRotationArray[20][15];
	 int tank_x=0;
	 int tank_y=0;

/////////////////////////////////////////////////////////////////////////////
// CLevelEditorView

IMPLEMENT_DYNCREATE(CLevelEditorView, CView)

BEGIN_MESSAGE_MAP(CLevelEditorView, CView)
	//{{AFX_MSG_MAP(CLevelEditorView)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLevelEditorView construction/destruction

CLevelEditorView::CLevelEditorView()
{
	// TODO: add construction code here
	images_dc = CreateCompatibleDC(NULL);
	//if (!images_dc) AfxMessageBox("CreateCompatibleDC failed.");

    //CBitmap images;  // Construct it, then initialize
	hbm = (HBITMAP)LoadImage(GetModuleHandle(NULL), "IMAGES", IMAGE_BITMAP, 1184, 32, LR_CREATEDIBSECTION);
    //if (images.LoadBitmap("IMAGES"))
}

CLevelEditorView::~CLevelEditorView()
{
}

BOOL CLevelEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CLevelEditorView drawing

void CLevelEditorView::OnDraw(CDC* pDC)
{
	CLevelEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);



	// TODO: add draw code for native data here
    if (hbm&&images_dc) {
        // Select it into the device context
        // Save the old pen at the same time
        HGDIOBJ oldmap =SelectObject(images_dc, hbm);
		for (int y=0; y<15; y++) {
			for (int x=0; x<20; x++) {
				BitBlt(
					pDC->m_hDC,	// handle to destination device context 
					x*32,	// x-coordinate of destination rectangle's upper-left corner
					y*32,	// x-coordinate of destination rectangle's upper-left corner
					32,	// width of destination rectangle 
					32,	// height of destination rectangle 
					images_dc,	// handle to source device context 
					array[x][y]*32,	// x-coordinate of source rectangle's upper-left corner  
					0,	// y-coordinate of source rectangle's upper-left corner
					SRCCOPY	// raster operation code
				);
			}
		}
		SelectObject(images_dc, oldmap);
    }
   else
    {
        // Alert the user that resources are low
		AfxMessageBox("Error Drawing.");
	}

}

/////////////////////////////////////////////////////////////////////////////
// CLevelEditorView diagnostics

#ifdef _DEBUG
void CLevelEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CLevelEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLevelEditorDoc* CLevelEditorView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLevelEditorDoc)));
	return (CLevelEditorDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLevelEditorView message handlers

void CLevelEditorView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	int x= point.x / 32;
	int y= point.y / 32;

	if (x>=0&&x<20&&y>=0&&y<15) AddElement(x, y, current_block);
	
	CDC* pDC=GetDC();
	OnDraw(pDC);
	ReleaseDC(pDC);


	CView::OnLButtonDown(nFlags, point);
}

void CLevelEditorView::AddElement(int x, int y, BlockType block)
{
	EditorToGame(x,y, block);
	
}

void CLevelEditorView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	if (MK_LBUTTON & nFlags) {
		int x= point.x / 32;
		int y= point.y / 32;

		if (x>=0&&x<20&&y>=0&&y<15) AddElement(x, y, current_block);

		CDC* pDC=GetDC();
		OnDraw(pDC);
		ReleaseDC(pDC);
	}

	CView::OnMouseMove(nFlags, point);
}

void CLevelEditorView::EditorToGame(int x, int y, BlockType block) {
	if (x==tank_x&&y==tank_y) return;
	BlockType2 *t;
	int *r;
	if (block==GROUND||block==WATER) {
		t=&groundTypeArray[x][y];
		r=&groundRotationArray[x][y];
		blockTypeArray[x][y]=NONE;
		blockRotationArray[x][y]=0;
	}
	else {
		if (groundTypeArray[x][y]==WATER_&&block!=STATIC&&block!=
			BARSVERT&&block!=BARSHORIZ&&block!=BARSCROSS&&block!=RUSTY&&
			!(block>=WHITEBLOCK&&block<=RUSTYBARSHORIZ)) return;
		if (block==TANK) {
			if (tank_x>=0&&tank_y>=0) {
				switch (groundRotationArray[tank_x][tank_y]) {
				case 33:
				default:
					array[tank_x][tank_y]=GROUND; break;
				//case 28:
				//	array[tank_x][tank_y]=RAILHORIZ; break;
				//case 27:
				//	array[tank_x][tank_y]=RAILCROSS; break;
				//case 25:
				//	array[tank_x][tank_y]=RAILVERT; break;
				}
				blockTypeArray[tank_x][tank_y]=NONE;
				blockRotationArray[tank_x][tank_y]=0;
			}
			tank_x=x; tank_y=y;

		}
		t=&blockTypeArray[x][y];
		r=&blockRotationArray[x][y];
	}

	array[x][y]=block;
	switch (block) {
		case GROUND:		*t=GROUND_;	*r=33;	break;
		case WATER:			*t=WATER_;	*r=0;	break;
		case REDBLOCK:		*t=REDBLOCK_;	*r=0;	break;
		case RUSTYREDBLOCK:	*t=RUSTYREDBLOCK_; *r=0;break;
		case TEE1:			*t=TEE_;		*r=4;	break;
		case TEE2:			*t=TEE_;		*r=5;	break;
		case TEE3:			*t=TEE_;		*r=6;	break;
		case TEE4:			*t=TEE_;		*r=3;	break;
		case TEE5:			*t=TEE_;		*r=0;	break;
		case TEE6:			*t=TEE_;		*r=1;	break;
		case TEE7:			*t=TEE_;		*r=2;	break;
		case MIRROR1:		*t=MIRROR_;	*r=2;	break;
		case MIRROR2:		*t=MIRROR_;	*r=3;	break;
		case MIRROR3:		*t=MIRROR_;	*r=0;	break;
		case MIRROR4:		*t=MIRROR_;	*r=1;	break;
		case NUKE:			*t=NUKE_;	*r=0;	break;
		case ENEMYNUKE:		*t=ENEMYNUKE_;*r=0;	break;
		case STATIC:		*t=STATIC_;	*r=rand()%13;	break;
		case RUSTY:			*t=RUSTY_;	*r=rand()%13;	break;
		case BARSVERT:		*t=BARSVERT_;	*r=0;	break;
		case BARSHORIZ:		*t=BARSHORIZ_;	*r=0;	break;
		case BARSCROSS:		*t=BARSCROSS_;	*r=0;	break;
		case TANK:			*t=TANK_;	*r=0;	break;
		case ENEMYTANK:		*t=ENEMYTANK_;	*r=0;	break;
		case TANTANK:		*t=TANK_;	*r=8;	break;
		case WHITEBLOCK:	*t=WHITEBLOCK_; *r=0; break;
		case TRIANGLE1:		*t=TRIANGLE_; *r=2; break;
		case TRIANGLE2:		*t=TRIANGLE_; *r=3; break;
		case TRIANGLE3:		*t=TRIANGLE_; *r=0; break;
		case TRIANGLE4:		*t=TRIANGLE_; *r=1; break;
		case RUSTYTRIANGLE1:		*t=RUSTYTRIANGLE_; *r=2; break;
		case RUSTYTRIANGLE2:		*t=RUSTYTRIANGLE_; *r=3; break;
		case RUSTYTRIANGLE3:		*t=RUSTYTRIANGLE_; *r=0; break;
		case RUSTYTRIANGLE4:		*t=RUSTYTRIANGLE_; *r=1; break;
		case RUSTYBARSVERT:			*t=RUSTYBARSVERT_;	*r=0;	break;
		case RUSTYBARSHORIZ:		*t=RUSTYBARSHORIZ_;	*r=0;	break;
		case RUSTYWHITEBLOCK:		*t=RUSTYWHITEBLOCK_;*r=0;	break;

		default: break;
			//*t=WATER_; *r=0; break;
	}
}
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "LevelEditor.h"

#include "enum.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_GROUND, OnGround)
	ON_COMMAND(ID_MIRROR1, OnMirror1)
	ON_COMMAND(ID_MIRROR2, OnMirror2)
	ON_COMMAND(ID_MIRROR3, OnMirror3)
	ON_COMMAND(ID_MIRROR4, OnMirror4)
	ON_COMMAND(ID_NEXT_PANE, OnNextPane)
	ON_COMMAND(ID_NUKE, OnNuke)
	ON_COMMAND(ID_PREV_PANE, OnPrevPane)
	ON_COMMAND(ID_RAILCROSS, OnRailcross)
	ON_COMMAND(ID_RAILHORIZ, OnRailhoriz)
	ON_COMMAND(ID_RAILVERT, OnRailvert)
	ON_COMMAND(ID_REDBLOCK, OnRedblock)
	ON_COMMAND(ID_RUSTY, OnRusty)
	ON_COMMAND(ID_RUSTYREDBLOCK, OnRustyredblock)
	ON_COMMAND(ID_STATIC, OnStatic)
	ON_COMMAND(ID_TEE1, OnTee1)
	ON_COMMAND(ID_TEE2, OnTee2)
	ON_COMMAND(ID_TEE3, OnTee3)
	ON_COMMAND(ID_TEE4, OnTee4)
	ON_COMMAND(ID_TEE5, OnTee5)
	ON_COMMAND(ID_TEE6, OnTee6)
	ON_COMMAND(ID_TEE7, OnTee7)
	ON_COMMAND(ID_WATER, OnWater)
	ON_COMMAND(ID_ENEMEYNUKE, OnEnemeynuke)
	ON_COMMAND(ID_TANK, OnTank)
	ON_COMMAND(ID_TANTANK, OnTantank)
	ON_COMMAND(ID_ENEMYTANK, OnEnemytank)
	ON_COMMAND(ID_WHITEBLOCK, OnWhiteblock)
	ON_COMMAND(ID_TRIANGLE1, OnTriangle1)
	ON_COMMAND(ID_TRIANGLE2, OnTriangle2)
	ON_COMMAND(ID_TRIANGLE3, OnTriangle3)
	ON_COMMAND(ID_TRIANGLE4, OnTriangle4)
	ON_COMMAND(ID_RUSTYTRIANGLE1, OnRustytriangle1)
	ON_COMMAND(ID_RUSTYTRIANGLE2, OnRustytriangle2)
	ON_COMMAND(ID_RUSTYTRIANGLE3, OnRustytriangle3)
	ON_COMMAND(ID_RUSTYTRIANGLE4, OnRustytriangle4)
	ON_COMMAND(ID_RUSTYBARSHORIZ, OnRustybarshoriz)
	ON_COMMAND(ID_RUSTYBARSVERT, OnRustybarsvert)
	ON_COMMAND(ID_RUSTYWHITEBLOCK, OnRustywhiteblock)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

//BlockType CMainFrame::current_block=GROUND;
//BlockType CMainFrame::array[20][15]= {GROUND};


CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Remove this if you don't want tool tips or a resizeable toolbar
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	
	FloatControlBar(&m_wndToolBar, CPoint(100,100));

	m_wndToolBar.SetWindowPos(this, 0, 0, 300, 400, SWP_FRAMECHANGED/*SWP_DRAWFRAME|SWP_NOMOVE|SWP_NOZORDER|SWP_SHOWWINDOW*/);



	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.cx=640;
	cs.cy=480;
	//cs.style &= ~(WS_MAXIMIZEBOX|WS_MINIMIZEBOX|WS_THICKFRAME);
	return CFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnGround() 
{
	// TODO: Add your command handler code here
	current_block=GROUND;
}

void CMainFrame::OnMirror1() 
{
	// TODO: Add your command handler code here
	current_block=MIRROR1;
}

void CMainFrame::OnMirror2() 
{
	// TODO: Add your command handler code here
	current_block=MIRROR2;
}

void CMainFrame::OnMirror3() 
{
	// TODO: Add your command handler code here
	current_block=MIRROR3;
}

void CMainFrame::OnMirror4() 
{
	// TODO: Add your command handler code here
	current_block=MIRROR4;
}

void CMainFrame::OnNextPane() 
{
	// TODO: Add your command handler code here
}

void CMainFrame::OnNuke() 
{
	// TODO: Add your command handler code here
	current_block=NUKE;
}

void CMainFrame::OnPrevPane() 
{
	// TODO: Add your command handler code here
}

void CMainFrame::OnRailcross() 
{
	// TODO: Add your command handler code here
	current_block=BARSCROSS;
}

void CMainFrame::OnRailhoriz() 
{
	// TODO: Add your command handler code here
	current_block=BARSHORIZ;
}

void CMainFrame::OnRailvert() 
{
	// TODO: Add your command handler code here
	current_block=BARSVERT;
}

void CMainFrame::OnRedblock() 
{
	// TODO: Add your command handler code here
	current_block=REDBLOCK;
}

void CMainFrame::OnRusty() 
{
	// TODO: Add your command handler code here
	current_block=RUSTY;
}

void CMainFrame::OnRustyredblock() 
{
	// TODO: Add your command handler code here
	current_block=RUSTYREDBLOCK;
}

void CMainFrame::OnStatic() 
{
	// TODO: Add your command handler code here
	current_block=STATIC;
}

void CMainFrame::OnTee1() 
{
	// TODO: Add your command handler code here
	current_block=TEE1;
}

void CMainFrame::OnTee2() 
{
	// TODO: Add your command handler code here
	current_block=TEE2;
}

void CMainFrame::OnTee3() 
{
	// TODO: Add your command handler code here
	current_block=TEE3;
}

void CMainFrame::OnTee4() 
{
	// TODO: Add your command handler code here
	current_block=TEE4;
}

void CMainFrame::OnTee5() 
{
	// TODO: Add your command handler code here
	current_block=TEE5;
}

void CMainFrame::OnTee6() 
{
	// TODO: Add your command handler code here
	current_block=TEE6;
}

void CMainFrame::OnTee7() 
{
	// TODO: Add your command handler code here
	current_block=TEE7;
}

void CMainFrame::OnWater() 
{
	// TODO: Add your command handler code here
	current_block=WATER;
}

void CMainFrame::OnEnemeynuke() 
{
	// TODO: Add your command handler code here
	current_block=ENEMYNUKE;
}



void CMainFrame::OnTank() 
{
	// TODO: Add your command handler code here
	current_block=TANK;
	
}

void CMainFrame::OnTantank() 
{
	// TODO: Add your command handler code here
	current_block=TANTANK;
	
}

void CMainFrame::OnEnemytank() 
{
	// TODO: Add your command handler code here
	current_block=ENEMYTANK;
	
}

void CMainFrame::OnWhiteblock() 
{
	// TODO: Add your command handler code here
	current_block=WHITEBLOCK;
	
}

void CMainFrame::OnTriangle1() 
{
	// TODO: Add your command handler code here
	current_block=TRIANGLE1;
	
}

void CMainFrame::OnTriangle2() 
{
	// TODO: Add your command handler code here
	current_block=TRIANGLE2;
	
}

void CMainFrame::OnTriangle3() 
{
	// TODO: Add your command handler code here
	current_block=TRIANGLE3;
	
}

void CMainFrame::OnTriangle4() 
{
	// TODO: Add your command handler code here
	current_block=TRIANGLE4;
	
}

void CMainFrame::OnRustytriangle1() 
{
	// TODO: Add your command handler code here
	current_block=RUSTYTRIANGLE1;
	
}

void CMainFrame::OnRustytriangle2() 
{
	// TODO: Add your command handler code here
	current_block=RUSTYTRIANGLE2;
	
}

void CMainFrame::OnRustytriangle3() 
{
	// TODO: Add your command handler code here
	current_block=RUSTYTRIANGLE3;
	
}

void CMainFrame::OnRustytriangle4() 
{
	// TODO: Add your command handler code here
	current_block=RUSTYTRIANGLE4;
	
}

void CMainFrame::OnRustybarshoriz() 
{
	// TODO: Add your command handler code here
	current_block=RUSTYBARSHORIZ;
	
}

void CMainFrame::OnRustybarsvert() 
{
	// TODO: Add your command handler code here
	current_block=RUSTYBARSVERT;
	
}

void CMainFrame::OnRustywhiteblock() 
{
	// TODO: Add your command handler code here
	current_block=RUSTYWHITEBLOCK;
	
}

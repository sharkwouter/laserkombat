// LevelEditorDoc.cpp : implementation of the CLevelEditorDoc class
//

#include "stdafx.h"
#include "enum.h"
#include "LevelEditor.h"

#include "LevelEditorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLevelEditorDoc

IMPLEMENT_DYNCREATE(CLevelEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CLevelEditorDoc, CDocument)
	//{{AFX_MSG_MAP(CLevelEditorDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLevelEditorDoc construction/destruction

CLevelEditorDoc::CLevelEditorDoc()
{
	// TODO: add one-time construction code here
	for (int y=0; y<15; y++) {
		for (int x=0; x<20; x++) {
			array[x][y]=GROUND;
		}
	}
	current_block=WATER;

}

CLevelEditorDoc::~CLevelEditorDoc()
{
}

BOOL CLevelEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	for (int y=0; y<15; y++) {
		for (int x=0; x<20; x++) {
			array[x][y]=GROUND;
			groundTypeArray[x][y]=GROUND_;
			blockTypeArray[x][y]=NONE;
			groundRotationArray[x][y]=33;
			blockRotationArray[x][y]=0;
		}
	}
	array[0][0]=TANK;
	blockTypeArray[0][0]=TANK_;
	blockRotationArray[0][0]=0;
	current_block=WATER;
	tank_x=tank_y=0;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CLevelEditorDoc serialization

void CLevelEditorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here

		ar.Write( &tank_x, sizeof(tank_x));
		ar.Write( &tank_y, sizeof(tank_y));
		ar.Write( groundTypeArray, 20*15*sizeof(BlockType2));
		ar.Write( groundRotationArray, 20*15*sizeof(int));
		ar.Write( blockTypeArray, 20*15*sizeof(BlockType2));
		ar.Write( blockRotationArray, 20*15*sizeof(int));
		ar.Write( array, 20*15*sizeof(BlockType));


	}
	else
	{
		// TODO: add loading code here

		ar.Read( &tank_x, sizeof(tank_x));
		ar.Read( &tank_y, sizeof(tank_y));
		ar.Read( groundTypeArray, 20*15*sizeof(BlockType2));
		ar.Read( groundRotationArray, 20*15*sizeof(int));
		ar.Read( blockTypeArray, 20*15*sizeof(BlockType2));
		ar.Read( blockRotationArray, 20*15*sizeof(int));
		ar.Read( array, 20*15*sizeof(BlockType));
	}
}

/////////////////////////////////////////////////////////////////////////////
// CLevelEditorDoc diagnostics

#ifdef _DEBUG
void CLevelEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CLevelEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLevelEditorDoc commands



// LevelEditorView.h : interface of the CLevelEditorView class
//
/////////////////////////////////////////////////////////////////////////////


class CLevelEditorView : public CView
{
protected: // create from serialization only
	CLevelEditorView();
	DECLARE_DYNCREATE(CLevelEditorView)

// Attributes
public:
	CLevelEditorDoc* GetDocument();
	void EditorToGame(int x, int y, BlockType block);
	void AddElement(int x, int y, BlockType block);
	HDC images_dc;
	HBITMAP hbm;


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLevelEditorView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLevelEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CLevelEditorView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in LevelEditorView.cpp
inline CLevelEditorDoc* CLevelEditorView::GetDocument()
   { return (CLevelEditorDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

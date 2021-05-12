// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

class CMainFrame : public CFrameWnd
{
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnGround();
	afx_msg void OnMirror1();
	afx_msg void OnMirror2();
	afx_msg void OnMirror3();
	afx_msg void OnMirror4();
	afx_msg void OnNextPane();
	afx_msg void OnNuke();
	afx_msg void OnPrevPane();
	afx_msg void OnRailcross();
	afx_msg void OnRailhoriz();
	afx_msg void OnRailvert();
	afx_msg void OnRedblock();
	afx_msg void OnRusty();
	afx_msg void OnRustyredblock();
	afx_msg void OnStatic();
	afx_msg void OnTee1();
	afx_msg void OnTee2();
	afx_msg void OnTee3();
	afx_msg void OnTee4();
	afx_msg void OnTee5();
	afx_msg void OnTee6();
	afx_msg void OnTee7();
	afx_msg void OnWater();
	afx_msg void OnEnemeynuke();
	afx_msg void OnTank();
	afx_msg void OnTantank();
	afx_msg void OnEnemytank();
	afx_msg void OnWhiteblock();
	afx_msg void OnTriangle1();
	afx_msg void OnTriangle2();
	afx_msg void OnTriangle3();
	afx_msg void OnTriangle4();
	afx_msg void OnRustytriangle1();
	afx_msg void OnRustytriangle2();
	afx_msg void OnRustytriangle3();
	afx_msg void OnRustytriangle4();
	afx_msg void OnRustybarshoriz();
	afx_msg void OnRustybarsvert();
	afx_msg void OnRustywhiteblock();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

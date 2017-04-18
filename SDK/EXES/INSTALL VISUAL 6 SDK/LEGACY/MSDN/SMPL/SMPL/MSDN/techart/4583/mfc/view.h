// view.h : interface of the CXlclientView class
//
/////////////////////////////////////////////////////////////////////////////

class CXlclientView : public CView
{
protected: // create from serialization only
	CXlclientView();
	DECLARE_DYNCREATE(CXlclientView)

// Attributes
public:
	CXlclientDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXlclientView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CXlclientView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CXlclientView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in vew.cpp
inline CXlclientDoc* CXlclientView::GetDocument()
   { return (CXlclientDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

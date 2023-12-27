
// Lab1View.h : interface of the CLab1View class
//

#pragma once


class CLab1View : public CView
{
protected: // create from serialization only
	CLab1View() noexcept;
	DECLARE_DYNCREATE(CLab1View)

	// Attributes
public:
	CLab1Doc* GetDocument() const;
	int grid_size;
	bool key_pressed;
	// Operations
public:

	// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void DrawGrid(CDC* pDC);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// Implementation
public:
	virtual ~CLab1View();
	void DrawRegularPolygon(CDC* pDC, float cx, float cy, float r, int n, float rotAngle);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Lab1View.cpp
inline CLab1Doc* CLab1View::GetDocument() const
{
	return reinterpret_cast<CLab1Doc*>(m_pDocument);
}
#endif


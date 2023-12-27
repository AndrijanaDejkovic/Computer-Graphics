
// LV03View.h : interface of the CLV03View class
//

#pragma once
#include "DImage.h" //mora i da prevuces DImage.h/cpp fajlove i da ih ubacis onamo u folder

class CLV03View : public CView
{
protected: // create from serialization only
	CLV03View();
	DECLARE_DYNCREATE(CLV03View)
	//Methods
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Mirror(CDC* pDC, bool mx, bool my, bool rightMultiply);
	void DrawGrid(CDC* memDC);
	void MakeItGray(DImage* img);
	void MakeItBlue(DImage* img);
	void MemDCTopDC(CDC* memDC, CDC* pDC);
	void MakeItRed(DImage* img); //ce treba za lab mzd
	void DrawTransparent(CDC* pDC, DImage* img, int x, int y);
	void SetPuzzle(CDC* pDC, CBitmap* bmp);
	void SetPuzzle(CDC* pDC, CBitmap bmp);
	// Attributes
	DImage** part;
	const float C_PI = 3.141592653589793;
	const float stepAngle = C_PI / 180; // Jedan stepen
// Attributes
public:
	CLV03Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CLV03View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in LV03View.cpp
inline CLV03Doc* CLV03View::GetDocument() const
   { return reinterpret_cast<CLV03Doc*>(m_pDocument); }
#endif


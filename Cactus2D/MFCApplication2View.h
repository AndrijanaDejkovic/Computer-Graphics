
// IND_16038View.h : interface of the CIND16038View class
//

#pragma once


class CMFCApplication2View : public CView
{
protected: // create from serialization only
	CMFCApplication2View() noexcept;
	DECLARE_DYNCREATE(CMFCApplication2View)
	bool grid = 0;
	int pomeraj = 25;
	double ugaoSrednji = 0;
	double ugaoNovi = 0;
	double ugaoDela = 0;
	double ugaoLevi = -45;
	const double PI = 3.14159265359;

	HENHMETAFILE kaktusSvetli;
	HENHMETAFILE kaktusTamni;

	// Attributes
public:
	CMFCApplication2Doc* GetDocument() const;

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
	virtual void mappingMode(CDC* pDC);
	virtual void nacrtajGrid(CDC* pDC);
	virtual void nacrtajKockice(CDC* pDC);
	virtual void nacrtajSaksiju(CDC* pDC);
	virtual void nacrtajKrug(CDC* pDC, POINT center, int r);
	virtual void translate(CDC* pDC, int x, int y);
	virtual void rotate(CDC* pDC, double angle);
	virtual void napisiFont(CDC* pDC);
	virtual void nacrtajKaktus(CDC* pDC);
	virtual void nacrtajDeo(CDC* pDC);


	// Implementation
public:
	virtual ~CMFCApplication2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in IND_16038View.cpp
inline CIND16038Doc* CIND16038View::GetDocument() const
{
	return reinterpret_cast<CIND16038Doc*>(m_pDocument);
}
#endif



// MFCApplication2View.cpp : implementation of the CMFCApplication2View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MFCApplication2.h"
#endif

#include "MFCApplication2Doc.h"
#include "MFCApplication2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// MFCApplication2View

IMPLEMENT_DYNCREATE(CMFCApplication2View, CView)

BEGIN_MESSAGE_MAP(CMFCApplication2View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// MFCApplication2View construction/destruction

CMFCApplication2View::CMFCApplication2View() noexcept
{
	// TODO: add construction code here

	//Učitavanje unapređenog metafajla(EMF) iz datoteke ostvaruje se odgovarajućom
	//Win32 funkcijom, kojoj se kao parametar prosleđuje naziv datoteke
	kaktusTamni = GetEnhMetaFile(L"cactus_part.emf");
	kaktusSvetli = GetEnhMetaFile(L"cactus_part_light.emf");
	grid = true;

}

CMFCApplication2View::~CMFCApplication2View()
{

	//Po završetku korišćenja, metafajl treba obrisati prosleđivanjem njegovog handle-a
	DeleteEnhMetaFile(kaktusSvetli);
	DeleteEnhMetaFile(kaktusTamni);
}

BOOL CMFCApplication2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// MFCApplication2View drawing

void CMFCApplication2View::OnDraw(CDC* pDC)
{
	CMFCApplication2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here

	this->mappingMode(pDC);
	int oldMode = SetGraphicsMode(pDC->m_hDC, GM_ADVANCED);

	nacrtajGrid(pDC);

	if (grid)
		nacrtajKockice(pDC);

	XFORM form, oldForm;
	GetWorldTransform(pDC->m_hDC, &oldForm);

	translate(pDC, 10 * pomeraj, 10 * pomeraj);
	rotate(pDC, 180);
	translate(pDC, -10 * pomeraj, -10 * pomeraj);

	//ovde samo transformisi sliku za ugao koji hoces, i onda znaju kako menja koordinate kad vrsis rotaciju

	nacrtajSaksiju(pDC);

	/*pDC->SetWorldTransform(&oldForm);


	translate(pDC, 10 * pomeraj, 10 * pomeraj);
	rotate(pDC, 180);
	translate(pDC, -10 * pomeraj, -10 * pomeraj);*/

	napisiFont(pDC);


	/*translate(pDC, 10 * pomeraj, 17 * pomeraj);
	rotate(pDC, ugaoSrednji);
	translate(pDC, -10 * pomeraj, -17 * pomeraj);*/    //ove 3 linije se odnose na sledecu (nacrtajKaktus). ja

	nacrtajKaktus(pDC);






}

void CMFCApplication2View::mappingMode(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	pDC->SetMapMode(MM_ISOTROPIC);
	pDC->SetWindowExt(1, 1);
	pDC->SetViewportExt(1, 1);
}

void CMFCApplication2View::nacrtajGrid(CDC* pDC)
{
	CBrush* brush = new CBrush(RGB(135, 206, 255));

	CPen* oldPen = (CPen*)pDC->SelectStockObject(NULL_PEN);
	CPen* pen = new CPen(PS_GEOMETRIC, 0, RGB(255, 255, 255));


	pDC->SelectObject(brush);
	pDC->SelectObject(pen);
	pDC->Rectangle(0, 0, 500, 500);

}

void CMFCApplication2View::nacrtajKockice(CDC* pDC) {

	CPen* pen = new CPen(PS_GEOMETRIC, 0, RGB(255, 255, 255));
	pDC->SelectObject(pen);

	for (int i = 0; i <= 500; i += pomeraj) {

		pDC->MoveTo(0, i);
		pDC->LineTo(500, i);

		pDC->MoveTo(i, 0);
		pDC->LineTo(i, 500);
	}

}

void CMFCApplication2View::nacrtajSaksiju(CDC* pDC)
{
	CBrush* brush2 = new CBrush(RGB(0, 222, 0));
	CPen* pen1 = new CPen(PS_GEOMETRIC, 0, RGB(0, 0, 0));
	pDC->SelectObject(brush2);
	pDC->SelectObject(pen1);

	nacrtajKrug(pDC, { 10 * pomeraj, 17 * pomeraj }, pomeraj);

	CBrush* brush1 = new CBrush(RGB(222, 148, 0));
	pDC->SelectObject(brush1);

	CPoint niz[9];

	niz[0] = CPoint(8.5 * pomeraj, 20 * pomeraj);
	niz[1] = CPoint(11.5 * pomeraj, 20 * pomeraj);
	niz[2] = CPoint(12 * pomeraj, 18 * pomeraj);
	niz[3] = CPoint(12.3 * pomeraj, 18 * pomeraj);
	niz[4] = CPoint(12.3 * pomeraj, 17.2 * pomeraj);
	niz[5] = CPoint(7.7 * pomeraj, 17.2 * pomeraj);
	niz[6] = CPoint(7.7 * pomeraj, 18 * pomeraj);
	niz[7] = CPoint(8 * pomeraj, 18 * pomeraj);
	niz[8] = CPoint(8.5 * pomeraj, 20 * pomeraj);

	pDC->Polygon(niz, 9);

}

void CMFCApplication2View::nacrtajKrug(CDC* pDC, POINT center, int r)
{
	pDC->Ellipse(center.x - r / 2, center.y - r / 2, center.x + r / 2, center.y + r / 2);
}

void CMFCApplication2View::translate(CDC* pDC, int x, int y)
{
	XFORM xForm;
	xForm.eM11 = 1;
	xForm.eM12 = 0;
	xForm.eM21 = 0;
	xForm.eM22 = 1;
	xForm.eDx = x;
	xForm.eDy = y;
	ModifyWorldTransform(pDC->m_hDC, &xForm, MWT_LEFTMULTIPLY);
}

void CMFCApplication2View::rotate(CDC* pDC, double angle)
{
	XFORM xForm;
	xForm.eM11 = cos(angle * 3.14 / 180);
	xForm.eM12 = sin(angle * 3.14 / 180);
	xForm.eM21 = -sin(angle * 3.14 / 180);
	xForm.eM22 = cos(angle * 3.14 / 180);
	xForm.eDx = 0;
	xForm.eDy = 0;
	ModifyWorldTransform(pDC->m_hDC, &xForm, MWT_LEFTMULTIPLY);
}

void CMFCApplication2View::nacrtajDeo(CDC* pDC) {
	PlayEnhMetaFile(*pDC, kaktusTamni, CRect(6.5 * pomeraj, 4 * pomeraj, 9.5 * pomeraj, 7 * pomeraj));				//ne treba nigde? ja
}
void CMFCApplication2View::nacrtajKaktus(CDC* pDC) {

	CRect rect;
	GetClientRect(&rect);
	XFORM form, oldForm;
	GetWorldTransform(pDC->m_hDC, &oldForm);

	//pDC->SetWorldTransform(&oldForm);


	CBrush* brush2 = new CBrush(RGB(0, 222, 0));
	CPen* pen1 = new CPen(PS_GEOMETRIC, 0, RGB(0, 0, 0));
	pDC->SelectObject(brush2);
	pDC->SelectObject(pen1);

	//srednji deo

	/*
	translate(pDC, 10 * pomeraj, 17 * pomeraj);
	rotate(pDC, ugaoSrednji);
	translate(pDC, -10 * pomeraj, -17 * pomeraj);

	*/


	//Crtanje unapređenog metafajla na osnovu prosleđenog handle-a i okvirnog
	//pravougaonika u kome se iscrtava
	//prva tri odozdo uspravno
	PlayEnhMetaFile(*pDC, kaktusTamni, CRect(8.7 * pomeraj, 14 * pomeraj, 11.3 * pomeraj, 17 * pomeraj));
	nacrtajKrug(pDC, { 10 * pomeraj, 14 * pomeraj }, pomeraj);
	//PlayEnhMetaFile(*pDC, kaktusTamni, CRect(9.5 * pomeraj, 11 * pomeraj, 10.5 * pomeraj, 14 * pomeraj));
	//DODATO!!!!!!!
	//desni sredisnji
	/*translate(pDC, 10 * pomeraj, 14 * pomeraj);
	rotate(pDC, 45);
	translate(pDC, -10 * pomeraj, -14 * pomeraj);


	PlayEnhMetaFile(*pDC, kaktusSvetli, CRect(9.2* pomeraj, 11.2 * pomeraj, 10.8 * pomeraj, 14 * pomeraj));*/

	pDC->SetWorldTransform(&oldForm);

	//levi sredisnji
	translate(pDC, 10 * pomeraj, 14 * pomeraj);
	rotate(pDC, -45);
	translate(pDC, -10 * pomeraj, -14 * pomeraj);

	PlayEnhMetaFile(*pDC, kaktusTamni, CRect(9.2 * pomeraj, 11.2 * pomeraj, 10.8 * pomeraj, 14 * pomeraj));

	pDC->SetWorldTransform(&oldForm);

	//prvi levi kruzic odozdo, gore bre
	nacrtajKrug(pDC, { 8 * pomeraj, 12 * pomeraj }, pomeraj);
	//ODAVDE
	//levi polozen
	translate(pDC, 8 * pomeraj, 12 * pomeraj);
	rotate(pDC, -45);
	translate(pDC, -8 * pomeraj, -12 * pomeraj);

	PlayEnhMetaFile(*pDC, kaktusTamni, CRect(7.5 * pomeraj, 9 * pomeraj, 8.5 * pomeraj, 12 * pomeraj));

	pDC->SetWorldTransform(&oldForm);

	translate(pDC, 8 * pomeraj, 12 * pomeraj);
	rotate(pDC, -90);
	translate(pDC, -8 * pomeraj, -12 * pomeraj);

	PlayEnhMetaFile(*pDC, kaktusTamni, CRect(7.5 * pomeraj, 9 * pomeraj, 8.5 * pomeraj, 12 * pomeraj));

	pDC->SetWorldTransform(&oldForm);

	translate(pDC, 8 * pomeraj, 12 * pomeraj);
	rotate(pDC, 0);
	translate(pDC, -8 * pomeraj, -12 * pomeraj);

	PlayEnhMetaFile(*pDC, kaktusTamni, CRect(7.5 * pomeraj, 9 * pomeraj, 8.5 * pomeraj, 12 * pomeraj));

	pDC->SetWorldTransform(&oldForm);

	//levi uspravni deo
	/*/PlayEnhMetaFile(*pDC, kaktusTamni, CRect(7 * pomeraj, 9 * pomeraj, 9 * pomeraj, 12 * pomeraj));
	nacrtajKrug(pDC, { 8 * pomeraj, 9 * pomeraj }, pomeraj);
	PlayEnhMetaFile(*pDC, kaktusTamni, CRect(6.5 * pomeraj, 5.7 * pomeraj, 9.5 * pomeraj, 9 * pomeraj));
	*/

	//DODATO!!!!!!!
	//desni kruzic prvi na koji se naidje od sredine
	nacrtajKrug(pDC, { 12 * pomeraj, 12 * pomeraj }, pomeraj);
	//PlayEnhMetaFile(*pDC, kaktusSvetli, CRect(11 * pomeraj, 9 * pomeraj, 13 * pomeraj, 12 * pomeraj));
	PlayEnhMetaFile(*pDC, kaktusTamni, CRect(11.5 * pomeraj, 9 * pomeraj, 12.5 * pomeraj, 12 * pomeraj));
	
	//desni sredisnji
	/*translate(pDC, 12 * pomeraj, 12 * pomeraj);
	rotate(pDC, 45);
	translate(pDC, -12 * pomeraj, -12 * pomeraj);

	PlayEnhMetaFile(*pDC, kaktusSvetli, CRect(11.5 * pomeraj, 9 * pomeraj, 12.5 * pomeraj, 12 * pomeraj));

	pDC->SetWorldTransform(&oldForm);*/

	//levi sredisnji
	translate(pDC, 12 * pomeraj, 12 * pomeraj);
	rotate(pDC, 90);
	translate(pDC, -12 * pomeraj, -12 * pomeraj);

	PlayEnhMetaFile(*pDC, kaktusTamni, CRect(11.5 * pomeraj, 9 * pomeraj, 12.5 * pomeraj, 12 * pomeraj));

	pDC->SetWorldTransform(&oldForm);

	nacrtajKrug(pDC, { 6 * pomeraj, 10 * pomeraj }, pomeraj);
	translate(pDC, 6 * pomeraj, 10 * pomeraj);
	rotate(pDC, -45);
	translate(pDC, -6 * pomeraj, -10 * pomeraj);
	PlayEnhMetaFile(*pDC, kaktusTamni, CRect(4.65 * pomeraj, 7 * pomeraj, 7.25 * pomeraj, 10 * pomeraj));

	pDC->SetWorldTransform(&oldForm);


	translate(pDC, 10 * pomeraj, 14 * pomeraj);
	rotate(pDC, 45);
	translate(pDC, -10 * pomeraj, -14 * pomeraj);

	translate(pDC, 10 * pomeraj, 14 * pomeraj);
	rotate(pDC, ugaoNovi);
	translate(pDC, -10 * pomeraj, -14 * pomeraj);



	PlayEnhMetaFile(*pDC, kaktusSvetli, CRect(9.2 * pomeraj, 11.2 * pomeraj, 10.8 * pomeraj, 14 * pomeraj));
	pDC->SetWorldTransform(&oldForm);



	//ende
	/*nacrtajKrug(pDC, { 6 * pomeraj, 10 * pomeraj }, pomeraj);
	translate(pDC, 6 * pomeraj, 10 * pomeraj);
	rotate(pDC, -45);
	translate(pDC, -6 * pomeraj, -10 * pomeraj);
	PlayEnhMetaFile(*pDC, kaktusTamni, CRect(4.65 * pomeraj, 7 * pomeraj, 7.25 * pomeraj, 10 * pomeraj));*/

	/*translate(pDC, 6 * pomeraj, 10 * pomeraj);
	rotate(pDC, -90);
	translate(pDC, -6 * pomeraj, -10 * pomeraj);*/

	//PlayEnhMetaFile(*pDC, kaktusTamni, CRect(5.2 * pomeraj, 7 * pomeraj, 6.8 * pomeraj, 10 * pomeraj));
	//pDC->SetWorldTransform(&oldForm);


	//desni na dole
	/*translate(pDC, 15 * pomeraj, 12 * pomeraj);
	rotate(pDC, 135);
	translate(pDC, -15 * pomeraj, -12 * pomeraj);

	PlayEnhMetaFile(*pDC, kaktusTamni, CRect(14 * pomeraj, 9 * pomeraj, 16 * pomeraj, 12 * pomeraj));*/

	//pDC->SetWorldTransform(&oldForm);






	//polozeni
	/*translate(pDC, 12 * pomeraj, 12 * pomeraj);
	rotate(pDC, 90);
	translate(pDC, -12 * pomeraj, -12 * pomeraj);

	PlayEnhMetaFile(*pDC, kaktusTamni, CRect(11 * pomeraj, 9 * pomeraj, 13 * pomeraj, 12 * pomeraj));

	pDC->SetWorldTransform(&oldForm);*/

	//krug do polozenog
	


	//desni na dole
	/*translate(pDC, 15 * pomeraj, 12 * pomeraj);
	rotate(pDC, 135);
	translate(pDC, -15 * pomeraj, -12 * pomeraj);

	PlayEnhMetaFile(*pDC, kaktusTamni, CRect(14.2 * pomeraj, 9 * pomeraj, 15.8 * pomeraj, 12 * pomeraj));*/

	pDC->SetWorldTransform(&oldForm);

	//listic koji se pomera za novi ugao
	//njega moramo poslednjeg da crtamo 
	//jer da smo ga gore nacrtali ne bi smelo 
	// pDC->SetWorldTransform(&oldForm); da se koristi nadalje
	translate(pDC, 12 * pomeraj, 12 * pomeraj);
	rotate(pDC, 45);
	translate(pDC, -12 * pomeraj, -12 * pomeraj);

	PlayEnhMetaFile(*pDC, kaktusTamni, CRect(11.5 * pomeraj, 9 * pomeraj, 12.5 * pomeraj, 12 * pomeraj));

	pDC->SetWorldTransform(&oldForm);

	nacrtajKrug(pDC, { 15 * pomeraj, 12 * pomeraj }, pomeraj);

	//desni na gore
	translate(pDC, 15 * pomeraj, 12 * pomeraj);
	rotate(pDC, 90);
	translate(pDC, -15 * pomeraj, -12 * pomeraj);

	translate(pDC, 15 * pomeraj, 12 * pomeraj);
	rotate(pDC, ugaoSrednji);
	translate(pDC, -15 * pomeraj, -12 * pomeraj);

	PlayEnhMetaFile(*pDC, kaktusSvetli, CRect(13.8 * pomeraj, 9 * pomeraj, 16.4 * pomeraj, 12 * pomeraj));


	//pDC->SetWorldTransform(&oldForm);


}

void CMFCApplication2View::napisiFont(CDC* pDC)
{

	XFORM form, oldForm;
	pDC->GetWorldTransform(&oldForm);

	CString string("18126 Andrijana Dejkovic");
	CFont* font = new CFont();
	font->CreateFontW(1.3 * pomeraj, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, L"Arial");

	CFont* oldFont = pDC->SelectObject(font);
	CPoint pozicija(19 * pomeraj, 19* pomeraj/*477, 20*/);

	pDC->SetBkMode(TRANSPARENT);
	double ugao = 3.14;
	form = {
		cosf(ugao),
		sinf(ugao),
		-sinf(ugao),
		cosf(ugao),										//objedinjeno transform i rotate? ja
		(FLOAT)pozicija.x,
		(FLOAT)pozicija.y
	};
	pDC->SetWorldTransform(&form);
	form = {
		1,
		0,
		0,
		1,
		(FLOAT)-pozicija.x,
		(FLOAT)-pozicija.y								//transform back? ja
	};
	pDC->ModifyWorldTransform(&form, MWT_LEFTMULTIPLY);

	COLORREF oldColor = pDC->SetTextColor(RGB(250, 250, 0));

	pDC->TextOutW(pozicija.x, pozicija.y, string);
	pDC->SetWorldTransform(&oldForm);
	pDC->SelectObject(oldFont);
	pDC->SetTextColor(oldColor);

	font->DeleteObject();

	font = nullptr;
}
// MFCApplication2View printing

BOOL CMFCApplication2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMFCApplication2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMFCApplication2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// MFCApplication2View diagnostics

#ifdef _DEBUG
void CMFCApplication2View::AssertValid() const
{
	CView::AssertValid();
}

void CMFCApplication2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCApplication2Doc* CMFCApplication2View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplication2Doc)));
	return (CMFCApplication2Doc*)m_pDocument;
}
#endif //_DEBUG


// MFCApplication2View message handlers

void CMFCApplication2View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == VK_SPACE) {
		if (grid == 1)
			grid = 0;
		else
			grid = 1;
		Invalidate();
	}
	else if (nChar == VK_RIGHT) {
		ugaoSrednji -= 30;
		Invalidate();
	}
	else if (nChar == VK_LEFT) {
		ugaoSrednji += 30;
		Invalidate();
	}
	else if (nChar == VK_UP) {
		ugaoNovi += 30;
		Invalidate();
	}
	else if (nChar == VK_DOWN) {
		ugaoNovi -= 30;
		Invalidate();
	}
	/*else if (nChar == 'Y') {
		ugaoDela += 5;
		Invalidate();
	}
	else if (nChar == 'X') {
		ugaoDela -= 5;
		Invalidate();
	}*/													//ja zakomentarisao
	CView::OnKeyDown(nChar, nRepCnt, nFlags);			//jel treba ovo? ja
}
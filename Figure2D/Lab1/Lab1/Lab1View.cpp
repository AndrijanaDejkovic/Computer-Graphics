
// Lab1View.cpp : implementation of the CLab1View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Lab1.h"
#endif

#include "Lab1Doc.h"
#include "Lab1View.h"
#include <iostream>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PI 3.14
// CLab1View

IMPLEMENT_DYNCREATE(CLab1View, CView)

BEGIN_MESSAGE_MAP(CLab1View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CLab1View construction/destruction

CLab1View::CLab1View() noexcept
{
	// TODO: add construction code here
	this->grid_size = 25;
	this->key_pressed = false;

}

CLab1View::~CLab1View()
{
}

BOOL CLab1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void CLab1View::DrawRegularPolygon(CDC* pDC, float cx, float cy, float r, int n, float rotAngle) {
	double angle_increment = 2 * PI / n;
	double mainRadius = r * this->grid_size;
	CPoint* points = new CPoint[n];
	for (int i = 0; i < n; i++)
	{
		double xcoord = (mainRadius * cos(rotAngle)) + (double)(cx * this->grid_size);
		double ycoord = (mainRadius * sin(rotAngle)) + (double)(cy * this->grid_size);
		points[i] = CPoint(xcoord, ycoord);
		rotAngle += angle_increment;
	}
	CGdiObject* old_brush = pDC->SelectStockObject(NULL_BRUSH);
	pDC->Polygon(points, n);
	pDC->SelectObject(old_brush);
	delete[] points;
}

// CLab1View drawing

void CLab1View::OnDraw(CDC* pDC)
{
	CLab1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	int n = this->grid_size;

	//Pravougaonik u kome crtam
	CBrush container_brush(RGB(211, 211, 211));
	CBrush* old_brush = (CBrush*)pDC->SelectObject(&container_brush);

	CRect container = new CRect(0, 0, this->grid_size * 20, this->grid_size * 20);
	pDC->Rectangle(container);
	pDC->SelectObject(old_brush);
	//Granica izmedju svih objekata
	CPen* purple_pen = new CPen(PS_SOLID, 5, RGB(255, 0, 0));
	CPen* old_pen = (CPen*)pDC->SelectObject(purple_pen);
	//Zuti trougao
	CBrush yellow_brush(RGB(255, 0, 0));
	old_brush = (CBrush*)pDC->SelectObject(&yellow_brush);
	POINT yellow_triangle_points[] = { { n,19 * n},{ n, 8 * n},{ 7* n, 14 * n} };
	pDC->Polygon(yellow_triangle_points, 3);
	pDC->SelectObject(old_brush);
	//Zeleni kvadrat
	CBrush green_brush(RGB(255, 165, 0));
	old_brush = (CBrush*)pDC->SelectObject(&green_brush);
	POINT green_square[] = { {n,19 * n},{7 * n,14 * n},{12 * n, 19 * n} };
	pDC->Polygon(green_square, 3);
	pDC->SelectObject(old_brush);
	//Srafirani trougo
	//CPen* blue_pen =  new CPen(PS_SOLID, 6, RGB(0, 0, 255));
	//CPen* old_pen = (CPen*)pDC->SelectObject(&blue_pen);
	CBrush* striped_brush = new CBrush(RGB(0, 255, 0));
	old_brush = (CBrush*)pDC->SelectObject(striped_brush);
	POINT striped_triangle[] = { {5.1 * n, 12 * n},{8 * n, 9.1 * n},{ 8 * n, 15 * n}};
	pDC->Polygon(striped_triangle, 3);
	pDC->SelectObject(old_brush);
	//Ljubicasti trougao
	CBrush purple_brush(RGB(255, 182, 193));
	old_brush = (CBrush*)pDC->SelectObject(&purple_brush);
	POINT purple_triangle[] = { {8 * n, 9.1 * n},{8 * n,15 * n},{10.7 * n,12 * n}, {10.7 * n, 6.3 * n } };
	pDC->Polygon(purple_triangle, 4);
	pDC->SelectObject(old_brush);
	//Narandzasti trougao
	CBrush orange_brush(HS_BDIAGONAL, RGB(0, 0, 255));
	old_brush = (CBrush*)pDC->SelectObject(&orange_brush);
	POINT orange_triangle[] = { {10.7 * n,12 * n},{10.7 * n,6.3 * n},{16 * n,6.3 * n}};
	pDC->Polygon(orange_triangle, 3);
	pDC->SelectObject(old_brush);
	//Roze trougao
	CBrush pink_brush(RGB(255, 0, 255));
	old_brush = (CBrush*)pDC->SelectObject(&pink_brush);
	POINT pink_triangle[] = { {13.5 * n, 3.8 * n},{10.7 * n, 6.3 * n}, {16 * n, 6.3 * n} };
	pDC->Polygon(pink_triangle, 3);
	pDC->SelectObject(old_brush);
	//Crveni poligon
	CBrush red_brush(RGB(255, 255, 0));
	old_brush = (CBrush*)pDC->SelectObject(&red_brush);
	POINT red_polygon[] = { {13.5 * n, 3.8 * n},{16.1 * n,6.3 * n}, { 18.8 * n, 3.8 * n} ,{16.2 * n, 1.2* n} };
	pDC->Polygon(red_polygon, 4);
	pDC->SelectObject(old_brush);
	//boja za poligone
	//Odradi poligone u centru

	CPen* pen = new CPen(PS_SOLID, 2, RGB(255, 0, 0));
	old_pen = (CPen*)pDC->SelectObject(pen);

	DrawRegularPolygon(pDC, 6.8, 16.8, 0.9, 5, 30);
	DrawRegularPolygon(pDC, 6.9, 12.2, 0.5, 8, 30);
	DrawRegularPolygon(pDC, 12.3, 7.9, 1, 6, 30);
	DrawRegularPolygon(pDC, 13.5, 5.2, 0.5, 7, 30);
	//DrawRegularPolygon();

	//GRID
	CPen* gridPen = new CPen(PS_SOLID, 2, RGB(230, 230, 230)); //nzm kako da napravim polu transparentnu belu olovku
	old_pen = pDC->SelectObject(gridPen);
	if (this->key_pressed)
	{
		for (long x = 0; x < this->grid_size * 21; x = x + this->grid_size)
		{
			pDC->MoveTo(x, 0);
			pDC->LineTo(x, this->grid_size * 20);
		}
		for (long y = 0; y < this->grid_size * 21; y = y + this->grid_size)
		{
			pDC->MoveTo(0, y);
			pDC->LineTo(this->grid_size * 20, y);
		}
	}
	pDC->SelectObject(old_pen);
}


// CLab1View printing

BOOL CLab1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLab1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLab1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CLab1View diagnostics

#ifdef _DEBUG
void CLab1View::AssertValid() const
{
	CView::AssertValid();
}

void CLab1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLab1Doc* CLab1View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLab1Doc)));
	return (CLab1Doc*)m_pDocument;
}
#endif //_DEBUG

void CLab1View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_SPACE)
	{
		this->key_pressed = !this->key_pressed;
		Invalidate();
	}
	//else if (nChar == VK_UP)
	//{
	//	this->grid_size += 5;
	//	//Invalidate();
	//}
	//else if (nChar == VK_DOWN)
	//{
	//	if (this->grid_size != 5)
	//	{
	//		this->grid_size -= 5;
	//		//Invalidate();
	//	}
	//}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


// LV03View.cpp : implementation of the CLV03View class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "LV03.h"
#endif

#include "LV03Doc.h"
#include "LV03View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define gridSize 25

// CLV03View

IMPLEMENT_DYNCREATE(CLV03View, CView)

BEGIN_MESSAGE_MAP(CLV03View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CLV03View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CLV03View construction/destruction

CLV03View::CLV03View()
{

	part = new DImage*[3];
	for (int i = 0; i < 3; i++)
		part[i] = new DImage[3];

	//fajlovi moraju da budu u folderu LV03, ne van gde je sln
	part[0][0].Load(CString("Bitmaps2/8.dib"));
	part[0][1].Load(CString("Bitmaps2/2.dib"));
	part[0][2].Load(CString("Bitmaps2/3.dib"));
	part[1][0].Load(CString("Bitmaps2/4.dib"));
	part[1][1].Load(CString("Bitmaps2/5.dib"));
	part[1][2].Load(CString("Bitmaps2/6.dib"));
	part[2][0].Load(CString("Bitmaps2/7.dib"));
	part[2][1].Load(CString("Bitmaps2/1.dib"));
	part[2][2].Load(CString("Bitmaps2/9.dib"));

	for (int i = 0; i < 3; i++)
	for (int j = 0; j < 3; j++)
	if (i == 1 && j == 0)
		MakeItRed(&part[i][j]); // moja funkcija za bojenje u plavo, mislim da ima i on ali nisam koristila 
	
	else
		MakeItGray(&part[i][j]); //u DImage klasi ne postoji ova fja kad skines sa cs-a
}

CLV03View::~CLV03View()
{
	for (int i = 0; i < 3; i++)
		delete[] part[i];
	delete[] part;
}



// CLV03View drawing

void CLV03View::OnDraw(CDC* pDC)
{
	CLV03Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//Kreiramo MemDC u koji ce da se sve iscrtava
	CRect clientWindow;
	GetClientRect(&clientWindow);

	//Implementirati "Anti-Flicker" tj.eliminaciju treperenja koje nastaje usled osvežavanja prozora aplikacije.Ovo se postiže kreiranjem memorijskog DC-ja .....
	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);

	// (i odgovarajuće kompatibilne bitmape) na kome se vrši celokupno iscrtavanje
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, clientWindow.Width(), clientWindow.Height());

	int oldGMode = pDC->SetGraphicsMode(GM_ADVANCED); //da omogucim fancy transformacije

	//Trebace za lab
	//Uspravna Mona Liza:
	//Rotate(pDC, 0 /*stepeni*/, false /*uvek svuda false*/);
	//Translate(pDC, 0, 0, false);

	//Mona Liza na dole:
	//Rotate(pDC, -180 /*stepeni*/, false /*uvek svuda false*/);
	//Translate(pDC, -500, -500, false);

	//Glava Mona lize ide na desno: 
	//Rotate(pDC, 90, false);
	//Translate(pDC, 0, -500, false);

	//Glava mona lize ide na levo:
	Rotate(pDC, -90, false);
	Translate(pDC, -500, 0, false);

	//crtamo celu sliku
	SetPuzzle(pDC, &bmp);

	delete memDC;
}

void CLV03View::SetPuzzle(CDC* pDC, CBitmap* bmp)
{
	//selektujemo bitmapu u kojoj crtamo
	pDC->SelectObject(bmp);
	DrawGrid(pDC);
	//====================
	int oldGMode = pDC->SetGraphicsMode(GM_ADVANCED);
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	//Ukoliko hoces sa true(da citas transformacije odozgo na dole) samo obrni redosled komandi,
	//ali lakse je sa false(da citas transoformacije odzdo na gore) jer prvo mozes vidis sliku pa da transformises, a ne prvo da radis transformacije pa onda da nagadjas gde ce slika da bude <3

	//0,0

	//stelovanje kordinata
	Translate(pDC, 128 - 34, 128 - 32, false); //transliramo ceo koordinatni sistem, ali biramo koordinate kao da koord. sistem uopste nije bio rotiran ili mirrorovan... x raste na desno i y na levo
	Mirror(pDC, true, false, false); //obrnemo ceo koordinatni sistem po x osi "in-place"
	Rotate(pDC, 41, false); //rotiramo ceo sistem
	Translate(pDC, -(128), -(128), false); //drugo, transliramo ceo sistem, a posto je slika 256x256, mi premestili sliku u centar

	DrawTransparent(pDC, &part[2][0], 0, 0); //prvo se ovo crta (jer smo svuda stavili false)  //OK
	pDC->SetWorldTransform(&oldForm);

	//0,1


	Translate(pDC, 142, 6, false); //6.
	Translate(pDC, 128 - 22, 128 - 25, false); //5.
	Rotate(pDC, -142, false); //4.
	Mirror(pDC, true, false, false); //3.
	Translate(pDC, -(128), -(128), false); //2.

	DrawTransparent(pDC, &part[2][2], 0, 0); //1.   //OK
	pDC->SetWorldTransform(&oldForm);

	//0,2 0,0

	Translate(pDC, 2 * 144, 6, false);
	Translate(pDC, 128 - 22, 128 - 28, false);
	Rotate(pDC, 239, false);
	Mirror(pDC, true, false, false);
	Translate(pDC, -(128), -(128), false);

	DrawTransparent(pDC, &part[0][0], 0, 0);  //POSOLEDNJE
	pDC->SetWorldTransform(&oldForm);

	//1,0  2,1

	Translate(pDC, 3, 162, false);
	Translate(pDC, 128 -25, 128 - 35, false);
	Rotate(pDC, 163, false);
	Mirror(pDC, true, false, false);
	Translate(pDC, -(128), -(128), false);

	DrawTransparent(pDC, &part[2][1], 0, 0);
	pDC->SetWorldTransform(&oldForm);

	//1,1

	Translate(pDC, 138, 149, false);
	Translate(pDC, 128 - 22, 128 - 21, false);
	Rotate(pDC, -120, false);
	Mirror(pDC, true, false, false);
	Translate(pDC, -(128), -(128), false);

	DrawTransparent(pDC, &part[1][0], 0, 0);
	pDC->SetWorldTransform(&oldForm);

	//1,2

	Translate(pDC, 2 * 147, 150, false);
	Translate(pDC, 128 - 30, 128 -25, false);
	Rotate(pDC, -90, false);
	Mirror(pDC, true, false, false);
	Translate(pDC, -(128), -(128), false);

	DrawTransparent(pDC, &part[0][2], 0, 0); //OK
	pDC->SetWorldTransform(&oldForm);

	//2,0

	Translate(pDC, 4, 2 * 151.3, false);
	Translate(pDC, 128 - 24, 128 - 32, false);
	Rotate(pDC, -250, false);
	Mirror(pDC, true, false, false);
	Translate(pDC, -(128), -(128), false);

	DrawTransparent(pDC, &part[1][2], 0, 0); //OK
	pDC->SetWorldTransform(&oldForm);

	//2,1  1,0

	Translate(pDC, 151, 2 * 153, false);
	Translate(pDC, 128 - 34, 128 - 27, false);
	Rotate(pDC, -129, false);
	Mirror(pDC, true, false, false);
	Translate(pDC, -(128), -(128), false);

	DrawTransparent(pDC, &part[0][1], 0, 0);  //OK
	pDC->SetWorldTransform(&oldForm);

	//2,2

	Translate(pDC, 2 * 146.5, 2 * 153.5, false);
	Translate(pDC, 128 - 30, 128 - 37, false);
	Rotate(pDC, 303,  false);
	Mirror(pDC, true, false, false);
	Translate(pDC, -(128), -(128), false);

	DrawTransparent(pDC, &part[1][1], 0, 0); //OK
	pDC->SetWorldTransform(&oldForm);

	pDC->SetGraphicsMode(oldGMode);
}
void CLV03View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM form =
	{
		1,
		0,
		0,
		1,
		dX,
		dY
	};
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CLV03View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM form =
	{
		sX,
		0,
		0,
		sY,
		0,
		0
	};
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CLV03View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	XFORM form =
	{
		cosf(angle * stepAngle),
		sinf(angle * stepAngle),
		-sinf(angle * stepAngle),
		cosf(angle * stepAngle),
		0,
		0
	};
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CLV03View::Mirror(CDC* pDC, bool mx, bool my, bool rightMultiply)
{
	XFORM form =
	{
		mx ? -1 : 1,
		0,
		0,
		my ? -1 : 1,
		0,
		0
	};
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CLV03View::DrawGrid(CDC* memDC)
{
	CPen* pen = new CPen(PS_SOLID, 1, RGB(1, 1, 1));
	CPen* oldPen = memDC->SelectObject(pen);
	for (int p = 25; p <= 500; p = p + 25)
	{
		memDC->MoveTo(p, 0);
		memDC->LineTo(p, 500);
		memDC->MoveTo(0, p);
		memDC->LineTo(500, p);
	}
	memDC->SelectObject(oldPen);
	delete pen;
}


//kopiranje svega sa memDC na pDC. Zasto? zbog antiflickering.. pDC je skroz beo, a u memDC je nasa finalna nacrtana slika
void CLV03View::MemDCTopDC(CDC* memDC, CDC* pDC)
{
	CRect clientWindow;
	GetClientRect(&clientWindow);  //dimenzije pravouganika \clienta u clientWindow
	//Kreiramo masku
	CBitmap mask;
	mask.CreateBitmap(clientWindow.Width(), clientWindow.Height(), 1, 1, NULL); // Pravljenje monohromatske bitmape
	CDC* dstDC = new CDC();
	dstDC->CreateCompatibleDC(memDC);
	dstDC->SelectObject(&mask);
	//Postavljamo memDC boje
	memDC->SetBkColor(RGB(0, 0, 0));
	memDC->SetTextColor(RGB(255, 255, 255)); //Sada je kod memDC-a Mona liza bela?????, a pozadina crna 
	//pravimo masku                                                                //COPY ili OR, svejedno
	dstDC->BitBlt(0, 0, clientWindow.Width(), clientWindow.Height(), memDC, 0, 0, SRCCOPY); //sve sa memDC je OR-ovano na masku. Background maske je beo,nastali content dstDC-a je crn . dstDC->mask 
	//Radimo pDC AND dstDC, da napravimo masku gde ce da bude iscrtano
	pDC->BitBlt(0, 0, clientWindow.Width(), clientWindow.Height(), dstDC, 0, 0, SRCAND); //pDC=sve belo  OR  memDC=(bg->beo i content->crn). Sad kod pDCa=(Crna mona liza sa belom pozadinom). Bukv se maska preslikava na pDC
	delete dstDC;
	//Crtamo sliku                                                                  //OR
	pDC->BitBlt(0, 0, clientWindow.Width(), clientWindow.Height(), memDC, 0, 0, SRCPAINT); //content sa memDC-a(crna pozadina, RGB mona liza)  OR   sa crnom mona lizom  =>   RGB mona liza sa bg bojom pDC-a. Odnosno dobijamo RGB Mona Lizu sa transparentnom pozadinom 
	//Nzm kako se napravi RGB mona liza kad smo stavili da je bela (kao npr sto smo stavili da je avionce da je belo u 27 slajd 1.linija), kao da nekako budu sacuvane boje... CU PITAM ANTOLA
}

void CLV03View::MakeItBlue(DImage* img)
{
	unsigned char* pixelArray = img->GetDIBBits();  //CTRL + levi click na fju da bi video njen opis u DImage.h fajlu
	DWORD sizeOfPicture = img->Width() * img->Height() * img->BPP(); //BitsPerPixel. Moguce je da bude i 1bit kod crno-bele slike.

	//Prvi piksel koji se smatra pozadinskim, za njega ne radimo aritmeticku sredinu
	//Zato sto postoji mogucnost da se pogodi nijansa sa nekim pikselom u slagalici
	//Pa bi deo imao providne piksele tamo gde ne treba

	int firstPixel[3];  //RGB -> 3 bajta
	//prolazimo kroz celu sliku              pokazuje na sl bajt 
	for (DWORD ptr = 0; ptr < sizeOfPicture; ptr += img->BPP())
	{
		bool bg = true; //flag

		for (int i = 0; i < 3; i++)
		{
			if (ptr == 0) //ako ptr pokazue na pocetak odnosno 1. pixel
				firstPixel[i] = pixelArray[i]; //kopiraj vrednosti
			else                                          //&& bg ne mora da se pise
				bg = (firstPixel[i] == pixelArray[ptr + i]) && bg;  //bg = (ako je 1. pixel (pozadina) isto sto i neki drugi pixel) && true
			//bg = true. znaci taj pixel jeste pozadina 
		}


		for (int i = 0; i < 3 && !bg; i++)
		{	//[B=0,G=1,R=2] ukoliko zelis drugu boju/boje da sacuvas i =1,2       
			if (i == 0) //[R,G,B] , ali kad ga cita iz memorije ide [B,G,R] kao sto je reko antol. Taj bajt cuvamo, a ostale odbacujemo i nastace plava slika
				pixelArray[ptr + i] = pixelArray[ptr + i];
			else
				pixelArray[ptr + i] = 0;
		}
	}
	img->Update();  //Update-uje sa vrednostima iz pixelArray niza odnosno ptr-a
}

//trebace za lab
void CLV03View::MakeItRed(DImage* img)
{
	unsigned char* pixelArray = img->GetDIBBits();
	DWORD sizeOfPicture = img->Width() * img->Height() * img->BPP();

	int firstPixel[3];

	// Adjust this factor to control the lightness (1.0 for no change, values > 1.0 for brighter)
	float lightnessFactor = 1.5;

	for (DWORD ptr = 0; ptr < sizeOfPicture; ptr += img->BPP())
	{
		bool bg = true;

		for (int i = 0; i < 3; i++)
		{
			if (ptr == 0)
				firstPixel[i] = pixelArray[i];
			else
				bg = bg && (firstPixel[i] == pixelArray[ptr + i]);
		}

		if (!bg)
		{
			// Increase the intensity of the blue component
			pixelArray[ptr] = static_cast<unsigned char>(pixelArray[ptr] * lightnessFactor);

			// Set the green and red components to zero
			pixelArray[ptr + 1] = 0; // Green component
			pixelArray[ptr] = 0; // blue component
		}
	}

	img->Update();
}



void CLV03View::MakeItGray(DImage* img)  //slicna ko u DImage.cpp samo svetlija siva
{
	unsigned char* pixelArray = img->GetDIBBits();
	DWORD sizeOfPicture = img->Width() * img->Height() * img->BPP();

	//Prvi piksel koji se smatra pozadinskim, za njega ne radimo aritmeticku sredinu
	//Zato sto postoji mogucnost da se pogodi nijansa sa nekim pikselom u slagalici
	//Pa bi deo imao providne piksele tamo gde ne treba

	int firstPixel[3];
	for (DWORD ptr = 0; ptr < sizeOfPicture; ptr += img->BPP())
	{
		int pixel = 0;
		bool bg = true;

		for (int i = 0; i < 3; i++)
		{
			if (ptr == 0)
				firstPixel[i] = pixelArray[i];
			else
				bg = (firstPixel[i] == pixelArray[ptr + i]) && bg;// nzm sto ovaj && bg, po mojoj logici moze i bez toga
			pixel += pixelArray[ptr + i]; //sabiramo da mi smo podelili sa 3.  18i slajd   
		}
		pixel /= 3; //dobijamo monohromatsku sliku, trebalo bi i 64 da se doda
		pixel += 64; //JER TAKO PISE U ZADATAKU 
		if (pixel > 255)
			pixel = 255;

		for (int i = 0; i < 3 && !bg; i++)
			pixelArray[ptr + i] = pixel;
	}
	img->Update();
}

//poenta ove fje je da od RGB slike napravi Monohromatsku sa pDC bg bojom
void CLV03View::DrawTransparent(CDC* pDC, DImage* img, int x, int y)
{
	//Kreiramo masku sa 1 bit po pikselu sto je monohromatska
	CBitmap mask;
	mask.CreateBitmap(img->Width(), img->Height(), 1, 1, NULL);
	//Kopiramo bajtove iz DImage u Bitmapu(RGB) sa kojom radimo
	CBitmap image;                       //broj color ravni?? br bitova po pixelu 
	image.CreateBitmap(img->Width(), img->Height(), 4, 8, NULL);
	//1=crno-bela, 8=RGB   ptr na bitove img-a
	image.SetBitmapBits(img->Width() * img->Height() * img->BPP(), img->GetDIBBits());

	CDC* srcDC = new CDC();  //RGB slika  (avionce sa crveni bg (sa prez))
	CDC* dstDC = new CDC();  //monohromtsko avioncee
	srcDC->CreateCompatibleDC(pDC);
	dstDC->CreateCompatibleDC(pDC);

	CBitmap* srcOldBitmap = srcDC->SelectObject(&image);
	CBitmap* dstOldBitmap = dstDC->SelectObject(&mask);

	//Pravimo masku 
	//30:10 https://www.youtube.com/watch?v=hUm_KS8FG4I&list=PLWLPHZCdUNsOh4telXXzBG_KMSikT_MTm&index=11&ab_channel=RII1999
	//Bukv sve isto ko sa prezentacija
	COLORREF removeColor = srcDC->GetPixel(0, 0); //1. pixel == pozadinska boja
	COLORREF srcOldBckColor = srcDC->SetBkColor(removeColor); //1. pixel setujemo kao ceo background! 

	//RGB slika u Monohromatsku. Automatski se kod nje bg color pretvara u belu boju, ostalo u crnu
	dstDC->BitBlt(0, 0, img->Width(), img->Height(), srcDC, 0, 0, SRCCOPY);                          //sada je dstDC crno avionce sa belom pozadinom. 26 slajd
	//kopiramo img sa srcDC na dstDC 

	//Menjamo boje i za bk i text i odstranjujemo pozadinsku boju kod RGB slike
	COLORREF srcOldTextColor = srcDC->SetTextColor(RGB(255, 255, 255)); //RGB slika: avionce je belo
	srcDC->SetBkColor(RGB(0, 0, 0)); //RGB slika: bg je crn
	//Monohromatska u RGB sliku! bg crna && bg bela => bg crna       avionce bela && avionce crna(ali msm da nije skroz crn, vec da su nekako sacuvane boje...)  =>  avionce RGB 
	srcDC->BitBlt(0, 0, img->Width(), img->Height(), dstDC, 0, 0, SRCAND); //sada je avionce RGB, a pozadina crna.  27 slajd

	dstDC->SelectObject(dstOldBitmap);
	delete dstDC; //oslobadjamo resurse, 28 sladj


	srcDC->SelectObject(&mask);
	//pDC=memDC jer smo tako prosledili u fji
	pDC->BitBlt(x, y, img->Width(), img->Height(), srcDC, 0, 0, SRCAND); //Iscrtavamo masku na pDC.-bela boja(pozadina) && monohromatska slika(maska==(crno avionce sa bg belom)) => crno avionce, beli bg

	srcDC->SelectObject(&image);                                //OR
	pDC->BitBlt(x, y, img->Width(), img->Height(), srcDC, 0, 0, SRCPAINT); //RGB slika i transparentna pozadina(u nasem slucaju bela)
	srcDC->SelectObject(srcOldBitmap);
	delete srcDC;
}



///// NEBITNO JE OVO DOLE 

BOOL CLV03View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}
void CLV03View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CLV03View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLV03View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLV03View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CLV03View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CLV03View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CLV03View diagnostics

#ifdef _DEBUG
void CLV03View::AssertValid() const
{
	CView::AssertValid();
}

void CLV03View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLV03Doc* CLV03View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLV03Doc)));
	return (CLV03Doc*)m_pDocument;
}
#endif //_DEBUG


// CLV03View message handlers DODAO
BOOL CLV03View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
}
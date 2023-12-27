#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#pragma comment(lib, "GL\\glut32.lib") //ovo je bilo zakomentarisano
#pragma comment(lib,"GL\\GLU32.lib")   //ovo smo dodali 

CGLRenderer::CGLRenderer(void)
{
	this->angle_v = 0;
	this->angle_h = 0;
	this->angle_cx = 0;
	this->angle_cy = 0;
}

CGLRenderer::~CGLRenderer(void)
{
}

bool CGLRenderer::CreateGLContext(CDC* pDC)  //Celo Template
{
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24; 
	pfd.iLayerType = PFD_MAIN_PLANE;

	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);

	if (nPixelFormat == 0) return false;

	BOOL bResult = SetPixelFormat(pDC->m_hDC, nPixelFormat, &pfd);

	if (!bResult) return false;

	m_hrc = wglCreateContext(pDC->m_hDC);

	if (!m_hrc) return false;

	return true;
}

void CGLRenderer::PrepareScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClearColor(0.00, 0.80, 0.00, 0.0); //POZADINA, DOBICES JE NA CS!!!
	glEnable(GL_DEPTH_TEST);  //uklucujes neki z buffer kako bi crtao objekte po redu, a ne obrnuto.. nije tlk bitno

	//---------------------------------

	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC* pDC) //najbitnija, crta se nesto konkretno
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //S. brisanje buffera 
	glMatrixMode(GL_MODELVIEW); //context openGL-a je setovan na ovaj jedan od 2 moda i sve transformacije se odnose na ovaj mod 
	glLoadIdentity(); //S. podesava transformaciju na jedinicnu transformaciju
	glRotatef(angle_h, .0, 0, 1.0); //omogucava pomeranje misem po celoj sceni
	glRotatef(angle_v, 0, 1.0, .0);

	DrawAxis(); //BOJE MENJAS SAMO
	DrawGrid(); //BOJE MENJAS SAMO
	
	glPushMatrix(); //da jednom transformacijom ne uticemo na drugi objekat, tu transformaciju pushamo na stack

	//Posto je prvobitno sve nacrtano u vecoj razmeri, samo je skalirano na polovinu
	glScalef(0.55, 0.55, 0.55);
	
	DrawPot();     //CE MENJAS BOJU VAZE
	DrawFigure();  //100% CE MENJAS KAKTUS


	glPopMatrix();
	glFlush();
	//---------------------------------
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC* pDC, int w, int h) //isto kao sa prezentacija
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, (double)w / (double)h, 1, 100);
	gluLookAt(3.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);

	//---------------------------------
	wglMakeCurrent(NULL, NULL); 
}

void CGLRenderer::DestroyScene(CDC* pDC)  //celo T
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 
	wglMakeCurrent(NULL, NULL);
	if (m_hrc)
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::DrawPrism(float radius, float height, int sides) //Gornji deo vaze samo?
{

	float t_height = height / 2.0;

	glPushMatrix();
	glRotatef(45, 0, 1, 0);
	float step = (2 * C_PI) / sides;
	//Baza 1
	glBegin(GL_POLYGON);
	for (int i = 0; i < sides; i++)
		glVertex3f(get_x(radius, i * step), t_height, get_y(radius, i * step));
	glEnd();

	//Baza 2
	glBegin(GL_POLYGON);
	for (int i = 0; i < sides; i++)
		glVertex3f(get_x(radius, i * step), -t_height, get_y(radius, i * step));
	glEnd();

	//Omotac
	
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < sides; i++) {
		glVertex3f(get_x(radius, i * step), -t_height, get_y(radius, i * step));
		glVertex3f(get_x(radius, (i + 1) * step), -t_height, get_y(radius, (i + 1) * step));
		glVertex3f(get_x(radius, i * step), t_height, get_y(radius, i * step));
		glVertex3f(get_x(radius, (i + 1) * step), t_height, get_y(radius, (i + 1) * step));
	}
	glEnd();
	
	glPopMatrix();
}

void CGLRenderer::DrawCone(float radius, float height, int sides)
{
	float step = (2 * C_PI) / sides;
	float t_height = height / 2.0;

	//Baza
	glBegin(GL_POLYGON);
	for (int i = 0; i < sides; i++)
		glVertex3f(get_x(radius, i * step), -t_height, get_y(radius, i * step));
	glEnd();

	//Omotac
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < sides; i++)
	{
		glVertex3f(get_x(radius, i * step), -t_height, get_y(radius, i * step));
		glVertex3f(0, t_height, 0);
		glVertex3f(get_x(radius, (i + 1) * step), -t_height, get_y(radius, (i + 1) * step));
	}
	glEnd();

}

void CGLRenderer::DrawRoller(float radius, float height, int sides)
{
	float step = (2 * C_PI) / sides;
	float t_height = height / 2.0;

	//Baza 1
	glBegin(GL_POLYGON);
	for (int i = 0; i < sides; i++)
		glVertex3f(get_x(radius, i * step), t_height, get_y(radius, i * step));
	glEnd();

	//Baza 2
	glBegin(GL_POLYGON);
	for (int i = 0; i < sides; i++)
		glVertex3f(get_x(radius, i * step), -t_height, get_y(radius, i * step));
	glEnd();


	//Omotac
	glBegin(GL_QUADS);
	for (int i = 0; i < sides; i++) {
		glVertex3f(get_x(radius, i * step), -t_height, get_y(radius, i * step));
		glVertex3f(get_x(radius, (i + 1) * step), -t_height, get_y(radius, (i + 1) * step));
		glVertex3f(get_x(radius, i * step), t_height, get_y(radius, i * step));
		glVertex3f(get_x(radius, (i + 1) * step), t_height, get_y(radius, (i + 1) * step));
	}
	glEnd();

}

void CGLRenderer::DrawSphere(float radius, float lats, float longs)
{
	int i, j;
	for (i = 0; i <= lats; i++) {
		double alfa = C_PI * (-0.5 + (double)(i - 1)  / lats);
		double z0 = sin(alfa);
		double zr0 = cos(alfa);// radius za jedan smer

		double beta = C_PI * (-0.5 + (double)i / lats);
		double z1 = sin(beta);
		double zr1 = cos(beta); // radisu za drugi smer

		glBegin(GL_QUAD_STRIP);
		for (j = 0; j <= longs; j++) {//koliko redova
			double lng = 2 * C_PI * (double)(j - 1) / longs;
			double x = cos(lng);
			double y = sin(lng);

			glNormal3f(x * zr0, y * zr0, z0);
			glVertex3f(radius * x * zr0, radius * y * zr0, radius * z0);
			glNormal3f(x * zr1, y * zr1, z1);
			glVertex3f(radius * x * zr1, radius * y * zr1, radius * z1);
		}
		glEnd();
	}

	
}

void CGLRenderer::DrawAxis()
{ 
	glLineWidth(1.0);

	glBegin(GL_LINES); //pocinjes da crtas
		glColor3f(1.0, 0.0, 0.0);  //boja linke 
		glVertex3f(0.0, 0.0, 0.0); //Vertex-tacka, na (x=0,y=0,z=0) poziciji
		glVertex3f(3.0, 0.0, 0.0); //druga tacka na  (x=3,y=0,z=0)
		//crvena linka. Ono automatski povuce linku nakon nacrtane 2  tacke
		glColor3f(0.0, 1.0, 0.0);   
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 3.0, 0.0);
		//plava linka
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 3.0);
		//zelena linka
	glEnd();
}

void CGLRenderer::DrawGrid()
{
	glLineWidth(1.0);
	glColor3f(0.47 ,0.00, 0.00);  

	glBegin(GL_LINES);
	//0.2 step 0> -1 do 1 = 10
	for (float i = -1; i <= 1; i += 0.2)
	{
		glVertex3f(1.0, 0, i);
		glVertex3f(-1.0, 0, i);

		glVertex3f(i, 0, 1.0);
		glVertex3f(i, 0, -1.0);
	}
	glEnd();
}

void CGLRenderer::SetAngles(float angle_h, float angle_v)
{
	this->angle_v = angle_h;
	this->angle_h = angle_v;
}

float CGLRenderer::get_x(float radius, float angle)
{
	return radius * cosf(angle);
}

float CGLRenderer::get_y(float radius, float angle)
{
	return radius * sinf(angle);
}

void CGLRenderer::DrawPot()
{
	int sides = 8;
	float t_height = 0.4 / 2.0; //u sustini menjaj visinu vaze

	glPushMatrix();

	glTranslatef(0, 0.2, 0); //malo podizes koord sistem (koji se sad zove lokalni?) da bi postavili vazu tik iznad grida, a ne da ga sece

	float step = (2 * C_PI) / sides;

	glColor3f(1.00, 0.60, 0.00); //BOJA, DONJEG DELA VAZE, DOBIJES JE NA CS
	//Baza 1 //gornji deo donjeg dela vaze
	glBegin(GL_POLYGON);
	for (int i = 0; i < sides; i++)
		glVertex3f(get_x(0.45, i * step), t_height, get_y(0.45, i * step));
	glEnd();

	//Baza 2 //donji deo donjeg dela vaze
	glBegin(GL_POLYGON);
	for (int i = 0; i < sides; i++)
		glVertex3f(get_x(0.4, i * step), -t_height, get_y(0.4, i * step));
	glEnd();

	//Omotac
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < sides; i++) {
		glVertex3f(get_x(0.4, i * step), -t_height, get_y(0.4, i * step));
		glVertex3f(get_x(0.4, (i + 1) * step), -t_height, get_y(0.4, (i + 1) * step));
		glVertex3f(get_x(0.45, i * step), t_height, get_y(0.45, i * step));
		glVertex3f(get_x(0.45, (i + 1) * step), t_height, get_y(0.45, (i + 1) * step));
	}
	glEnd();

	//Translirano je vec na 0.2 , dodamo jos 0.2 da postavimo na
	//pocetak donjeg dela + 0.1 da centriramo gornji deo koji je debljine 0.2
	//+ 0.01 cisto da se lepo vidi depth

	glTranslatef(0, 0.3, 0);
	glColor4f(1.0, 0.6, 0.0, 0.5);
	this->DrawPrism(0.5, 0.2, 8); //crta gornji deo vaze

	glPopMatrix();

}

void CGLRenderer::DrawFigure()
{
	//PRVA KUPA
	glColor3f(0.50, 0.75, 1.00); //plavaaaaaa
	glPushMatrix();
	glTranslatef(0, 0.8, 0); //podizemo malo koord sistem da ne bi vaza pojela kupicu
	DrawPrism(0.15, 0.4, 8);  //crtamo onu bas prvu kupu 
	glTranslatef(0, 0.3, 0); //nacrtal, malo gore koord sistem sad
	glColor3f(1.00 ,1.00, 1.00); //braonnnnnnnn
	DrawSphere(0.1, 15, 15); //loptica
	//sistem je na centru loptice

	//MiddleBranch();
	RightBranch();
	LeftBranch();


	glPopMatrix();
}

void CGLRenderer::MiddleBranch()
{

	glPushMatrix(); //cuvamo koord sistem kakav je bio pre ulazka u fju 

	//OVU DOLE FJU CES MORATI DA SAMO POSTAVIS IZNAD NEKOG DELA KAKTUSA KAKO BI SVE TO ROTIARAO
	///////////glRotatef(angle_cx, 1, 0, 0); //postavljamo ove promenljive za rotaciju. Sto znaci da kad se promenljive budu menjale pomocu pomocu tastature, sve figure ce se rotirati koje su nacrtane pod tom rotacijom 
	//glRotatef(angle_cy, 0, 1, 0); //ovu rotaciju ne traze od nas na lab, da se okrece...

	glTranslatef(0, 0.1, 0);
	//ako bude rotacija ove
	/*glRotatef(angle_cx, 1, 0, 0);
	glColor3f(1, 1, 0);*/

	//glColor3f(1, 1, 0);  // ZUTA KUPA. DEFINITVNO MORA STAVIS NEGDE DRUGDE
	glColor3f(0.50, 0.75, 1.00); //plavaaaaaa
	glTranslatef(0, 0.2, 0);
	
	DrawPrism(0.12, 0.4, 8);  //crtamo onu bas prvu kupu 

	glTranslatef(0, 0.3, 0);
	glColor3f(0, 1, 0);
	glColor3f(1.00 ,1.00, 1.00); //braonnnnnnnn
	DrawSphere(0.1, 15, 15);
	glColor3f(0.50, 0.75, 1.00); //plavaaaaaa

	//srednji dep srednje grane
	//glTranslatef(0, 0.3, 0);
	//DrawCone(0.2, 0.4);
	//glTranslatef(0, 0.3, 0);
	//glColor3f(8 / 255.0, 172 / 255.0, 4 / 255.0); //braonnnnnnnn
	//DrawSphere(0.1, 15, 15);
	//glTranslatef(0, -0.6, 0);
	//glColor3f(60 / 255.0, 256 / 255.0, 64 / 255.0); //plavaaaaaa


	//desni deo srednje grane 
	glPushMatrix(); //posle crtanja te grancice, hocemo posle da se vatimo na dnu grane kako bi smo poceli lepo od pocetka da crtamo levu. Posle pomocu glPopMatrix()
	glRotatef(-45, 1, 0, 0); //rotiramo za -45* po x osi 
	glTranslatef(0, 0.3, 0); 
	DrawPrism(0.12, 0.4, 8);  //crtamo onu bas prvu kupu 
	glTranslatef(0, 0.3, 0);

	glColor3f(1.00, 1.00, 1.00); //braonnnnnnnn
	DrawSphere(0.1, 15, 15);
	glColor3f(0.50, 0.75, 1.00); //plavaaaaaa
	glTranslatef(0, 0.3, 0);
	DrawPrism(0.12, 0.4, 8);  //crtamo onu bas prvu kupu 
	glTranslatef(0, 0.3, 0);
	glColor3f(1.00, 1.00, 1.00); //braonnnnnnnn
	DrawSphere(0.1, 15, 15);
	glColor3f(0.50, 0.75, 1.00); //plavaaaaaa
	glPopMatrix();
	
	//levi deo srednje grane
	glPushMatrix();
	glRotatef(angle_cx, 1, 0, 0);
	glColor3f(1, 1, 0);

	glRotatef(45, 1, 0, 0);
	glTranslatef(0, 0.3, 0);
	DrawPrism(0.12, 0.4, 8);  //crtamo onu bas prvu kupu 
	glTranslatef(0, 0.3, 0);
	glColor3f(1.00, 1.00, 1.00); //braonnnnnnnn
	DrawSphere(0.1, 15, 15);
	glColor3f(0.50, 0.75, 1.00); //plavaaaaaa
	glTranslatef(0, 0.3, 0);
	DrawCone(0.12, 0.4);
	glTranslatef(0, 0.3, 0);
	glColor3f(1.00, 1.00, 1.00); //braonnnnnnnn
	DrawSphere(0.1, 15, 15);
	glColor3f(0.50, 0.75, 1.00); //plavaaaaaa
	glPopMatrix();

	glPopMatrix();
}

void CGLRenderer::RightBranch()
{
	glPushMatrix();
	glColor3f(0.50, 0.75, 1.00); //plavaaaaaa  //BOJA
	glRotatef(-45, 1, 0, 0); //rotirani smo za 45 stepeni
	glTranslatef(0, 0.3, 0); //zato sada idemo samo gore po y osi, ali se nama cini kao da idemo po x i y osi tj ukoso
	DrawCone(0.12, 0.4);
	glTranslatef(0, 0.3, 0);
	glColor3f(1.00, 1.00, 1.00); //braonnnnnnnn
	DrawSphere(0.1, 15, 15);
	//glColor3f(0.50, 0.75, 1.00); //plavaaaaaa
	//
	//glTranslatef(0, 0.3, 0);
	//DrawCone(0.12, 0.4);

	//glColor3f(1.00, 1.00, 1.00); //braonnnnnnnn
	//glTranslatef(0, 0.3, 0);
	//DrawSphere(0.1, 15, 15);

	//glColor3f(0.50, 0.75, 1.00); //plavaaaaaa

	//glTranslatef(0, 0.3, 0);
	//DrawCone(0.12, 0.4);

	//glColor3f(1.00, 1.00, 1.00); //braonnnnnnnn
	//glTranslatef(0, 0.3, 0);
	//DrawSphere(0.1, 15, 15);

	//glColor3f(0.5, 0.75, 1.0);
	// 
	//glRotatef(-90, 1, 0, 0); //rotaj skroz udesno
	//glTranslatef(0, 0.3, 0); //idi napred, iznad sfere
	//DrawRoller(0.1, 0.4);    
	//glTranslatef(0, 0.3, 0);
	//DrawSphere(0.1, 15, 15);

	//glTranslatef(0, -0.3, 0);
	//glTranslatef(0, -0.3, 0); //vrati se na pocetak
	//glRotatef(-90, 1, 0, 0);

	glPopMatrix();
}

void CGLRenderer::LeftBranch()
{
	

	glPushMatrix();
	glRotatef(45, 1, 0, 0);
	glTranslatef(0, 0.3, 0);
	glColor3f(0.50, 0.75, 1.00); //plavaaaaaa
	DrawPrism(0.12, 0.4, 8);  //crtamo onu bas prvu kupu 
	glTranslatef(0, 0.3, 0);

	glColor3f(1.00, 1.00, 1.00); //braonnnnnnnn
	DrawSphere(0.1, 15, 15);

	glPushMatrix();
	glColor3f(0.50, 0.75, 1.00); //plavaaaaaa
	//srednji dep srednje grane
	glTranslatef(0, 0.3, 0);
	DrawPrism(0.12, 0.4, 8);
	glTranslatef(0, 0.3, 0);
	glColor3f(1.00, 1.00, 1.00); //braonnnnnnnn
	DrawSphere(0.1, 15, 15);
	//glPushMatrix();
	glRotatef(-45, 1, 0, 0); //rotiramo za -45* po x osi 
	glTranslatef(0, 0.3, 0);
	glColor3f(0.50, 0.75, 1.00); //plavaaaaaa
	DrawPrism(0.12, 0.4, 8);  //crtamo onu bas prvu kupu 
	glTranslatef(0, 0.3, 0);
	glColor3f(1.00, 1.00, 1.00); //braonnnnnnnn
	DrawSphere(0.1, 15, 15);
	glTranslatef(0, -0.6, 0);
	glRotatef(90, 1, 0, 0); //rotiramo za -45* po x osi 
	glTranslatef(0, 0.3, 0);
	glColor3f(0.50, 0.75, 1.00); //plavaaaaaa
	DrawPrism(0.12, 0.4, 8);  //crtamo onu bas prvu kupu 
	glTranslatef(0, 0.3, 0);
	glColor3f(1.00, 1.00, 1.00); //braonnnnnnnn
	DrawSphere(0.1, 15, 15);
	glPopMatrix();
	//glTranslatef(0, -0.6, 0);
	//glColor3f(60 / 255.0, 256 / 255.0, 64 / 255.0); //plavaaaaaa

	//desni deo srednje grane 
	glPushMatrix();//posle crtanja te grancice, hocemo posle da se vatimo na dnu grane kako bi smo poceli lepo od pocetka da crtamo levu. Posle pomocu glPopMatrix()
	glRotatef(-45, 1, 0, 0);//rotiramo za -45* po x osi
	glTranslatef(0, 0.3, 0);
	//glRotatef(-45, 1, 0, 0);//rotiramo za -45* po x osi
	glColor3f(0.50, 0.75, 1.00); //plavaaaaaa
	DrawPrism(0.12, 0.4, 8);  //crtamo onu bas prvu kupu 
	glTranslatef(0, 0.3, 0);
	glColor3f(1.00, 1.00, 1.00); //braonnnnnnnn
	DrawSphere(0.1, 15, 15);
	glRotatef(-45, 1, 0, 0); //rotiramo za -45* po x osi 
	glTranslatef(0, 0.3, 0);
	glColor3f(0.50, 0.75, 1.00); //plavaaaaaa
	DrawPrism(0.12, 0.4, 8);  //crtamo onu bas prvu kupu 
	glTranslatef(0, 0.3, 0);
	glColor3f(1.00, 1.00, 1.00); //braonnnnnnnn
	DrawSphere(0.1, 15, 15);
	glTranslatef(0, -0.6, 0);
	glRotatef(90, 1, 0, 0); //rotiramo za -45* po x osi 
	glTranslatef(0, 0.3, 0);
	glColor3f(0.50, 0.75, 1.00); //plavaaaaaa
	DrawPrism(0.12, 0.4, 8);  //crtamo onu bas prvu kupu 
	glTranslatef(0, 0.3, 0);
	glColor3f(1.00, 1.00, 1.00); //braonnnnnnnn
	DrawSphere(0.1, 15, 15);
	glPopMatrix();

	glPushMatrix();
	glRotatef(angle_cx, 1, 0, 0);
	glColor3f(1, 1, 0.0); //zuta  //BOJA
	glRotatef(45, 1, 0, 0); //rotirani smo za 45 stepeni
	glTranslatef(0, 0.3, 0); //zato sada idemo samo gore po y osi, ali se nama cini kao da idemo po x i y osi tj ukoso
	DrawCone(0.12, 0.4);
	glTranslatef(0, 0.3, 0);
	glColor3f(1.00, 1.00, 1.00); //braonnnnnnnn
	DrawSphere(0.1, 15, 15);
	glTranslatef(0, 0.3, 0);
	glColor3f(0.50, 0.75, 1.00); //plavaaaaaa//zato sada idemo samo gore po y osi, ali se nama cini kao da idemo po x i y osi tj ukoso
	DrawCone(0.12, 0.4);
	glTranslatef(0, 0.3, 0);
	glColor3f(1.00, 1.00, 1.00); //braonnnnnnnn
	DrawSphere(0.1, 15, 15);
	glPopMatrix();





	//glTranslatef(0, 0.3, 0);

	//glPushMatrix();
	//glRotatef(-45, 1, 0, 0);

	//glTranslatef(0, 0.3, 0);
	//DrawPrism(0.1,0.4);
//	glTranslatef(0, 0.3, 0);
	// ne treba

	//DrawSphere(0.1, 15, 15);

	//glTranslatef(0, 0.3, 0);
	//DrawPrism(0.2,0.4);
	//glTranslatef(0, 0.3, 0);

	//DrawSphere(0.1, 15, 15);
	//glPopMatrix();

	//glPushMatrix();

	//glRotatef(angle_cx, 1, 0, 0);
	/*glTranslatef(0, 0.3, 0);

	
	glColor3f(1, 1, 0);
	DrawPrism(.1,0.4);*/


	//glTranslatef(0, 0.3, 0);

	//glColor3f(8 / 255.0, 172 / 255.0, 4 / 255.0); //braonnnnnnnn
	//DrawSphere(0.1, 15, 15);
	//glColor3f(0.50, 0.75, 1.00); //plavaaaaaa

	//glTranslatef(0, 0.3, 0);
	//DrawCone(0.12, 0.4);
	//glTranslatef(0, 0.3, 0);


	//glColor3f(1.00, 1.00, 1.00); //braonnnnnnnn
	//DrawSphere(0.1, 15, 15);
	//glColor3f(0.50, 0.75, 1.00); //plavaaaaaa
	//glPopMatrix();

	//glPushMatrix();
	//glRotatef(45, 1, 0, 0);
	//glTranslatef(0, 0.3, 0);
	//ne treba ovaj blok
	//DrawPrism(0.1, 0.4);
	//glTranslatef(0, 0.3, 0);

	//DrawSphere(0.1, 15, 15);
	//glPopMatrix();
	glPopMatrix();
}
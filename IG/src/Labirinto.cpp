#include <GL/glut.h>
#include <time.h>
#include <stdlib.h>
#include "tga.h"
#include <iostream>
#include <math.h>

using namespace std;

const int dimmatrix=15;
const int dimgraphics=10;
int matrix[dimmatrix][dimmatrix];


int xPlayer, yPlayer;//posizione di start della camera

int riga=0, colonna=0, valiniz=0; //coordinate matrice
enum dir {destra=1, sotto=2, sinistra=3, sopra=4};
enum codir {destr=1, sinistr=2};
GLdouble eyeX=0;
GLdouble eyeY=0;
GLdouble rotate=0;
const double passo=1;
bool endGame=false;
string pathTexture="futureTexture";
bool up=false, down=false, leftb=false, rightb=false;
time_t start = time(0);
double angolo=0;

void StampaMatrix(){
	for (int i=0;i<dimmatrix;i++)
		{
			cout<<endl;
			for (int j=0;j<dimmatrix;j++)
				cout<<matrix[j][i]<<" ";
		}
}

bool CanMove (GLdouble x, GLdouble y){
	int xx=x/dimgraphics;
	int yy=y/dimgraphics;
	if (matrix[yy][xx]==1)
		return false;
	else if(matrix[yy][xx]==2){
		matrix[yy][xx]=3;
		endGame=true;
		return false;
	}
	else if (matrix[yy][xx]==3)
		return false;
	return true;
}


bool can3move(GLdouble x, GLdouble y){
	if (!CanMove(x,y))
		return false;
	x=x+sin(angolo)*1.7;
	y=y+cos(angolo)*1.7;
	if(!CanMove(x+sin(angolo+90)*1.5,y+cos(angolo+90)*1.5)||!CanMove(x+sin(angolo-90)*1.5,y+cos(angolo-90)*1.5)){
		return false;
	}
	x=x+sin(angolo)*1.7;
	y=y+cos(angolo)*1.7;
	if (x<0 || y<0)
		return false;
	if (CanMove(x,y)&&CanMove(x+sin(angolo+90)*2,y+cos(angolo+90)*2)&&CanMove(x+sin(angolo-90)*2,y+cos(angolo-90)*2))
		return true;
	return false;
}


void TargetQuad(int x, int y){
	GLbyte *pBytes;
	GLint iWidth, iHeight, iComponents;
	GLenum eFormat;

	string a = pathTexture + "/Target.tga";
	const char* file = &a[0] ;

	pBytes = gltLoadTGA(file, &iWidth, &iHeight, &iComponents, &eFormat);
	glTexImage2D (GL_TEXTURE_2D, 0 , iComponents , iWidth, iHeight, 0,eFormat, GL_UNSIGNED_BYTE, pBytes );
	free (pBytes);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//la texture si sovrappone al ccolore della geometria
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	glEnable(GL_TEXTURE_2D);

	glPushMatrix();

		glTranslated((y*dimgraphics)+dimgraphics/2,(x*dimgraphics)+dimgraphics/2,0);
		glRotated(rotate,0,0,1);

		glTranslated(-dimgraphics/2+2,-dimgraphics/2+2, 2.0);

		glBegin(GL_QUADS);

		/*
		//face in xy plane
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0,0 ,0 );

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0, dimgraphics, 0);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(dimgraphics, dimgraphics, 0);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(dimgraphics, 0, 0);
		*/

		//face in yz plane
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0, 0, 0);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0, 0, dimgraphics-4);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0, dimgraphics-4, dimgraphics-4);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0, dimgraphics-4, 0);

		//face in zx plance
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0, 0, 0  );

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(dimgraphics-4, 0, 0);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(dimgraphics-4, 0, dimgraphics-4);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0, 0, dimgraphics-4);

		//|| to xy plane.
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0, 0, dimgraphics-4);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(dimgraphics-4, 0, dimgraphics-4);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(dimgraphics-4, dimgraphics-4, dimgraphics-4);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0, dimgraphics-4, dimgraphics-4);


		//|| to yz plane
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(dimgraphics-4,0 ,0 );

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(dimgraphics-4, dimgraphics-4, 0);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(dimgraphics-4, dimgraphics-4, dimgraphics-4);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(dimgraphics-4, 0, dimgraphics-4);

		//|| to zx plane
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0, dimgraphics-4, 0  );

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0, dimgraphics-4, dimgraphics-4);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(dimgraphics-4, dimgraphics-4, dimgraphics-4);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(dimgraphics-4, dimgraphics-4, 0);





		glEnd();
		glFlush();



		glPopMatrix();






}


void TargetSphere(int x, int y){

	GLbyte *pBytes;
	GLint iWidth, iHeight, iComponents;
	GLenum eFormat;

	string a = pathTexture + "/Target.tga";
	const char* file = &a[0] ;

	pBytes = gltLoadTGA(file, &iWidth, &iHeight, &iComponents, &eFormat);
	glTexImage2D (GL_TEXTURE_2D, 0 , iComponents , iWidth, iHeight, 0,eFormat, GL_UNSIGNED_BYTE, pBytes );
	free (pBytes);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//la texture si sovrappone al ccolore della geometria
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	glEnable(GL_TEXTURE_2D);

	glPushMatrix();

	glTranslated((y*dimgraphics)+(dimgraphics/2),(x*dimgraphics)+(dimgraphics/2),(dimgraphics/2));
	glRotated(rotate,0,0,1);

	GLUquadric *quadratic = gluNewQuadric();
	gluQuadricTexture( quadratic, true );

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, iComponents);
	gluSphere(quadratic,(dimgraphics/2)-2,100,100);

	glPopMatrix();
}

void percorsoEsatto(int mat [][dimmatrix], int dim){
	int diminizio=dim-2;
	codir copiadirez=destr;
	dir direzione=sotto;
	srand(time(0));
	valiniz=rand()%diminizio+1;
	colonna=valiniz;
	riga=0;
	mat [riga][colonna]=0;
	xPlayer=riga;
	yPlayer=colonna;
	riga++;
	mat [riga][colonna]=0;
	eyeX=colonna*dimgraphics+dimgraphics/2;

	while (riga!=dim-2)//fino a quando il valore della righa non arriva all'ultima della matrice
	{
		if (direzione==sopra&&copiadirez==destr)
		{
			int ran=0;
			ran=rand()%2+1;
			switch (ran)
			{
			case(1):
							{
				direzione=destra;
				break;
							}
			case (2):
							{
				direzione=sopra;
				break;
							}
			}
			if (direzione==sopra)
				direzione=sopra;
			else
				direzione=destra;
		}
		else if (direzione==sopra&&copiadirez==sinistr)
		{
			int ran=0;
			ran=rand()%2+1;
			switch (ran)
			{
			case (1):
										{
				direzione=sinistra;
				break;
										}
			case (2):
										{
				direzione=sopra;
				break;
										}
			}
			if (direzione==sopra)
				direzione=sopra;
			else
				direzione=sinistra;
		}
		else
		{
			int ran=0;
			ran=rand()%4+1;
			switch (ran)
			{
			case(1):
										{
				direzione=destra;
				break;
										}
			case(2):
										{
				direzione=sotto;
				break;
										}
			case (3):
										{
				direzione=sinistra;
				break;
										}
			case (4):
										{
				direzione=sopra;
				break;
										}
			}
		}

		//inizio verifica direzioni possibili
		bool ver=true;
		if (direzione==destra)//destra
		{
			if (colonna>dim-3)
				ver=false;
			for (int i=riga-1;i<=riga+1;i++)
			{
				for (int j=colonna+1;j<=colonna+2;j++)
				{
					if (mat [i][j]==0)
						ver=false;
				}
			}
			if (ver)
			{
				colonna++;
				mat[riga][colonna]=0;
				copiadirez=destr;
			}
		}
		else if (direzione==sotto)//sotto
		{
			ver=true;
			if (riga>dim-1)
				ver=false;
			for (int i=colonna-1;i<=colonna+1;i++)
			{
				for (int j=riga+1;j<=riga+2;j++)
				{
					if (mat [j][i]==0)
						ver=false;
				}
			}
			if (ver)
			{
				riga++;
				mat [riga][colonna]=0;
			}
		}
		else if (direzione==sinistra)//sinistra
		{
			ver=true;
			if (colonna<2)
				ver=false;
			for (int i=riga-1;i<=riga+1;i++)
			{
				for (int j=colonna-2;j<=colonna-1;j++)
				{
					if (mat [i][j]==0)
						ver=false;
				}
			}
			if (ver)
			{
				colonna--;
				mat [riga][colonna]=0;
				copiadirez=sinistr;
			}
		}
		else//sopra
		{
			ver=true;
			if (riga<3||riga>dim-4||colonna<4||colonna>dim-4)
				ver=false;
			if (copiadirez==destr)
			{
				for (int i=colonna-1;i<=colonna+3;i++)
				{
					for (int j=riga-2;j<=riga-1;j++)
					{
						if (mat[j][i]==0)
							ver=false;
					}
				}
			}
			else//sinistra
			{
				for (int i=colonna-3;i<=colonna+1;i++)
				{
					for (int j=riga-3;j<=riga-1;j++)
					{
						if (mat[j][i]==0)
							ver=false;
					}
				}
			}
			if (ver)
			{
				riga--;
				mat[riga][colonna]=0;
			}
		}
		//fine verifica direzioni possibili
	}//fine while

	riga++;
	mat [riga][colonna]=2;
}

void stradeSecondarie (int mat [][dimmatrix], int dim)
{
	for (int i=1; i<dim-1; i++)
	{
		riga=i;
		for (int j=1; j<dim-1; j++)
		{
			colonna=j;
			if (mat [riga][colonna]==0)
			{
				bool ciclo=false, bodestr=true, bosot=true, bosin=true, bosopr=true;
				dir direzione=sotto;
				while (bosopr||bodestr||bosot||bosin)//fino a che tutti i booleani delle direzioni sono falsi
				{
					int ran=0;
					ran=rand()%4+1;
					switch (ran)
					{
					case(1):
												{
						direzione=destra;
						break;
												}
					case(2):
												{
						direzione=sotto;
						break;
												}
					case (3):
												{
						direzione=sinistra;
						break;
												}
					case (4):
												{
						direzione=sopra;
						break;
												}
					}
					//inizio verifica direzioni possibili
					if (direzione==destra)//destra
					{
						bodestr=true;
						if (colonna>dim-3)
							bodestr=false;
						for (int i=riga-1;i<=riga+1;i++)
						{

							if (mat [i][colonna+1]==0)
								bodestr=false;
						}
						if (mat [riga][colonna+2]==0)
							bodestr=false;
						if (bodestr)
						{
							colonna++;
							mat[riga][colonna]=0;
							ciclo=true;
						}
					}
					else if (direzione==sotto)//sotto
					{
						bosot=true;
						if (riga>dim-3)
							bosot=false;
						for (int i=colonna-1;i<=colonna+1;i++)
						{
							if (mat [riga+1][i]==0)
								bosot=false;
						}
						if (mat [riga+2][colonna]==0)
							bosot=false;
						if (bosot)
						{
							riga++;
							mat [riga][colonna]=0;
							ciclo=true;
						}
					}
					else if (direzione==sinistra)//sinistra
					{
						bosin=true;
						if (colonna<2)
							bosin=false;
						for (int i=riga-1;i<=riga+1;i++)
						{
							if (mat [i][colonna-1]==0)
								bosin=false;
						}
						if (mat [riga][colonna-2]==0)
							bosin=false;
						if (bosin)
						{
							colonna--;
							mat [riga][colonna]=0;
							ciclo=true;
						}
					}
					else//sopra
					{
						bosopr=true;
						if (riga<2||riga>dim-2||colonna<2||colonna>dim-2)
							bosopr=false;

						for (int i=colonna-1;i<=colonna+1;i++)
						{
							if (mat[riga-1][i]==0)
								bosopr=false;
						}
						if (mat[riga-2][colonna]==0)
							bosopr=false;
						if (bosopr)
						{
							riga--;
							mat[riga][colonna]=0;
							ciclo=true;
						}
					}
					//fine verifica direzioni possibili
					if (ciclo)//se ci si è potuti spostare in una qualsiasi direzione
					{
						i=1;
						j=0;
					}
				}//fine while
			}
		}
	}
}

void InizializzaMatrice(){
	for (int i=0;i<dimmatrix;i++)
	{
		for (int j=0;j<dimmatrix;j++)
			matrix[i][j]=1;
	}
}


void init(void)
{
	//RIMOZIONE SUPERFICI NASCOSTE
	glEnable(GL_DEPTH_TEST);

	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_FLAT);

	InizializzaMatrice();

	percorsoEsatto(matrix, dimmatrix);

	stradeSecondarie(matrix, dimmatrix);

}



void Wall(int x, int y){
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);


	GLbyte *pBytes;
	GLint iWidth, iHeight, iComponents;
	GLenum eFormat;


	string a = pathTexture + "/Stone.tga";
	const char* file = &a[0] ;

	pBytes = gltLoadTGA(file, &iWidth, &iHeight, &iComponents, &eFormat);
	glTexImage2D (GL_TEXTURE_2D, 0 , iComponents , iWidth, iHeight, 0,eFormat, GL_UNSIGNED_BYTE, pBytes );
	free (pBytes);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//la texture si sovrappone al ccolore della geometria
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	glEnable(GL_TEXTURE_2D);



	glPushMatrix();

	glTranslated((y*dimgraphics),(x*dimgraphics),0);


	glBegin(GL_QUADS);

	/*
	//face in xy plane
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0,0 ,0 );

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0, dimgraphics, 0);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(dimgraphics, dimgraphics, 0);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(dimgraphics, 0, 0);
	*/

	//face in yz plane
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 0, 0);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0, 0, dimgraphics);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0, dimgraphics, dimgraphics);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0, dimgraphics, 0);

	//face in zx plance
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 0, 0  );

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(dimgraphics, 0, 0);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(dimgraphics, 0, dimgraphics);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0, 0, dimgraphics);

	//|| to xy plane.
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 0, dimgraphics);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(dimgraphics, 0, dimgraphics);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(dimgraphics, dimgraphics, dimgraphics);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0, dimgraphics, dimgraphics);


	//|| to yz plane
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(dimgraphics,0 ,0 );

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(dimgraphics, dimgraphics, 0);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(dimgraphics, dimgraphics, dimgraphics);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(dimgraphics, 0, dimgraphics);

	//|| to zx plane
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, dimgraphics, 0  );

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0, dimgraphics, dimgraphics);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(dimgraphics, dimgraphics, dimgraphics);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(dimgraphics, dimgraphics, 0);

	glEnd();
	glFlush();



	glPopMatrix();

}




void build(){
	for (int i=0; i<dimmatrix; i++){
		for (int j=0; j<dimmatrix; j++){
			if (matrix[i][j]==1){
				Wall(i,j);
			}

			else if (matrix[i][j]==2){
				TargetQuad(i,j);
			}

			else if (matrix[i][j]==3){
				TargetSphere(i,j);
			}

		}
	}
}



void FloorRoof(){


	GLbyte *pBytes;
		GLint iWidth, iHeight, iComponents;
		GLenum eFormat;

		string a = pathTexture + "/Floor.tga";
		const char* file = &a[0] ;

		pBytes = gltLoadTGA(file, &iWidth, &iHeight, &iComponents, &eFormat);
		glTexImage2D (GL_TEXTURE_2D, 0 , iComponents , iWidth, iHeight, 0,eFormat, GL_UNSIGNED_BYTE, pBytes );
		free (pBytes);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//la texture si sovrappone al colore della geometria
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

		glEnable(GL_TEXTURE_2D);



	glPushMatrix();

	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0,0,0);
	glTexCoord2f(10.0f, 0.0f);
	glVertex3f(dimmatrix*dimgraphics,0,0);
	glTexCoord2f(10.0f, 10.0f);
	glVertex3f(dimmatrix*dimgraphics,dimmatrix*dimgraphics,0);
	glTexCoord2f(0.0f, 10.0f);
	glVertex3f(0,dimmatrix*dimgraphics,0);

	glEnd();

	glPopMatrix();


	a = pathTexture + "/Roof.tga";
	file = &a[0] ;

	pBytes = gltLoadTGA(file, &iWidth, &iHeight, &iComponents, &eFormat);
	glTexImage2D (GL_TEXTURE_2D, 0 , iComponents , iWidth, iHeight, 0,eFormat, GL_UNSIGNED_BYTE, pBytes );
	free (pBytes);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//la texture si sovrappone al colore della geometria
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	glEnable(GL_TEXTURE_2D);


	glPushMatrix();

		glBegin(GL_QUADS);

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0,0,dimgraphics);
		glTexCoord2f(0.0f, 10.0f);
		glVertex3f(0,dimmatrix*dimgraphics,dimgraphics);
		glTexCoord2f(10.0f, 10.0f);
		glVertex3f(dimmatrix*dimgraphics,dimmatrix*dimgraphics,dimgraphics);
		glTexCoord2f(10.0f, 0.0f);
		glVertex3f(dimmatrix*dimgraphics,0,dimgraphics);

		glEnd();

		glPopMatrix();
}

void display(void)
{

	double seconds_since_start = difftime( time(0), start);
	if (seconds_since_start>=60){
		if (!endGame)
			endGame=true;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (pathTexture=="futureTexture" && !endGame){
		if (seconds_since_start>60)
			glColor3f (1.0, 1.0, 1.0);
		else if (seconds_since_start>=50)
			glColor3f (1.0, 0.0, 0.0);
		else if (seconds_since_start>=40 && seconds_since_start<50)
			glColor3f (1.0, 0.5, 0.0);
		else if (seconds_since_start>=20 && seconds_since_start<40)
			glColor3f (1.0, 1.0, 0.0);
		else if (seconds_since_start<20)
			glColor3f (0.0, 1.0, 0.0);
	}
	else
		glColor3f (1.0, 1.0, 1.0);

	FloorRoof();

	build();

	glutSwapBuffers();

	rotate+=10;

	if (!endGame){
		if (up){
			eyeX+=sin(angolo);
			eyeY+=cos(angolo);
			if (can3move(eyeX, eyeY)){

				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				gluLookAt (eyeX, eyeY, dimgraphics/2, eyeX+sin(angolo), eyeY+cos(angolo), dimgraphics/2-0.001, 0.0, 0.0, 1.0);
			}
			else{
				eyeX-=sin(angolo);
				eyeY-=cos(angolo);
			}
		}

		if(down){
			eyeX-=sin(angolo);
			eyeY-=cos(angolo);
			if (can3move(eyeX, eyeY)){

				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				gluLookAt (eyeX, eyeY, dimgraphics/2, eyeX+sin(angolo), eyeY+cos(angolo), dimgraphics/2-0.001, 0.0, 0.0, 1.0);
			}
			else{
				eyeX+=sin(angolo);
				eyeY+=cos(angolo);
			}

		}
		if(leftb){
			angolo-=0.1;
			if (can3move(eyeX, eyeY)){
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				gluLookAt (eyeX, eyeY, dimgraphics/2, eyeX+sin(angolo), eyeY+cos(angolo), dimgraphics/2-0.001, 0.0, 0.0, 1.0);
			}
			else
				angolo+=0.1;
		}
		if (rightb){
			angolo+=0.1;
			if (can3move(eyeX, eyeY)){
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				gluLookAt (eyeX, eyeY, dimgraphics/2, eyeX+sin(angolo), eyeY+cos(angolo), dimgraphics/2-0.001, 0.0, 0.0, 1.0);
			}
			else
				angolo-=0.1;
		}
	}

	glutPostRedisplay();

}

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, dimgraphics*dimmatrix+dimgraphics);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	gluLookAt (eyeX, eyeY, dimgraphics/2, eyeX+sin(angolo), eyeY+cos(angolo), dimgraphics/2-0.001, 0.0, 0.0, 1.0);

}

//key pressed
void keyboard (unsigned char key, int x, int y){
	switch (key) {
	case 27:
		exit(0);
		break;
	case '1':
		pathTexture="classicTexture";
		build();
		break;
	case '2':
		pathTexture="futureTexture";
		build();
		break;
	case '3':
		pathTexture="futureStaticTexture";
		build();
		break;
	case 'v':
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		//visione dall'alto al centro
		gluLookAt ((dimgraphics*dimmatrix)/2, (dimgraphics*dimmatrix)/2, 150.0, (dimgraphics*dimmatrix)/2, (dimgraphics*dimmatrix)/2, 0.0, 0.0, 1.0, 0.0);
		break;
	case 'c':
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt (eyeX, eyeY, dimgraphics/2, eyeX+sin(angolo), eyeY+cos(angolo), dimgraphics/2-0.001, 0.0, 0.0, 1.0);
		break;
	default:
		break;
	}
}

//key released
void keyBoardUp( int key, int x, int y ){
	switch( key ) {
	case GLUT_KEY_UP:
		up = false;
		break;
	case GLUT_KEY_DOWN:
		down=false;
		break;
	case GLUT_KEY_LEFT:
		leftb=false;
		break;
	case GLUT_KEY_RIGHT:
		rightb=false;
		break;
	default:
		break;
	}
}

//special-key pressed
void specialKeyboard (int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP:
		up=true;
		break;
	case GLUT_KEY_DOWN:
		down=true;
		break;
	case GLUT_KEY_LEFT:
		leftb=true;
		break;
	case GLUT_KEY_RIGHT:
		rightb=true;
		break;
	case 110:
		cout<<"SIIIII";
		break;
	default:
		break;
	}

}


int main(int argc, char** argv)
{


	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (1280, 700);
	glutInitWindowPosition (75, 0);
	glutCreateWindow (argv[0]);
	init ();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialUpFunc(keyBoardUp);
	glutSpecialFunc(specialKeyboard);

	glutMainLoop();
	return 0;
}

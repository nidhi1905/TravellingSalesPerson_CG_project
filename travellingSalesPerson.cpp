#include <stdlib.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include<sstream>
#include<string>
using namespace std;
typedef struct{
	int x, y;
}point;

point* arrayCities = NULL;
int nCities;								
int* position = NULL;				
double** distances;					
double initialDistance;			



void display()
{
	glClearColor( 0.4, 0.4 , 0.7 , 0 );
	glClear( GL_COLOR_BUFFER_BIT );
	glLoadIdentity();
	glColor3ub(0, 0, 0);
	
	glBegin(GL_LINE_LOOP);
	for (int k = 0; k < nCities; k++)
		glVertex2i(arrayCities[position[k]].x, arrayCities[position[k]].y);
	glEnd();

	glPointSize(15);
	glColor3ub(200, 100, 100);
	ostringstream str1;
	for (int k = 0; k < nCities; k++)
	{
		str1<<k;
		glBegin(GL_POINTS);
			if(k==0 || k==nCities-1)
				glColor3ub(200,200,200);
			//glRasterPos2f(arrayCities[k].x, arrayCities[k].y);
			glVertex2i(arrayCities[k].x, arrayCities[k].y);
			glColor3ub(150,150,150);
			//glutBitmapCharacter(GLUT_BITMAP_8_BY_13,k);
		glEnd();
	}
	glutSwapBuffers();
}

double computeDistance()
{
	double distancePath = 0.0;
	for (int k = 0; k < nCities - 1; k++)
		distancePath += distances[position[k]][position[k + 1]];
	distancePath += distances[position[nCities - 1]][position[0]];
	return distancePath;
}

void Swap2Elements(int* e1, int *e2)
{
	do
	{
		*e1 = rand() % nCities;
		*e2 = rand() % nCities;
	} while (*e1 == *e2);
	int temp = position[*e1];
	position[*e1] = position[*e2];
	position[*e2] = temp;
}

void getShortestPath()
{
	int k = 0;
	int limit = 10000;
	while (k < 25000 && limit > 0)
	{
		int e1, e2;
		double d1 = computeDistance();
		Swap2Elements(&e1, &e2);
		double d2 = computeDistance();
		if (d2 > d1)
		{
			int temp = position[e1];
			position[e1] = position[e2];
			position[e2] = temp;
			//std::cout << "No swap" << std::endl;
			limit--;
		}
		else
			printf("Distance iteration-%d : %lf\n", k,computeDistance());
		k++;
	}
}


void onKeyDown(unsigned char key, int x, int y )
{
	if (key == 27)
		exit(0);
	 else if (key == 'a')
	 {
		 getShortestPath();
		 printf("Initial distance: %lf; Shortest path: %lf\n", initialDistance, computeDistance());
	 }
	 else if (key == 's')
	 {
			int e1, e2;
			double d1 = computeDistance();
			Swap2Elements(&e1, &e2);
			double d2 = computeDistance();
			if (d2 > d1)
			{
				//revert the swaps
				int temp = position[e1];
				position[e1] = position[e2];
				position[e2] = temp;
			}
			printf("Distance: %lf \n", computeDistance());

	 }
   glutPostRedisplay() ;
}


void onResize( int w, int h )
{
	glViewport( 0,0,w,h) ;
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( -w/2, w/2, -h/2, h/2, -1, 1);
	glMatrixMode( GL_MODELVIEW);
	glLoadIdentity();
}

double distanceFinder(point a, point b)
{
	return sqrt(pow(a.x - b.x, 2.0) + pow(a.y - b.y, 2.0));
}

void Init(int size) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	
	nCities = size;			 
	srand(time(NULL));	

	arrayCities = (point*)malloc(size * sizeof(point));
	position = (int*)malloc(size * sizeof(int));
	distances = (double**)malloc(size * sizeof(double*));

	for (int k = 0; k < size; k++)
	{
		distances[k] = (double*)malloc(size * sizeof(double));
		int ri = rand() % (700 + 1);
		ri -= 700 / 2;
		arrayCities[k].x = ri;
		ri = rand() % (600 + 1);
		ri -= 600 / 2;
		arrayCities[k].y = ri;
		position[k] = k;
	}

	for (int y = 0; y < size; y++)
		for (int x = 0; x < size; x++)
			distances[x][y] = distanceFinder(arrayCities[x], arrayCities[y]);	

	initialDistance = computeDistance();
	printf("Initial distance: %lf\n", initialDistance);
}

int main( int argc, char *argv[] )
{
	int n;
	printf("Please insert the number of cities : ");
	scanf("%d", &n);
	printf("\n");
	glutInit(&argc, argv );
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
	glutInitWindowSize(700, 600);
	glutCreateWindow( "Travelling Sales Person" ) ;

	glutDisplayFunc(display ) ;
	glutIdleFunc(display);
	glutReshapeFunc( onResize );
	glutKeyboardFunc( onKeyDown ) ;
	Init(n);
	
	glutMainLoop();
	return 0;
}


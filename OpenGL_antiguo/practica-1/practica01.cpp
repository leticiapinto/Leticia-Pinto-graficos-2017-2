// Para compilar en MacOSX
// g++ -o practica01 practica01.cpp -framework OpenGL -framework GLUT

#include <stdlib.h>
#include <stdio.h>
#ifdef __APPLE__
#include "GLUT/glut.h"
#else
#include "GL/glut.h"
#endif
#include <iostream>
#define NUMVTX (10)

static int winwidth,winheight;
static int mx,my;
static int flag=0;
static float rotx=0.0f, roty=0.0f;


//Leticia Leonor

GLfloat l[24][3]={  {0,1,1},{0,1,-1},
                      {0,-1,1},{0,-1,-1},
                      {1.5,-1,1},{1.5,-1,-1}

                      };
/*
GLfloat l[30][3]={  {0,1,1},{0.5,1,1},
                    {0,-1,1},{0.5,-1,1},
                    {0.5,-1,1},{1.5,-1,1},
                    {0.5,-0.5,1},{1.5,-0.5,1},

                      {1.5,-1,1},{1.5,-1,-1},
                      {1.5,-0.5,1},{1.5,-0.5,-1},
                      {0.5,-0.5,1},{0.5,-0.5,-1},
                      {0.5,1,1},{0.5,1,-1},

                      {0,1,1},{0,1,-1},
                      {0,-1,1},{0,-1,-1},
                      {1.5,-1,1},{1.5,-1,-1},
                      {1.5,-0.5,-1},{1.5,-1.0,-1},

                      {0.5,-0.5,-1},{0.5,-1.0,-1},
                      {0.5,-1.0,-1},{0.0,-1.0,-1},
                      {0.5,1.0,-1},{0.0,1.0,-1}

                };
 */
//Pinto
// P en 2D
GLfloat p[10][3]={  {0,-1,1},{0,-1,-1},
                      {0,1,1},{0,1,-1},
                      {1,1,1},{1,1,-1},
                      {1,0,1},{1,0,-1},
                      {0,0,1},{0,0,-1}
                      };
/*
GLfloat p[36][3]={
                {-0.5,1.0,1.0},{-0.5,1.0,-1.0},
                {0.5,1.0,1.0},{0.5,1.0,-1.0},
                {0.5,0.0,1.0},{0.5,0.0,-1.0},
                {-0.5,0.0,1.0},{-0.5,0.0,-1.0},
                {-0.5,1.0,1.0},{-0.5,1.0,-1.0},


                {-0.25,0.75,1.0},{-0.25,0.75,-1.0},
                {0.25,0.75,1.0},{0.25,0.75,-1.0},
                {0.25,0.25,1.0},{0.25,0.25,-1.0},
                {-0.25,0.25,1.0},{-0.25,0.25,-1.0},
                {-0.25,0.75,1.0},{-0.25,0.75,-1.0},


                //{-0.5,0.0,-1.0},{-0.5,1.0,-1.0},
                //{0.5,0.0,-1.0},{0.5,1.0,-1.0},
                //{0.5,0.0,-1.0},{-0.5,0.0,-1.0},
                 //{-0.5,1.0,1.0},{0.5,1.0,1.0},
                  //{-0.5,0.0,1.0},{0.5,0.0,1.0},


                  {-0.5,0.0,1.0},{0.0,0.0,1.0},
                  {-0.5,-1.0,1.0},{0.0,-1.0,1.0},
                  {0.0,0.0,1.0},{0.0,-1.0,1.0},
                  {0.0,0.0,-1.0},{0.0,-1.0,-1.0},
                  {-0.5,0.0,-1.0},{-0.5,-1.0,-1.0},
                  {-0.5,0.0,1.0},{-0.5,-1.0,1.0},
                  {-0.5,-1.0,-1.0},{-0.5,-1.0,1.0},
                  {0.0,-1.0,-1.0},{0.0,-1.0,1.0},
                  //{0.0,-1.0,1.0},{0.0,-1.0,-1.0},
};

*/
//Alva
GLfloat a[10][3]={  {-0.5,-1,1},{-0.5,-1,-1},
                      {0,1,1},{0,1,-1},
                      {0.5,-1,1},{0.5,-1,-1},
                      {0.25,0,1},{0.25,0,-1},
                      {-0.25,0,1},{-0.25,0,-1}
                      };

GLfloat c[40][3]={    {1,0,0},{1,0,0},
                      {0,1,0},{0,1,0},
                      {0,0,1},{0,0,1},
                      {1,0,0},{1,0,0},

                      {1,0,0},{1,0,0},
                      {0,1,0},{0,1,0},
                      {0,0,1},{0,0,1},
                      {1,0,0},{1,0,0},

                      {1,0,0},{1,0,0},
                      {0,1,0},{0,1,0},
                      {0,0,1},{0,0,1},
                      {1,0,0},{1,0,0},

                      {1,0,0},{1,0,0},
                      {0,1,0},{0,1,0},
                      {0,0,1},{0,0,1},
                      {1,0,0},{1,0,0},

                      {1,0,0},{1,0,0},
                      {0,1,0},{0,1,0},
                      {0,0,1},{0,0,1},
                      {1,0,0},{1,0,0}
                      };

void drawVertex(float posIni,int sizeV, GLfloat vertex[][3], GLfloat colors[][3])
{
    glBegin(GL_QUAD_STRIP);
    int i;
    for (i=0; i<sizeV; i++)
	{
	    GLfloat temp[3];
        for(int j=0;j<3;j++)
        {
            temp[j]=vertex[i][j];
        }
        temp[0]+=posIni;
		glColor3fv(colors[i]);
		glVertex3fv(temp);
	}
	glEnd();
}

void drawgraphix()
{


	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glPushMatrix();

	glTranslatef(0.0f,0.0f,-3.0f);

	glRotatef(rotx,0.0f,1.0f,0.0f);
	glRotatef(roty,1.0f,0.0f,0.0f);

    //drawVertex(-2.0f,30,l,c); //l en 3D
    //drawVertex(0.0f,32,l,c);   //l en 3D
    drawVertex(-2.5f,6,l,c); //l en 2D
    drawVertex(-0.5f,6,l,c); //l en 2D
	drawVertex(1.5f,10,p,c); //P en 2D
	//drawVertex(2.5f,20,p,c); //P en 3D

	drawVertex(3.0f,10,a,c);




	glPopMatrix();

	glutSwapBuffers();
}

void reshapefunc(int width,int height)
{
	winwidth=width;
	winheight=height;

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glViewport(0,0,width,height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(80.0,(float)width/height,1.0,30.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0f,0.0f,3.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);
}

void mousefunc(int button,int state,int x,int y)
{
	if (button==GLUT_LEFT_BUTTON) {
		if (state==GLUT_DOWN) {
			flag=1;
		} else {
			flag=0;
		}
	}
}

void motionfunc(int x,int y)
{
	if (flag>0) {
		if (flag>1) {
			rotx+=360.0f*(x-mx)/winwidth;
			roty+=360.0f*(y-my)/winheight;
		}

		mx=x;
		my=y;

		drawgraphix();

		flag=2;
	}
}

void keyboardfunc(unsigned char key,int x,int y)
{
	if (key=='q' || key==27) exit(0);
}


void idlefunc()
{
	glutPostRedisplay();
}

void light()
{
    glEnable(GL_NORMALIZE);  //normalizacion automatica --- revisar!!
    //GLfloat global_ambient[] = {0.5f,0.5f,0.5f,1.0f};
    //glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
    //
    glEnable(GL_LIGHTING);

    const GLfloat light_ambient[] = { 0.0f,0.0f,0.0f,1.0f};
    const GLfloat light_diffuse[] = { 1.0f,1.0f,1.0f,1.0f}; //rojo 1 0 0 1 :  r g b alpha
    const GLfloat light_specular[] = { 1.0f,1.0f,1.0f,1.0f};
    const GLfloat light_position[] = { 5.0f,0.0f,5.0f,0.0f};

    //
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    //glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    const GLfloat mat_ambient[] = { 0.0f,0.0f,0.0f,1.0f};
    const GLfloat mat_diffuse[] = { 0.0f,1.0f,0.0f,1.0f};
    const GLfloat mat_specular[] = { 1.0f,1.0f,1.0f,1.0f};
    const GLfloat mat_shininess[] = { 10.0f};

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    //glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}
int main(int argc,char **argv)
{
   winwidth=winheight=512;

   glutInit(&argc,argv);
   glutInitWindowSize(winwidth,winheight);
   glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
   glutCreateWindow("Practica 1 - Ejercicio 1.1");

   glutDisplayFunc(drawgraphix);
   light();
   glutReshapeFunc(reshapefunc);
   glutMouseFunc(mousefunc);
   glutMotionFunc(motionfunc);
   glutKeyboardFunc(keyboardfunc);
   glutIdleFunc(idlefunc);

   glutMainLoop();

   return(0);
}

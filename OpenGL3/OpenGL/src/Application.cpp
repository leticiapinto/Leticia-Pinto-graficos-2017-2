#include <iostream>
#include <GL/glew.h>
//#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <sstream>

#ifdef __APPLE__
#include "GLUT/glut.h"
#else
#include "glut.h"
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "VertexPositions.h"

using namespace std;

//GLUT
int window;
static int mx, my;
static int flag = 0;
static float rotx = 0.0f, roty = 0.0f;

//Shader mShader;

Shader lampShader;


VertexPositions *mName, *mLamp;

int swidth = 800;
int sheight = 600;

glm::mat4 view(1.0);
glm::mat4 projection(1.0);


// lighting
glm::vec3* lightposition;
glm::vec3* viewposition;

void loadModelGraphicLetters()
{
	float distance = 2.5f;
	float xMove= -3.0f;
	//pintando L

	mName->AddElements(
	{	-1.0f + xMove, -1.0f, -1.0f,
		-1.0f + xMove, -1.0f, 1.0f,
		-1.0f + xMove,  1.0f, 1.0f,
		-1.0f + xMove,  1.0f, -1.0f
	});

	
	mName->AddElements({
		-1.0f + xMove, -1.0f,  -1.0f,
		-1.0f + xMove, -1.0f, 1.0f,
		1.0f + xMove, -1.0f, 1.0f,
		1.0f + xMove, -1.0f, -1.0f
	});
	
	xMove += distance;
	
	//L

	mName->AddElements(
	{ -1.0f + xMove, -1.0f, -1.0f,
		-1.0f + xMove, -1.0f, 1.0f,
		-1.0f + xMove,  1.0f, 1.0f,
		-1.0f + xMove,  1.0f, -1.0f
	});


	mName->AddElements({
		-1.0f + xMove, -1.0f,  -1.0f,
		-1.0f + xMove, -1.0f, 1.0f,
		1.0f + xMove, -1.0f, 1.0f,
		1.0f + xMove, -1.0f, -1.0f
	});

	xMove += distance;

	//P

	mName->AddElements({
		-1.0f + xMove, -1.0f, 1.0f,
		-1.0f + xMove, -1.0f, -1.0f,
		-1.0f + xMove, 1.0f, -1.0f,
		-1.0f + xMove, 1.0f, 1.0f,
	});

	mName->AddElements({
		-1.0f + xMove, 1.0f, 1.0f,
		-1.0f + xMove, 1.0f, -1.0f,
		1.0f + xMove, 1.0f, -1.0f,
		1.0f + xMove, 1.0f, 1.0f,
	});

	mName->AddElements({
		1.0f + xMove, 0.0f, 1.0f,
		1.0f + xMove, 0.0f, -1.0f,
		1.0f + xMove, 1.0f, -1.0f,
		1.0f + xMove, 1.0f, 1.0f,
	});

	mName->AddElements({
		1.0f + xMove, 0.0f, 1.0f,
		1.0f + xMove, 0.0f, -1.0f,
		-1.0f + xMove, 0.0f, -1.0f,
		-1.0f + xMove, 0.0f, 1.0f,
	});

	xMove += distance;

	//A

	mName->AddElements({
		-1.0f + xMove, -1.0f, 1.0f,
		-1.0f + xMove, -1.0f, -1.0f,
		-1.0f + xMove, 1.0f, -1.0f,
		-1.0f + xMove, 1.0f, 1.0f,
	});

	mName->AddElements({
		-1.0f + xMove, 1.0f, 1.0f,
		-1.0f + xMove, 1.0f, -1.0f,
		1.0f + xMove, 1.0f, -1.0f,
		1.0f + xMove, 1.0f, 1.0f,
	});

	mName->AddElements({
		1.0f + xMove, 0.0f, 1.0f,
		1.0f + xMove, 0.0f, -1.0f,
		1.0f + xMove, 1.0f, -1.0f,
		1.0f + xMove, 1.0f, 1.0f,
	});

	mName->AddElements({
		1.0f + xMove, 0.0f, 1.0f,
		1.0f + xMove, 0.0f, -1.0f,
		-1.0f + xMove, 0.0f, -1.0f,
		-1.0f + xMove, 0.0f, 1.0f,
	});

	mName->AddElements({
		1.0f + xMove, -1.0f, 1.0f,
		1.0f + xMove, -1.0f, -1.0f,
		1.0f + xMove, 1.0f, -1.0f,
		1.0f + xMove, 1.0f, 1.0f,
	});

}

void cuboLampara()
{
	mLamp->AddElements({
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
	});

	mLamp->AddElements({
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f, 1.0f,  1.0f,
	});


	mLamp->AddElements({
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
	});

	mLamp->AddElements({
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
	});

	mLamp->AddElements({
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, 1.0f,
	});

	mLamp->AddElements({
		-1.0f, 1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f, 1.0f, 1.0f,
	});

}

void cubo()
{
	mName->AddCube(0.0f, 4.0f);
	mName->AddCube(0.0f, 3.0f);
	mName->AddCube(0.0f, 2.0f);
	mName->AddCube(0.0f, 1.0f);
	mName->AddCube(1.0f, 1.0f);
	mName->AddCube(2.0f, 1.0f);

	mName->AddCube(4.0f, 4.0f);
	mName->AddCube(4.0f, 3.0f);
	mName->AddCube(4.0f, 2.0f);
	mName->AddCube(4.0f, 1.0f);
	mName->AddCube(5.0f, 1.0f);
	mName->AddCube(6.0f, 1.0f);

	mName->AddCube(8.0f, 4.0f);
	mName->AddCube(9.0f, 4.0f);
	mName->AddCube(10.0f, 4.0f);
	mName->AddCube(8.0f, 3.0f);
	mName->AddCube(10.0f, 3.0f);
	mName->AddCube(8.0f, 2.0f);
	mName->AddCube(9.0f, 2.0f);
	mName->AddCube(10.0f, 2.0f);
	mName->AddCube(8.0f, 1.0f);

	mName->AddCube(12.0f, 4.0f);
	mName->AddCube(13.0f, 4.0f);
	mName->AddCube(14.0f, 4.0f);
	mName->AddCube(12.0f, 3.0f);
	mName->AddCube(14.0f, 3.0f);
	mName->AddCube(12.0f, 2.0f);
	mName->AddCube(13.0f, 2.0f);
	mName->AddCube(14.0f, 2.0f);
	mName->AddCube(12.0f, 1.0f);
	mName->AddCube(14.0f, 1.0f);

	//cubo de luz blanca
	/*
	float desplazamientox = 3.0f;
	float desplazamientoY = 3.0f;
	float desplazamientoZ = 3.0f;


	mVertexPosition.AddElements({
		-0.5f + desplazamientox, -0.5f + desplazamientoY, -0.5f + desplazamientoZ,
		 0.5f + desplazamientox, -0.5f + desplazamientoY, -0.5f + desplazamientoZ,
		 0.5f + desplazamientox,  0.5f + desplazamientoY, -0.5f + desplazamientoZ,
		-0.5f + desplazamientox,  0.5f + desplazamientoY, -0.5f + desplazamientoZ,
	}, {1.0f, 1.0f, 1.0f});

	mVertexPosition.AddElements({
		-0.5f + desplazamientox, -0.5f + desplazamientoY,  0.5f + desplazamientoZ,
		0.5f + desplazamientox, -0.5f + desplazamientoY,  0.5f + desplazamientoZ,
		0.5f + desplazamientox,  0.5f + desplazamientoY,  0.5f + desplazamientoZ,
		-0.5f + desplazamientox, 0.5f + desplazamientoY,  0.5f + desplazamientoZ,
	}, { 1.0f, 1.0f, 1.0f });


	mVertexPosition.AddElements({
		-0.5f + desplazamientox,  0.5f + desplazamientoY,  0.5f + desplazamientoZ,
		-0.5f + desplazamientox,  0.5f + desplazamientoY, -0.5f + desplazamientoZ,
		-0.5f + desplazamientox, -0.5f + desplazamientoY, -0.5f + desplazamientoZ,
		-0.5f + desplazamientox, -0.5f + desplazamientoY,  0.5f + desplazamientoZ,
	}, { 1.0f, 1.0f, 1.0f });

	mVertexPosition.AddElements({
		0.5f + desplazamientox,  0.5f + desplazamientoY,  0.5f + desplazamientoZ,
		0.5f + desplazamientox,  0.5f + desplazamientoY, -0.5f + desplazamientoZ,
		0.5f + desplazamientox, -0.5f + desplazamientoY, -0.5f + desplazamientoZ,
		0.5f + desplazamientox, -0.5f + desplazamientoY,  0.5f + desplazamientoZ,
	}, { 1.0f, 1.0f, 1.0f });

	mVertexPosition.AddElements({
		-0.5f + desplazamientox, -0.5f + desplazamientoY, -0.5f + desplazamientoZ,
		0.5f + desplazamientox, -0.5f + desplazamientoY, -0.5f + desplazamientoZ,
		0.5f + desplazamientox, -0.5f + desplazamientoY,  0.5f + desplazamientoZ,
		-0.5f + desplazamientox, -0.5f + desplazamientoY, 0.5f + desplazamientoZ,
	}, { 1.0f, 1.0f, 1.0f });

	mVertexPosition.AddElements({
		-0.5f + desplazamientox, 0.5f + desplazamientoY, -0.5f + desplazamientoZ,
		0.5f + desplazamientox,  0.5f + desplazamientoY, -0.5f + desplazamientoZ,
		0.5f + desplazamientox,  0.5f + desplazamientoY,  0.5f + desplazamientoZ,
		-0.5f + desplazamientox, 0.5f + desplazamientoY, 0.5f + desplazamientoZ,
	}, { 1.0f, 1.0f, 1.0f });
	*/
}

void LoadShader()
{
	

	viewposition= new glm::vec3(0.0f, 0.0f, -10.0f);
	view = glm::translate(view, *viewposition);
	projection = glm::perspective(glm::radians(60.0f), (float)swidth / (float)sheight, 0.1f, 100.0f);

	

	glm::mat4* modelLamp = new glm::mat4(1.0f);
	*modelLamp=glm::scale(*modelLamp, glm::vec3(0.2f, 0.2f, 0.2f));

	lightposition = new glm::vec3(20.0f, 20.0f, 0.0f);

	*modelLamp = glm::translate(*modelLamp, *lightposition);

	mLamp = new VertexPositions("res/shaders/Basic.shader",modelLamp,&view,&projection);
	cuboLampara();
	mLamp->ConstructBuffers();


	glm::vec3* lighColor = new glm::vec3(1.0f, 1.0f, 1.0f);
	mName = new VertexPositions("res/shaders/Lighting.shader", new glm::mat4(1.0f), &view, &projection, lighColor, lightposition, viewposition);
	cubo();
	mName->ConstructBuffers();
	
}

void LoadModelViewProjection()
{
	//matrices 
	delete mName->Model;
	mName->Model = new glm::mat4(1.0f);
	*mName->Model = glm::rotate(*mName->Model, glm::radians(rotx), glm::vec3(0.0f, 1.0f, 0.0f));
	*mName->Model = glm::rotate(*mName->Model, glm::radians(roty), glm::vec3(1.0f, 0.0f, 0.0f));
}

void ini()
{
	//debemos ponerlo despues del context

	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	LoadShader();
}
void loop()
{
	LoadModelViewProjection();
	mName->draw();
	mLamp->draw();
}

void displayFunc()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	loop();
	glutSwapBuffers();
}

void reshapefunc(int width, int height)
{

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glViewport(0, 0, width, height);

}

void mousefunc(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			flag = 1;
		}
		else {
			flag = 0;
		}
	}
}

void motionfunc(int x, int y)
{
	if (flag>0) {
		if (flag>1) {
			rotx += 360.0f*(x - mx) / swidth;
			roty += 360.0f*(y - my) / sheight;
		}

		mx = x;
		my = y;

		displayFunc();
		flag = 2;
	}
}

void keyboardfunc(unsigned char key, int x, int y)
{
	if (key == 'q' || key == 27) exit(0);
}


void idlefunc()
{
	glutPostRedisplay();
}

int main(int argc, char **argv)
{

	glutInit(&argc, argv);
	glutInitWindowSize(swidth, sheight);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	window = glutCreateWindow("Practica 1 - Ejercicio 1.2");
	ini();

	glutDisplayFunc(displayFunc);
	glutReshapeFunc(reshapefunc);
	glutMouseFunc(mousefunc);
	glutMotionFunc(motionfunc);
	glutKeyboardFunc(keyboardfunc);
	glutIdleFunc(idlefunc);

	glutMainLoop();


	/*
	GLFWwindow* window;
	// Initialize the library 
	if (!glfwInit())
		return -1;
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL); // Create a windowed mode window and its OpenGL context 
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window); // Make the window's context current 
	//debemos ponerlo despues del context
	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;
	
	std::cout << glGetString(GL_VERSION) << std::endl;*/
	
	/* Loop until the user closes the window */
	/*
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT); // Render here 
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr ); //glDrawArrays(GL_TRIANGLES, 0, 6);
		glfwSwapBuffers(window); // Swap front and back buffers 
		glfwPollEvents(); // Poll for and process events 
	}
	mShader.DeleteShader(); //glDeleteProgram(shader);

	glfwTerminate();

	*/
	return 0;
}
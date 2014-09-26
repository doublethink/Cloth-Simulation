#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>

// My definition
#include "define.h"
#include "Spotlight.h"
#include "Cloth.h"
#include "Shader.h"
#include "textfile.h"
#include <iostream>

using namespace std;

// Global Variables
GLuint g_mainWnd;
GLuint g_nWinWidth  = G308_WIN_WIDTH;
GLuint g_nWinHeight = G308_WIN_HEIGHT;
Camera camera;
Cloth* cloth[3];
float thetax, thetay, zoom;
int timerState, numCloth, windState;
Spotlight* spotlight;
Shader* clothshader;
Shader* shader;
G308_Point wind;
bool fullscreen;

void G308_Display() ;
void G308_Reshape(int w, int h) ;
void G308_SetCamera();
void G308_SetLight();
void G308_SetShader();
void G308_Mouse(int button, int state, int x, int y);
void G308_MouseMove(int x, int y);
void G308_SpecialKey(int, int, int);
void G308_Keyboard(unsigned char, int, int);
void G308_Timer(int);
void G308_initMisc();

int main(int argc, char** argv)
{

	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(g_nWinWidth, g_nWinHeight);
    g_mainWnd = glutCreateWindow("COMP308 Cloth Simulation");

    glutDisplayFunc(G308_Display);
    glutReshapeFunc(G308_Reshape);
    glutMouseFunc(G308_Mouse);
    glutMotionFunc(G308_MouseMove);
    glutKeyboardFunc(G308_Keyboard);
    glutSpecialFunc(G308_SpecialKey);

    glewInit();
	if (glewIsSupported("GL_VERSION_2_0"))
		printf("OpenGL 2.0 supported.\n");
	else {
		printf("OpenGL 2.0 not supported !\n");
		exit(1);
	}

	//G308_SetCamera();
	G308_SetLight();
	G308_SetShader();
	G308_initMisc();

	glutTimerFunc(TIMER_SPEED, G308_Timer, 0);

	glutMainLoop();

    return 0;
}

// Display function
void G308_Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	glTranslatef(0.0,0.0,zoom);
	glRotatef(thetax,1.0,0.0,0.0);
	glRotatef(thetay,0.0,1.0,0.0);
	// Render cloth
	glUseProgram(shader->p);
	spotlight->setLight();
	spotlight->RenderGeometry();
	cloth[0]->DrawGrid();

	glUseProgram(clothshader->p);
	for (int i =0; i <numCloth; i++){
		cloth[i]->RenderGeometry();
	}

	glPopMatrix();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);

	glutSwapBuffers();
}

// Reshape function
void G308_Reshape(int w, int h)
{
    if (h == 0) h = 1;

	g_nWinWidth = w;
	g_nWinHeight = h;

    glViewport(0, 0, g_nWinWidth, g_nWinHeight);
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat)w / (GLfloat)h, 1.f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
}

// Set Light
void G308_SetLight()
{
	glMatrixMode(GL_MODELVIEW);
	GLfloat lmodel_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

	// Direction lighting
	float position[]	  = {0.0f, 0.75f, 0.25f, 0.0f};
	float diffintensity[] = {0.8f, 0.8f, 0.8f, 1.0f};
	float ambient[]       = {0.05f, 0.05f, 0.05f, 1.0f};
	float specular[]      = {1.0f, 1.0f, 1.0f, 1.0f};

	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffintensity);
	glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	// Spotlight
	spotlight = new Spotlight;

	// Point light - defined as a spotlight but cutoff not used in shader
	float position1[] = {0.0f,.5f,3.0f,1.0f};
	float direction1[] = {0.0, 1.0, 0.0};
	float constantatt[] = {0.3};
	float linearatt[] = {0.25};
	glLightfv(GL_LIGHT2, GL_POSITION,position1);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, direction1);
	glLightfv(GL_LIGHT2, GL_DIFFUSE,  diffintensity);
	glLightfv(GL_LIGHT2, GL_AMBIENT,  ambient);
	glLightfv(GL_LIGHT2, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT2, GL_CONSTANT_ATTENUATION, constantatt);
	glLightfv(GL_LIGHT2, GL_LINEAR_ATTENUATION, linearatt);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT2);
}

void G308_Mouse(int button, int state, int x, int y){
}

void G308_MouseMove(int x, int y){

}

void G308_SpecialKey(int key, int x, int y){

	switch(key){
	case GLUT_KEY_UP:
		thetax += 5.0f;				// Move eye position
		break;
	case GLUT_KEY_DOWN:
		thetax -= 5.0f;
		break;
	case GLUT_KEY_RIGHT:
		thetay += 5.0f;
		break;
	case GLUT_KEY_LEFT:
		thetay -= 5.0f;
		break;
	case GLUT_KEY_HOME:
		zoom -= 5.0f;
		break;
	case GLUT_KEY_END:
		zoom += 2.0f;
		break;
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camera.centerx, camera.centery, camera.centerz, camera.eyex, camera.eyey, camera.eyez, camera.upx, camera.upy, camera.upz);
	float position1[] = {0.0f,.5f,3.0f,1.0f};
	glLightfv(GL_LIGHT2, GL_POSITION,position1);
	spotlight->setLight();
	glutPostRedisplay();

}

void G308_Keyboard(unsigned char key, int x, int y){

	switch(key){
	case 't':	// start/stop timer
		if(timerState == TIMER_OFF){
			timerState = TIMER_ON;
			glutTimerFunc(TIMER_SPEED, G308_Timer, 0);
		}
		else
			timerState = TIMER_OFF;
		glutPostRedisplay();
		break;
	case 'e':	// change selected part of spotlight
		spotlight->changeSelection();
		break;
	case 'w':	// move currently selected spotlight part
	case 'a':
	case 's':
	case 'd':
	case 'z':
	case 'x':
		spotlight->move(key);
		break;
	case 27:
		exit(0);
		break;
	case 'n':	// add cloth to world
		if (numCloth==1){
			cloth[1] = new Cloth(6.0f, 8.0f, 0.0f, 15.0f, 10.0f, 0.004f, -0.002f);
			cloth[1]->ReadTexture("assets/carpet.jpg");
			numCloth++;
		} else if (numCloth==2){
			cloth[2] = new Cloth(-14.0f, 9.0f, 0.0f, 8.0f, 12.0f, 0.001f, -0.0005f);
			cloth[2]->ReadTexture("assets/superman_cape.jpg");
			numCloth++;
		}
		break;
	case 'm':
		if (windState == WIND_ON)
			windState = WIND_OFF;
		else
			windState = WIND_ON;
		break;
	case ',':
		wind.x *= 1.1;
		wind.z *= 1.1;
		break;
	case '.':
		wind.x *= 0.9;
		wind.z *= 0.9;
		break;
	case 'f':
		if (!fullscreen){
			glutFullScreen();
			fullscreen = true;
		}else {
			glutReshapeWindow(G308_WIN_WIDTH, G308_WIN_HEIGHT);
        	glutPositionWindow(0,0);
        	fullscreen = false;
		}
	}

}

// Set Camera Position
void G308_SetCamera()
{
	camera.centery = 17.0f;	// Initialise starting camera position
	camera.centerz = 42.0f;
	camera.upy = 1.0f;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(G308_FOVY, (double) g_nWinWidth / (double) g_nWinHeight, G308_ZNEAR_3D, G308_ZFAR_3D);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(camera.centerx, camera.centery, camera.centerz, camera.eyex, camera.eyey, camera.eyez, camera.upx, camera.upy, camera.upz);

}

void G308_Timer(int value){

	if (timerState == TIMER_ON){
		for (int i = 0; i < numCloth; i++){
			if (windState == WIND_ON)
				cloth[i]->addWind(wind);
			cloth[i]->update();
		}
		glutPostRedisplay();
		glutTimerFunc(TIMER_SPEED, G308_Timer, 0);
	}
}

void G308_SetShader(){
	if (clothshader != NULL) delete [] clothshader;
	clothshader = new Shader();
	clothshader->makeShader("shaders/texshader.vert", "shaders/texshader.frag");
	if (shader != NULL) delete [] shader;
	shader = new Shader();
	shader->makeShader("shaders/shader.vert", "shaders/shader.frag");
}

void G308_initMisc(){
	if (cloth[0] != NULL) delete [] cloth[0];
		cloth[0] = new Cloth(-5.0f, 8.0f, 0.0f, 10.0f, 10.0f, 0.002f, -0.001f);
	numCloth = 1;
	cloth[0]->ReadTexture("assets/fabric.jpg");
	wind = {-0.0005f,0.0f,-0.0015f};
	thetax = thetay = 0;
	timerState = TIMER_ON;
	windState = WIND_OFF;
	zoom = -21.0;
	fullscreen = false;
}










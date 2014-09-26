/*
 * Spotlight.cpp
 *
 *  Created on: Sep 28, 2013
 *      Author: stephen
 */

#include "Spotlight.h"
#include <stdio.h>
#include <math.h>
#include "define.h"

Spotlight::Spotlight() {
	position[0] = 0.0;
	position[1] = 17.0;
	position[2] = 7.0;
	position[3] = 1.0;

	diffintensity[0] = 1.0;
	diffintensity[1] = 1.0;
	diffintensity[2] = 1.0;
	diffintensity[3] = 1.0;

	direction[0] = 0.0;
	direction[1] = -3.0;
	direction[2] = -1.0;

	exponent[0] = 10.0;

	specular[0] = 1.0;
	specular[1] = 1.0;
	specular[2] = 1.0;
	specular[3] = 1.0;

	cutoff[0] = 27.0;
	coneheight = 3.0;
	calcCutoff();

	constatten[0] = 0.25;

	calcDirection();

	currSelection = POSITION;

}

Spotlight::~Spotlight() {
	// TODO Auto-generated destructor stub
}

void Spotlight::RenderGeometry() {

	GLUquadric* quad = gluNewQuadric(); //Create a new quadric to allow you to draw cylinders
	if (quad == 0) {
		printf("Not enough memory to allocate space to draw\n");
		exit(EXIT_FAILURE);
	}

	glPushMatrix();
		glTranslatef(position[0], position[1], position[2]);

		glColor3f(1,1,1);
		if (currSelection == POSITION)
			glColor3f(1.0, 0.0, 0.0);	// Position sphere
		glutSolidSphere(1.0, 50, 50);

		glColor3f(1,1,1);
		glPushMatrix();
			glRotatef(90.0, 1.0, 0.0, 0.0);
			glTranslatef(0.0, 0.0, -1.5);
			gluCylinder(quad, 0.3, 0.3, 3.0, 50, 50);
		glPopMatrix();

		// Rotate towards direction of spotlight
		glRotatef(angle, axisOfRotationx, axisOfRotationy, axisOfRotationz);

		if(currSelection == CUTOFF)
			glColor4f(1.0, 0.0, 0.0, 0.5);
		gluCylinder(quad, 0.0, conebase, coneheight, 8.0, 8.0);	// Cutoff cone
		glColor3f(1,1,1);

		glBegin(GL_LINES);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(0.0,0.0,5.0);
		glEnd();

		glTranslatef(0.0,0.0,5.0);

		if(currSelection == DIRECTION)
			glColor3f(1.0, 0.0, 0.0);
		glutSolidCone(0.5,1.0,8.0 ,8.0);	// Direction cone

	glPopMatrix();

	gluDeleteQuadric(quad);
}

void Spotlight::setLight(){
	//glDisable(GL_LIGHT1);

	glLightfv(GL_LIGHT1, GL_POSITION, position);
	glLightfv(GL_LIGHT1, GL_DIFFUSE,  diffintensity);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction);
	glLightfv(GL_LIGHT1, GL_SPOT_EXPONENT, exponent);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT1, GL_SPOT_CUTOFF, cutoff);
	glLightfv(GL_LIGHT1, GL_CONSTANT_ATTENUATION, constatten);

	glEnable(GL_LIGHT1);
}

// Change selection mode of light adjustment
void Spotlight::changeSelection(){
	switch(currSelection){
	case POSITION:
		currSelection = DIRECTION;
		break;
	case DIRECTION:
		currSelection = CUTOFF;
		break;
	case CUTOFF:
		currSelection = POSITION;
	}

	glutPostRedisplay();
}

// Adjusts either the position or direction of the light
void Spotlight::move(unsigned char key){

	switch(currSelection){
		case POSITION:
			changePosition(key);
			break;
		case DIRECTION:
			changeDirection(key);
			break;
		case CUTOFF:
			changeCutoff(key);
			break;
	}
	setLight();
	glutPostRedisplay();
}

void Spotlight::changePosition(unsigned char key){
	switch(key){
	case 'z':
		position[1] += 1.0;
		break;
	case 'x':
		position[1] -= 1.0;
		break;
	case 'a':
		position[0] -= 1.0;
		break;
	case 'd':
		position[0] += 1.0;
		break;
	case 'w':
		position[2] -= 1.0;
		break;
	case 's':
		position[2] += 1.0;
		break;
	}
}

void Spotlight::changeDirection(unsigned char key){

	// Is far from a perfect implimentation
	// Should have used arcball but this was an easy hack
	switch(key){
	case 'w':
		direction[2] -= 0.1;
		break;
	case 's':
		direction[2] += 0.1;
		break;
	case 'a':
		direction[0] -= 0.1;
		break;
	case 'd':
		direction[0] += 0.1;
	}
	calcDirection();

}

void Spotlight::changeCutoff(unsigned char key){
	switch(key){
	case 'd':	// Make cutoff larger
		cutoff[0] += 1.0;
		if (cutoff[0] >= 90.0)
			cutoff[0] = 90.0;
		calcCutoff();
		break;
	case 'a':	// Make cutoff smaller
		cutoff[0] -= 1.0;
		if (cutoff[0] <= 0.0)
			cutoff[0] = 0.0;
		calcCutoff();
	}

}

void Spotlight::calcCutoff(){
	conebase = coneheight* tan((double)(cutoff[0]*M_PI/180));
}

void Spotlight::calcDirection(){
	// Normalise vector
	// Normalise vector
	float nlength = (float)sqrt(pow(direction[0],2) + pow(direction[1],2) + pow(direction[2],2));
	direction[0] = direction[0] / nlength;
	direction[1] = direction[1] / nlength;
	direction[2] = direction[2] / nlength;

	angle = (float)(acos(direction[2]))*180 / M_PI;
	axisOfRotationx = 0 - (1 * direction[1]);
	axisOfRotationy = 1 * direction[0];
	axisOfRotationz = 0;
}

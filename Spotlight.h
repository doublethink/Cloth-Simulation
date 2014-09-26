/*
 * Spotlight.h
 *
 *  Created on: Sep 28, 2013
 *      Author: stephen
 */

#ifndef SPOTLIGHT_H_
#define SPOTLIGHT_H_

#include <GL/glut.h>

class Spotlight {
	private:
		void changePosition(unsigned char);
		void changeDirection(unsigned char);
		void changeCutoff(unsigned char);
		void calcCutoff();
		void calcDirection();
		int currSelection;
		float angle, axisOfRotationx, axisOfRotationy, axisOfRotationz;

	public:
		Spotlight(void);
		virtual ~Spotlight(void);
		void RenderGeometry();
		void setLight();
		void changeSelection();
		void move(unsigned char key);

		float position[4];
		float diffintensity[4];
		float direction[3];
		float exponent[1];
		float specular[4];
		float cutoff[1];
		float coneheight;
		float conebase;
		float constatten[1];

		float dirColor[3];
		float posColor[3];
};

#endif /* SPOTLIGHT_H_ */

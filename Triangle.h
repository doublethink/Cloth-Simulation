/*
 * Triangle.h
 *
 *  Created on: Oct 12, 2013
 *      Author: stephen
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "GL/glut.h"
#include "define.h"
#include "Node.h"
#include "Cloth.h"
#include <math.h>

class Triangle {
private:

	float uv[3][2];
public:
	Triangle(Node* node1, Node* node2, Node* node3, int x, int y, int hor, int vert, bool isTop);
	G308_Point calcNormal();
	virtual ~Triangle();
	void draw();

	Node* n1;
	Node* n2;
	Node* n3;
};
#endif /* TRIANGLE_H_ */

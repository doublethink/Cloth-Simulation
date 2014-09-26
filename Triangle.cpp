/*
 * Triangle.cpp
 *
 *  Created on: Oct 12, 2013
 *      Author: stephen
 */

#include "Triangle.h"

Triangle::Triangle(Node* node1, Node* node2, Node* node3, int x, int y, int hor, int vert, bool isBottom){

	// Calculate texture coordinates based on how many nodes there are up and down and whether it is the top triangle
	if(!isBottom){
		uv[0][0] = (float)(x+1)/(float)hor;
		uv[0][1] = 1.0-(float)y/(float)vert;
		uv[1][0] = (float)(x)/(float)hor;
		uv[1][1] = 1.0-(float)y/(float)vert;
		uv[2][0] = (float)(x)/(float)hor;
		uv[2][1] = 1.0-(float)(y+1)/(float)vert;
	} else {
		uv[0][0] = (float)(x+1)/(float)hor;
		uv[0][1] = 1.0-(float)(y+1)/(float)vert;
		uv[1][0] = (float)(x+1)/(float)hor;
		uv[1][1] = 1.0-(float)y/(float)vert;
		uv[2][0] = (float)(x)/(float)hor;
		uv[2][1] = 1.0-(float)(y+1)/(float)vert;
	}

	n1 = node1;
	n2 = node2;
	n3 = node3;

}

Triangle::~Triangle() {
	// TODO Auto-generated destructor stub
}

void Triangle::draw(){
	G308_Point normal = n1->getNormal();
	glTexCoord2fv(uv[0]);
	glNormal3f(normal.x, normal.y, normal.z);
	glVertex3f(n1->currPos.x, n1->currPos.y, n1->currPos.z);

	normal = n2->getNormal();
	glTexCoord2fv(uv[1]);
	glNormal3f(normal.x, normal.y, normal.z);
	glVertex3f(n2->currPos.x, n2->currPos.y, n2->currPos.z);

	normal = n3->getNormal();
	glTexCoord2fv(uv[2]);
	glNormal3f(normal.x, normal.y, normal.z);
	glVertex3f(n3->currPos.x, n3->currPos.y, n3->currPos.z);
}

G308_Point Triangle::calcNormal(){
	G308_Point vectorU, vectorV, normal;

	// Calculate two edges of triangle
	vectorU.x = n2->currPos.x - n1->currPos.x;
	vectorV.x = n3->currPos.x - n1->currPos.x;
	vectorU.y = n2->currPos.y - n1->currPos.y;
	vectorV.y = n3->currPos.y - n1->currPos.y;
	vectorU.z = n2->currPos.z - n1->currPos.z;
	vectorV.z = n3->currPos.z - n1->currPos.z;

	// Calculate cross product
	normal.x = (vectorU.y * vectorV.z) - (vectorU.z * vectorV.y);
	normal.y = (vectorU.z * vectorV.x) - (vectorU.x * vectorV.z);
	normal.z = (vectorU.x * vectorV.y) - (vectorU.y * vectorV.x);

	// Normalise vector
	float nlength = (float)sqrt(pow(normal.x,2) + pow(normal.y,2) + pow(normal.z,2));
	normal.x = normal.x / nlength;
	normal.y = normal.y / nlength;
	normal.z = normal.z / nlength;

	return normal;
}

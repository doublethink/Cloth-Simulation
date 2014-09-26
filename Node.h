/*
 * Node.h
 *
 *  Created on: Oct 12, 2013
 *      Author: stephen
 */

#ifndef NODE_H_
#define NODE_H_

#include "define.h"
#include "Triangle.h"
#include <math.h>
#include <vector>
#include <stdio.h>

class Node {
private:
	G308_Point prevPos;
	bool fixed;
	float mass;
public:
	Node(G308_Point);
	Node();
	virtual ~Node();
	void addTriangle(Triangle* triangle);
	G308_Point getNormal();
	void setUnmoveable();
	void offsetPos(G308_Point);
	void integrate(float);
	void addForce(G308_Point);
	void addDamping();
	void clearF();

	G308_Point currPos;
	G308_Point V;
	G308_Point F;
	std::vector<Triangle*> triangles;
};
#endif /* NODE_H_ */

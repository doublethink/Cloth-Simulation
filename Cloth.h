/*
 * Cloth.h
 *
 *  Created on: Oct 6, 2013
 *      Author: stephen
 */

#ifndef CLOTH_H_
#define CLOTH_H_

#include "GL/glut.h"
#include "define.h"
#include "Node.h"
#include "Connection.h"
#include "Triangle.h"
#include "G308_ImageLoader.h"
#include <vector>

class Cloth {
private:
	void initForces();
	Node* getNode(int, int);
	void addConnection(Node* n1, Node* n2);
	void satisfyConstraint(Connection);
	void addWindToTri(Triangle, G308_Point);

	int numHorizonalNodes, numVerticalNodes;
	float width, height, mass, xpos, ypos, zpos, Ks, Kd;
	std::vector<Node> nodes;
	std::vector<Connection> springs;
	std::vector<Triangle*> triangles;
	float ambmat[4], diffmat[4], specmat[4], shiny;
	G308_RGBA color;
	G308_Point force;
	TextureInfo t;
	GLuint texName;
public:
	Cloth(float, float, float, float, float, float, float);
	virtual ~Cloth();

	void DrawGrid();
	void RenderGeometry();
	void update();
	void addWind(G308_Point);
	void addSpring();
	void ReadTexture(const char* file);
};

#endif /* CLOTH_H_ */

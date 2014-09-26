/*
 * Cloth.cpp
 *
 *  Created on: Oct 6, 2013
 *      Author: stephen
 */

#include "Cloth.h"

Cloth::Cloth(float x, float y, float z, float w, float h, float ks, float kd) {
	// Gravitational force constant
	force = {0.0f,-0.005f,0.0f};

	// Spring strengths
	Ks = ks;
	Kd = kd;

	// World position
	xpos = x;
	ypos = y;
	zpos = z;

	// Dimensions
	width = w;
	height = h;

	// Initialise sizes
	numHorizonalNodes = 20;
	numVerticalNodes = 20;

	initForces();

	// Material properties of cloth
	ambmat[0] = 0.2f;
	ambmat[1] = 0.2f;
	ambmat[2] = 0.2f;
	ambmat[3] = 1.0f;
	diffmat[0] = 0.7f;
	diffmat[1] = 0.7f;
	diffmat[2] = 0.7f;
	diffmat[3] = 1.0f;
	specmat[0] = 0.7f;
	specmat[1] = 0.7f;
	specmat[2] = 0.7f;
	specmat[3] = 1.0f;
	shiny = 0.2f;
	color.r = 1.0f;
	color.g = 1.0f;
	color.b = 1.0f;
	color.a = 1.0f;

}

Cloth::~Cloth() {
	nodes.clear();
	springs.clear();
	triangles.clear();
}

void Cloth::initForces(){

	// Initialise nodes to starting position
	nodes.resize(numHorizonalNodes*numVerticalNodes);
	for (int i = 0; i < numHorizonalNodes; i++){
		for (int j = 0; j < numVerticalNodes; j++){
			G308_Point pos = {width * (i/(float)numHorizonalNodes)
				, 0, height * (j/(float)numVerticalNodes)};
			nodes[j*numHorizonalNodes+i] = Node(pos);
		}
	}

	// Triangles are the objects that are actually draw, they consist of pointers to node positions
	for (int i = 0; i < numHorizonalNodes-1; i++){
		for (int j = 0; j < numVerticalNodes-1; j++){
			Triangle* tr1 = new Triangle(getNode(i+1,j),getNode(i,j),getNode(i, j+1), i, j, numHorizonalNodes, numVerticalNodes, false);
			Triangle* tr2 = new Triangle(getNode(i+1,j+1),getNode(i+1,j),getNode(i, j+1), i, j, numHorizonalNodes, numVerticalNodes, true);

			getNode(i+1, j)->addTriangle(tr1);
			getNode(i,j)->addTriangle(tr1);
			getNode(i,j+1)->addTriangle(tr1);

			getNode(i+1, j+1)->addTriangle(tr2);
			getNode(i+1,j)->addTriangle(tr2);
			getNode(i,j+1)->addTriangle(tr2);

			triangles.push_back(tr1);
			triangles.push_back(tr2);
		}
	}
	// Very bottom triangle is missed in above loop
	Triangle* trextra = new Triangle(getNode(numHorizonalNodes-1,numVerticalNodes-1),getNode(numHorizonalNodes-1,numVerticalNodes-2),getNode(numHorizonalNodes-2, numVerticalNodes-1),
			numHorizonalNodes-1, numVerticalNodes-1, numHorizonalNodes, numVerticalNodes, true);
	getNode(numHorizonalNodes-1,numVerticalNodes-1)->addTriangle(trextra);
	getNode(numHorizonalNodes-1,numVerticalNodes-2)->addTriangle(trextra);
	getNode(numHorizonalNodes-2, numVerticalNodes-1)->addTriangle(trextra);
	triangles.push_back(trextra);

	// Add structural and shear (consider edge cases)
	for (int i = 0; i < numHorizonalNodes;i++){
		for (int j = 0; j < numVerticalNodes; j++){
			if (i < numHorizonalNodes-1)
				addConnection(getNode(i,j), getNode(i+1,j));	// Horizontal spring
			if (j < numVerticalNodes-1)
				addConnection(getNode(i,j),getNode(i, j+1));	// Vertical spring
			if (i < numHorizonalNodes -1 && j < numVerticalNodes-1){
				addConnection(getNode(i,j), getNode(i+1,j+1));	// Shear spring
				addConnection(getNode(i+1,j), getNode(i, j+1));	// Shear spring
			}
		}
	}

	// Add bend springs (diagonally adjacent nodes)
	for (int i = 0; i < numHorizonalNodes;i++){
		for (int j = 0; j < numVerticalNodes; j++){
			if (i < numHorizonalNodes-2)
				addConnection(getNode(i,j), getNode(i+2,j));
			if (j < numVerticalNodes-2)
				addConnection(getNode(i,j),getNode(i, j+2));
			if (i < numHorizonalNodes -2 && j < numVerticalNodes-2){
				addConnection(getNode(i,j), getNode(i+2,j+2));
				addConnection(getNode(i+2,j), getNode(i, j+2));
			}
		}
	}

	// Make top corners fixed
	getNode(0,0)->setUnmoveable();
	getNode(numHorizonalNodes-1,0)->setUnmoveable();
}

// Called inside timer, adds all forces except wind then integrates
void Cloth::update(){

	// addForce applies gravity to all nodes, damping slightly reduces velocity
	std::vector<Node>::iterator node;
	for (node = nodes.begin(); node != nodes.end(); node++){
		(*node).addForce(force);
		(*node).addDamping();
	}

	// Spring forces added
	std::vector<Connection>::iterator spg;
	for (spg = springs.begin(); spg != springs.end(); spg++){
		(*spg).addSpring();
	}

	// Force added to velocity then next position is calculated
	std::vector<Node>::iterator particle;
	for (particle = nodes.begin(); particle != nodes.end(); particle++){
		(*particle).integrate(0.5f);		// Explicit Euler integration
	}

	// Inverse Provot method, is a reverse spring method only used on springs
	// that are further away from each other than the resting distance
	std::vector<Connection>::iterator constraint;
	for (int j = 0; j < springs.size(); j++){
		springs[j].satisfyConstraint();
	}

	// Clear force of each node for next update
	std::vector<Node>::iterator clearF;
	for (clearF = nodes.begin(); clearF != nodes.end(); clearF++){
		(*clearF).clearF();
	}
}

void Cloth::DrawGrid(){

	glPushMatrix();
	glTranslatef(0.0f,-5.0f,0.0f);
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);
	for(int i=-GRID_SIZE;i<=GRID_SIZE;i++)
	{
		glVertex3f((float)i,0,(float)-GRID_SIZE);
		glVertex3f((float)i,0,(float)GRID_SIZE);

		glVertex3f((float)-GRID_SIZE,0,(float)i);
		glVertex3f((float)GRID_SIZE,0,(float)i);
	}
	glEnd();
	glPopMatrix();
}

void Cloth::RenderGeometry(){

	glColor3f(color.r, color.g, color.b);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambmat);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffmat);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specmat);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny * 128.0f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texName);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Easier to translate cloth to center that adjust initialising positions
	glPushMatrix();
	glTranslatef(xpos, ypos, zpos);
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < triangles.size()-1; i++){
			triangles[i]->draw();
		}
		glEnd();

	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

}

// Called from timer if wind is turned on
void Cloth::addWind(G308_Point dir){
	for (int i = 0; i< triangles.size(); i++){
		addWindToTri(*triangles[i], dir);
	}
}

// Strength of wind force is calculated based on the normal of the triangle
void Cloth::addWindToTri(Triangle tri, G308_Point d){
	G308_Point n = tri.calcNormal();
	G308_Point winddir;
	float ndotd = (n.x * d.x) + (n.y*d.y) + (n.z*d.z);
	winddir.x = n.x * ndotd;
	winddir.y = n.y * ndotd;
	winddir.z = n.y * ndotd;
	tri.n1->addForce(winddir);
	tri.n2->addForce(winddir);
	tri.n3->addForce(winddir);

}

// Helper method
void Cloth::addConnection(Node* n1, Node* n2){
	springs.push_back(Connection(n1, n2, Ks, Kd));
}

// Helper method to find node
Node* Cloth::getNode(int x, int y){
	return &nodes[y*numHorizonalNodes+x];
}

void Cloth::ReadTexture(const char* file) {

	char* filename = (char*)file;

	printf("Reading Texture File %s\n", filename);

	loadTextureFromJPEG(filename, &t);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//Only useful for PNG files, since JPEG doesn't support alpha
	if (t.hasAlpha) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t.width, t.height, 0, GL_RGBA,
				GL_UNSIGNED_BYTE, t.textureData);
	} else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t.width, t.height, 0, GL_RGB,
				GL_UNSIGNED_BYTE, t.textureData);
	}

	//Once the texture has been loaded by GL, we don't need this anymore.
	free(t.textureData);

	printf("Finished loading texture\n");
}

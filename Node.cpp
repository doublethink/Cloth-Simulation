/*
 * Node.cpp
 *
 *  Created on: Oct 12, 2013
 *      Author: stephen
 */

#include "Node.h"

Node::Node(G308_Point pos) {
	currPos = pos;
	prevPos = pos;
	F = {0.0f, 0.0f, 0.0f};
	V = {0.0f, 0.0f, 0.0f};
	fixed = false;
	mass = 1.0;

}
Node::Node() {}

Node::~Node() {
}

void Node::addTriangle(Triangle* triangle){
	triangles.push_back(triangle);
}

G308_Point Node::getNormal(){

	G308_Point n = {0.0,0.0,0.0};
	for(unsigned int i =0; i < triangles.size(); i++){
		G308_Point nt = triangles[i]->calcNormal();
		n.x += nt.x;
		n.y += nt.y;
		n.z += nt.z;
	}

	n.x = n.x/ triangles.size();
	n.y = n.y / triangles.size();
	n.z = n.z / triangles.size();

	float nlength = (float)sqrt(pow(n.x,2) + pow(n.y,2) + pow(n.z,2));
	n.x = n.x / nlength;
	n.y = n.y / nlength;
	n.z = n.z / nlength;

	return n;
}

void Node::setUnmoveable(){
	fixed = true;
}

void Node::offsetPos(G308_Point v){
	if(!fixed){
		V.x += v.x;
		V.y += v.y;
		V.z += v.z;
	}
}

void Node::integrate(float timeStep){
	if(!fixed){
		float deltaTimeMass = timeStep / mass;
		G308_Point Vn = V;
		V.x += F.x * deltaTimeMass;
		V.y += F.y * deltaTimeMass;
		V.z += F.z * deltaTimeMass;
		currPos.x += timeStep * Vn.x;
		currPos.y += timeStep * Vn.y;
		currPos.z += timeStep * Vn.z;

	}
}

void Node::addForce(G308_Point forceVec){

	F.x += forceVec.x;
	F.y += forceVec.y;
	F.z += forceVec.z;
	//printf("accel x = %f, y= %f, z= %f\n", accel.x, accel.y, accel.z);
}

void Node::addDamping(){
	F.x += DEFAULT_DAMPING*V.x;
	F.y += DEFAULT_DAMPING*V.y;
	F.z += DEFAULT_DAMPING*V.z;
}

void Node::clearF(){
	F.x = 0.0f;
	F.y = 0.0f;
	F.z = 0.0f;
}

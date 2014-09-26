/*
 * Connection.cpp
 *
 *  Created on: Oct 12, 2013
 *      Author: stephen
 */

#include "Connection.h"

Connection::Connection(Node* node1, Node* node2, float ks, float kd) {

	n1 = node1;
	n2 = node2;
	G308_Point vec;
	vec.x = n2->currPos.x - n1->currPos.x;
	vec.y = n2->currPos.y - n1->currPos.y;
	vec.z = n2->currPos.z - n1->currPos.z;
	restDistance = (float)sqrt(pow(vec.x,2) + pow(vec.y,2) + pow(vec.z,2));

	Ks = ks;
	Kd = kd;

}

Connection::~Connection() {
	// TODO Auto-generated destructor stub
}

void Connection::satisfyConstraint(){
	// Inverse Provot method
	// Is an easy method of attractive force
	// Stops nodes moving too far away from each other
	G308_Point temp;
	temp.x = n2->currPos.x - n1->currPos.x;
	temp.y = n2->currPos.y - n1->currPos.y;
	temp.z = n2->currPos.z - n1->currPos.z;

	// The 
	float currentDistance = (float)sqrt(pow(temp.x,2) + pow(temp.y,2) + pow(temp.z,2));
	//printf("dis = %f, curdis = %f\n", restDistance, currentDistance);
	//if (currentDistance > restDistance){
		G308_Point correctionVector;
		correctionVector.x = (temp.x*(1- restDistance/currentDistance))*0.5;
		correctionVector.y = (temp.y*(1- restDistance/currentDistance))*0.5;
		correctionVector.z = (temp.z*(1- restDistance/currentDistance))*0.5;


		n1->offsetPos(correctionVector);
		G308_Point negvec = {-1.0 * correctionVector.x, -1.0 * correctionVector.y, -1.0 * correctionVector.z};
		n2->offsetPos(negvec);
	//}
}

void Connection::addSpring(){
	// Find vector between points
	G308_Point deltaP= {0.0,0.0,0.0};
	deltaP.x = n2->currPos.x - n1->currPos.x;
	deltaP.y = n2->currPos.y - n1->currPos.y;
	deltaP.z = n2->currPos.z - n1->currPos.z;
	float currentDistance = (float)sqrt(pow(deltaP.x,2) + pow(deltaP.y,2) + pow(deltaP.z,2));

	// Find velocity difference
	G308_Point deltaV= {0.0,0.0,0.0};
	deltaV.x = n2->currPos.x - n1->currPos.x;
	deltaV.y = n2->currPos.y - n1->currPos.y;
	deltaV.z = n2->currPos.z - n1->currPos.z;

	float leftTerm = (Ks*-1) * restDistance;
	float vdotp = (deltaP.x * deltaV.x) + (deltaP.y* deltaV.y) + (deltaP.z* deltaV.z);
	float rightTerm = Kd * (vdotp/currentDistance);

	G308_Point springForce;
	springForce.x = (deltaP.x / currentDistance) * (leftTerm + rightTerm);
	springForce.y = (deltaP.y / currentDistance) * (leftTerm + rightTerm);
	springForce.z = (deltaP.z / currentDistance) * (leftTerm + rightTerm);

	n1->addForce(springForce);
	springForce.x *= -1.0;
	springForce.y *= -1.0;
	springForce.z *= -1.0;
	n2->addForce(springForce);

}

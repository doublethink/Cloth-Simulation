/*
 * Connection.h
 *
 *  Created on: Oct 12, 2013
 *      Author: stephen
 */

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include "Node.h"

class Connection {
private:
	float Ks, Kd;
public:
	Connection(Node* node1, Node* node2, float, float);
	virtual ~Connection();
	void satisfyConstraint();
	void addSpring();

	Node* n1;
	Node* n2;
	float restDistance;
};
#endif /* CONNECTION_H_ */

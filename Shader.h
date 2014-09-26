/*
 * Shader.h
 *
 *  Created on: Oct 3, 2013
 *      Author: stephen
 */

#ifndef SHADER_H_
#define SHADER_H_
#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "textfile.h"
#include "define.h"

class Shader {
public:
	Shader();
	virtual ~Shader();
	void makeShader(char*, char*);
	void setDebug(bool);

	GLuint p;
private:
	void printShaderInfoLog(GLuint obj);

	GLuint v, f;	// Shader variables
	bool debug;
};

#endif /* SHADER_H_ */

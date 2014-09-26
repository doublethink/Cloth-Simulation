/*
 * Shader.cpp
 *
 *  Created on: Oct 3, 2013
 *      Author: stephen
 */

#include "Shader.h"
#include <GL/glew.h>
#include <GL/glut.h>

Shader::Shader() {
		debug = false;
		v = f= p = 0;
}

Shader::~Shader() {
	// TODO Auto-generated destructor stub
}

void Shader::printShaderInfoLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
		free(infoLog);
	}
}

void Shader::makeShader(char* vert, char* frag){
	char *vs = NULL,*fs = NULL;

	printf("Adding shaders\n");
	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

	vs = textFileRead(vert);
	fs = textFileRead(frag);

	const char * ff = fs;
	const char * vv = vs;

	glShaderSource(v, 1, &vv,NULL);
	glShaderSource(f, 1, &ff,NULL);

	free(vs);free(fs);

	glCompileShader(v);
	glCompileShader(f);

	if(debug){
		printf("Vert log\n");
		printShaderInfoLog(v);
		printf("Frag log\n");
		printShaderInfoLog(f);
	}

	p = glCreateProgram();
	glAttachShader(p,f);
	glAttachShader(p,v);

	glLinkProgram(p);
	printf("Shaders added\n");
}

void Shader::setDebug(bool d){
	debug = d;
}


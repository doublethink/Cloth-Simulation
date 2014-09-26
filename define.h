#pragma once

// Default Window
#define G308_WIN_WIDTH	800
#define G308_WIN_HEIGHT	600

// Projection parameters
#define G308_FOVY		20.0
#define G308_ZNEAR_3D	1
#define G308_ZFAR_3D	1000.0
#define G308_ZNEAR_2D	-50.0
#define G308_ZFAR_2D	50.0

// Shading mode : 0 Polygon, 1 Wireframe
#define G308_SHADE_POLYGON 0
#define G308_SHADE_WIREFRAME 1

// Define number of vertex
#define G308_NUM_VERTEX_PER_FACE 3 // Triangle = 3, Quad = 4

// Timer state defines
#define TIMER_SPEED 7
#define TIMER_ON 1
#define TIMER_OFF 0

//Selection constants
#define CUTOFF 2
#define DIRECTION 1
#define POSITION 0

// Cloth define
#define GRID_SIZE 10
#define WIND_ON 1
#define WIND_OFF 0
#define STRUCTURAL_SPRING 0
#define SHEAR_SPRING 1
#define BEND_SPRING 2
#define DEFAULT_DAMPING -0.015f

// Define Basic Structures
struct G308_Point {
	float x;
	float y;
	float z;
};

struct G308_RGBA {
	float r;
	float g;
	float b;
	float a;
};

typedef G308_Point G308_Normal;

struct G308_UVcoord {

	float u;
	float v;
};

/*
 * Normals and textures need to be defined by face, not by vertex.
 * Reminder: these are just indicies into the normal and texture arrays.
 * n1 and t1 are the normals and texture co-ordinates for vertex 1 of this face.
 * Same goes for (v2,n2,t2) etc.
 */
struct G308_Triangle {

	unsigned int v1;
	unsigned int v2;
	unsigned int v3;
	unsigned int n1;
	unsigned int n2;
	unsigned int n3;
	unsigned int t1;
	unsigned int t2;
	unsigned int t3;
};

struct G308_Quad {

	unsigned int v1;
	unsigned int v2;
	unsigned int v3;
	unsigned int v4;
	unsigned int n1;
	unsigned int n2;
	unsigned int n3;
	unsigned int n4;
	unsigned int t1;
	unsigned int t2;
	unsigned int t3;
	unsigned int t4;
};

struct Camera {
	double eyex;
	double eyey;
	double eyez;
	double centerx;
	double centery;
	double centerz;
	double upx;
	double upy;
	double upz;
};

class Node;
class Triangle;
class Connection;
class Cloth;

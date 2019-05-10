#include "stdafx.h"
#include <windows.h>
#include <math.h>
#include <gl.h>
#include <glut.h>

#include <stdio.h>										
#include <stdlib.h>

#include <iostream>
using namespace std;

typedef	struct
{
	GLubyte	* imageData;									// Image Data (Up To 32 Bits)
	GLuint	bpp;											// Image Color Depth In Bits Per Pixel
	GLuint	width;											// Image Width
	GLuint	height;											// Image Height
	GLuint	texID;											// Texture ID Used To Select A Texture
	GLuint	type;											// Image Type (GL_RGB, GL_RGBA)
} Texture;

//typedef struct
//{
//	GLubyte Header[12];									// TGA File Header
//} TGAHeader;


//typedef struct
//{
//	GLubyte		header[6];								// First 6 Useful Bytes From The Header
//	GLuint		bytesPerPixel;							// Holds Number Of Bytes Per Pixel Used In The TGA File
//	GLuint		imageSize;								// Used To Store The Image Size When Setting Aside Ram
//	GLuint		temp;									// Temporary Variable
//	GLuint		type;
//	GLuint		Height;									//Height of Image
//	GLuint		Width;									//Width ofImage
//	GLuint		Bpp;									// Bits Per Pixel
//} TGA;

/*
TGAHeader tgaheader;*/									// TGA header

/*Lubyte uTGAcompare[12] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };*/	// Uncompressed TGA Header

//supportClass.h
class Point3
{
public:
	float x, y, z;
	void set(float dx, float dy, float dz)
	{
		x = dx; y = dy; z = dz;
	}
	void set(Point3& p)
	{
		x = p.x; y = p.y; z = p.z;
	}
	Point3() { x = y = z = 0; }
	Point3(float dx, float dy, float dz)
	{
		x = dx; y = dy; z = dz;
	}

};

class Vector3
{
public:
	float	x, y, z;
	void set(float dx, float dy, float dz)
	{
		x = dx; y = dy; z = dz;
	}
	void set(Vector3& v)
	{
		x = v.x; y = v.y; z = v.z;
	}
	void flip()
	{
		x = -x; y = -y; z = -z;
	}
	void normalize();
	Vector3() { x = y = z = 0; }
	Vector3(float dx, float dy, float dz)
	{
		x = dx; y = dy; z = dz;
	}
	Vector3(Vector3& v)
	{
		x = v.x; y = v.y; z = v.z;
	}
	Vector3 cross(Vector3 b);
	float dot(Vector3 b);
};
Vector3 Vector3::cross(Vector3 b)
{
	Vector3 c(y*b.z - z*b.y, z*b.x - x*b.z, x*b.y - y*b.x);
	return c;
}
float Vector3::dot(Vector3 b)
{
	return x*b.x + y*b.y + z*b.z;
}
void Vector3::normalize()
{
	float temp = sqrt(x*x + y*y + z*z);
	x = x / temp;
	y = y / temp;
	z = z / temp;
}

class VertexID
{
public:
	int		vertIndex;
	int		colorIndex;
};

class Face
{
public:
	int		nVerts;
	VertexID*	vert;
	Vector3		facenorm;

	Face()
	{
		nVerts = 0;
		vert = NULL;
	}
	~Face()
	{
		if (vert != NULL)
		{
			delete[] vert;
			vert = NULL;
		}
		nVerts = 0;
	}
};

class Mesh
{
public:
	int		numVerts;
	Point3*		pt;

	int		numFaces;
	Face*		face;
public:
	Mesh()
	{
		numVerts = 0;
		pt = NULL;
		numFaces = 0;
		face = NULL;
	}
	~Mesh()
	{
		if (pt != NULL)
		{
			delete[] pt;
		}
		if (face != NULL)
		{
			delete[] face;
		}
		numVerts = 0;
		numFaces = 0;
	}
	void DrawWireframe();

	//lab2
	void CreateTetrahedron();
	void CreateCube(float	fSize);
	void Mesh::CreateCuboid(float	fSizeX, float fSizeY, float	fSizeZ);
	void Mesh::CreateCuboidHole(float	fSizeX, float fSizeY, float	fSizeZ, float n);
	void Mesh::CreateCylinder(float height, float R);
	void Mesh::CreateOvan(float len, float height, float R);
	void Mesh::CreateOvanHole(float len, float height, float R, float r);

	//Lab3
	float	slideX, slideY, slideZ;
	float	rotateX, rotateY, rotateZ;
	float	scaleX, scaleY, scaleZ;
	void Mesh::SetColor(int colorIdx);
	void Mesh::Draw();
	void Mesh::CalculateFacesNorm();
	void Mesh::setupMaterial(float ambient[], float diffuse[], float specular[], float shiness);
};

/////////////////////////////  Mesh.cpp  //////////////////////////////

#define PI			3.1415926

//Ve thanh hinh hop chu nhat.
void Mesh::CreateCuboid(float	fSizeX, float fSizeY, float	fSizeZ)
{
	//int i;

	numVerts = 8;
	pt = new Point3[numVerts];
	pt[0].set(-fSizeX, fSizeY, fSizeZ);
	pt[1].set(fSizeX, fSizeY, fSizeZ);
	pt[2].set(fSizeX, fSizeY, -fSizeZ);
	pt[3].set(-fSizeX, fSizeY, -fSizeZ);
	pt[4].set(-fSizeX, -fSizeY, fSizeZ);
	pt[5].set(fSizeX, -fSizeY, fSizeZ);
	pt[6].set(fSizeX, -fSizeY, -fSizeZ);
	pt[7].set(-fSizeX, -fSizeY, -fSizeZ);

	numFaces = 6;
	face = new Face[numFaces];

	//Left face
	face[0].nVerts = 4;
	face[0].vert = new VertexID[face[0].nVerts];
	face[0].vert[0].vertIndex = 1;
	face[0].vert[1].vertIndex = 5;
	face[0].vert[2].vertIndex = 6;
	face[0].vert[3].vertIndex = 2;

	//Right face
	face[1].nVerts = 4;
	face[1].vert = new VertexID[face[1].nVerts];
	face[1].vert[0].vertIndex = 0;
	face[1].vert[1].vertIndex = 3;
	face[1].vert[2].vertIndex = 7;
	face[1].vert[3].vertIndex = 4;

	//top face
	face[2].nVerts = 4;
	face[2].vert = new VertexID[face[2].nVerts];
	face[2].vert[0].vertIndex = 0;
	face[2].vert[1].vertIndex = 1;
	face[2].vert[2].vertIndex = 2;
	face[2].vert[3].vertIndex = 3;

	//bottom face
	face[3].nVerts = 4;
	face[3].vert = new VertexID[face[3].nVerts];
	face[3].vert[0].vertIndex = 7;
	face[3].vert[1].vertIndex = 6;
	face[3].vert[2].vertIndex = 5;
	face[3].vert[3].vertIndex = 4;

	//near face
	face[4].nVerts = 4;
	face[4].vert = new VertexID[face[4].nVerts];
	face[4].vert[0].vertIndex = 4;
	face[4].vert[1].vertIndex = 5;
	face[4].vert[2].vertIndex = 1;
	face[4].vert[3].vertIndex = 0;

	//Far face
	face[5].nVerts = 4;
	face[5].vert = new VertexID[face[5].nVerts];
	face[5].vert[0].vertIndex = 3;
	face[5].vert[1].vertIndex = 2;
	face[5].vert[2].vertIndex = 6;
	face[5].vert[3].vertIndex = 7;
}

void Mesh::CreateCuboidHole(float	fSizeX, float fSizeY, float	fSizeZ, float n)
{
	int i;

	numVerts = 16;
	pt = new Point3[numVerts];
	pt[0].set(-fSizeX, fSizeY, fSizeZ);
	pt[1].set(fSizeX, fSizeY, fSizeZ);
	pt[2].set(fSizeX, fSizeY, -fSizeZ);
	pt[3].set(-fSizeX, fSizeY, -fSizeZ);
	pt[4].set(-fSizeX, -fSizeY, fSizeZ);
	pt[5].set(fSizeX, -fSizeY, fSizeZ);
	pt[6].set(fSizeX, -fSizeY, -fSizeZ);
	pt[7].set(-fSizeX, -fSizeY, -fSizeZ);


	pt[8].set(-fSizeX / n, fSizeY / n, fSizeZ / n);
	pt[9].set(fSizeX / n, fSizeY / n, fSizeZ / n);
	pt[10].set(fSizeX / n, fSizeY / n, -fSizeZ / n);
	pt[11].set(-fSizeX / n, fSizeY / n, -fSizeZ / n);
	pt[12].set(-fSizeX / n, -fSizeY / n, fSizeZ / n);
	pt[8].set(-fSizeX / n, fSizeY / n, fSizeZ / n);
	pt[13].set(fSizeX / n, -fSizeY / n, fSizeZ / n);
	pt[14].set(fSizeX / n, -fSizeY / n, -fSizeZ / n);
	pt[15].set(-fSizeX / n, -fSizeY / n, -fSizeZ / n);

	numFaces = 16;
	face = new Face[numFaces];

	//Left face
	face[0].nVerts = 4;
	face[0].vert = new VertexID[face[0].nVerts];
	face[0].vert[0].vertIndex = 1;
	face[0].vert[1].vertIndex = 5;
	face[0].vert[2].vertIndex = 6;
	face[0].vert[3].vertIndex = 2;
	for (i = 0; i<face[0].nVerts; i++)
		face[0].vert[i].colorIndex = 0;

	//Right face
	face[1].nVerts = 4;
	face[1].vert = new VertexID[face[1].nVerts];
	face[1].vert[0].vertIndex = 0;
	face[1].vert[1].vertIndex = 3;
	face[1].vert[2].vertIndex = 7;
	face[1].vert[3].vertIndex = 4;
	for (i = 0; i<face[1].nVerts; i++)
		face[1].vert[i].colorIndex = 1;

	//top face1
	face[2].nVerts = 4;
	face[2].vert = new VertexID[face[2].nVerts];
	face[2].vert[0].vertIndex = 0;
	face[2].vert[1].vertIndex = 1;
	face[2].vert[2].vertIndex = 9;
	face[2].vert[3].vertIndex = 8;
	for (i = 0; i<face[2].nVerts; i++)
		face[2].vert[i].colorIndex = 2;
	//top face2
	face[6].nVerts = 4;
	face[6].vert = new VertexID[face[6].nVerts];
	face[6].vert[0].vertIndex = 1;
	face[6].vert[1].vertIndex = 2;
	face[6].vert[2].vertIndex = 10;
	face[6].vert[3].vertIndex = 9;
	for (i = 0; i<face[6].nVerts; i++)
		face[6].vert[i].colorIndex = 6;
	//top face2
	face[7].nVerts = 4;
	face[7].vert = new VertexID[face[7].nVerts];
	face[7].vert[0].vertIndex = 2;
	face[7].vert[1].vertIndex = 3;
	face[7].vert[2].vertIndex = 11;
	face[7].vert[3].vertIndex = 10;
	for (i = 0; i<face[7].nVerts; i++)
		face[7].vert[i].colorIndex = 7;
	//top face4
	face[8].nVerts = 4;
	face[8].vert = new VertexID[face[8].nVerts];
	face[8].vert[0].vertIndex = 3;
	face[8].vert[1].vertIndex = 11;
	face[8].vert[2].vertIndex = 8;
	face[8].vert[3].vertIndex = 0;
	for (i = 0; i<face[8].nVerts; i++)
		face[8].vert[i].colorIndex = 8;



	//bottom face1
	face[9].nVerts = 4;
	face[9].vert = new VertexID[face[9].nVerts];
	face[9].vert[0].vertIndex = 5;
	face[9].vert[1].vertIndex = 6;
	face[9].vert[2].vertIndex = 14;
	face[9].vert[3].vertIndex = 13;
	for (i = 0; i<face[9].nVerts; i++)
		face[9].vert[i].colorIndex = 9;
	//bottom face2
	face[10].nVerts = 4;
	face[10].vert = new VertexID[face[10].nVerts];
	face[10].vert[0].vertIndex = 6;
	face[10].vert[1].vertIndex = 7;
	face[10].vert[2].vertIndex = 15;
	face[10].vert[3].vertIndex = 14;
	for (i = 0; i<face[10].nVerts; i++)
		face[10].vert[i].colorIndex = 10;
	//bottom face3
	face[3].nVerts = 4;
	face[3].vert = new VertexID[face[3].nVerts];
	face[3].vert[0].vertIndex = 7;
	face[3].vert[1].vertIndex = 15;
	face[3].vert[2].vertIndex = 12;
	face[3].vert[3].vertIndex = 4;
	for (i = 0; i<face[3].nVerts; i++)
		face[3].vert[i].colorIndex = 3;
	//bottom face4
	face[11].nVerts = 4;
	face[11].vert = new VertexID[face[11].nVerts];
	face[11].vert[0].vertIndex = 5;
	face[11].vert[1].vertIndex = 13;
	face[11].vert[2].vertIndex = 12;
	face[11].vert[3].vertIndex = 4;
	for (i = 0; i<face[11].nVerts; i++)
		face[11].vert[i].colorIndex = 11;


	//inside face1
	face[12].nVerts = 4;
	face[12].vert = new VertexID[face[12].nVerts];
	face[12].vert[0].vertIndex = 9;
	face[12].vert[1].vertIndex = 10;
	face[12].vert[2].vertIndex = 14;
	face[12].vert[3].vertIndex = 13;
	for (i = 0; i<face[12].nVerts; i++)
		face[12].vert[i].colorIndex = 13;
	//inside face 2
	face[13].nVerts = 4;
	face[13].vert = new VertexID[face[13].nVerts];
	face[13].vert[0].vertIndex = 10;
	face[13].vert[1].vertIndex = 11;
	face[13].vert[2].vertIndex = 15;
	face[13].vert[3].vertIndex = 14;
	for (i = 0; i<face[13].nVerts; i++)
		face[13].vert[i].colorIndex = 12;
	//inside face3
	face[14].nVerts = 4;
	face[14].vert = new VertexID[face[14].nVerts];
	face[14].vert[0].vertIndex = 11;
	face[14].vert[1].vertIndex = 8;
	face[14].vert[2].vertIndex = 12;
	face[14].vert[3].vertIndex = 15;
	for (i = 0; i<face[14].nVerts; i++)
		face[14].vert[i].colorIndex = 19;
	//inside face4
	face[15].nVerts = 4;
	face[15].vert = new VertexID[face[15].nVerts];
	face[15].vert[0].vertIndex = 9;
	face[15].vert[1].vertIndex = 8;
	face[15].vert[2].vertIndex = 12;
	face[15].vert[3].vertIndex = 13;
	for (i = 0; i<face[15].nVerts; i++)
		face[15].vert[i].colorIndex = 20;

	//near face
	face[4].nVerts = 4;
	face[4].vert = new VertexID[face[4].nVerts];
	face[4].vert[0].vertIndex = 4;
	face[4].vert[1].vertIndex = 5;
	face[4].vert[2].vertIndex = 1;
	face[4].vert[3].vertIndex = 0;
	for (i = 0; i<face[4].nVerts; i++)
		face[4].vert[i].colorIndex = 4;

	//Far face
	face[5].nVerts = 4;
	face[5].vert = new VertexID[face[5].nVerts];
	face[5].vert[0].vertIndex = 3;
	face[5].vert[1].vertIndex = 2;
	face[5].vert[2].vertIndex = 6;
	face[5].vert[3].vertIndex = 7;
	for (i = 0; i<face[5].nVerts; i++)
		face[5].vert[i].colorIndex = 5;
}

//ve de
void Mesh::CreateCylinder(float height, float R){
	//int i;
	int j = 0;
	numVerts = 42;
	pt = new Point3[numVerts];
	for (int i = 0; i<20; i = i + 1){
		pt[i].set(R*cos(j*PI / 180), height / 2, R*sin(j*PI / 180));
		j = j - 18;
	}
	j = 0;
	for (int i = 20; i<40; i = i + 1){
		pt[i].set(R*cos(j*PI / 180), -height / 2, R*sin(j*PI / 180));
		j = j - 18;
	}
	pt[40].set(0, height / 2, 0);
	pt[41].set(0, -height / 2, 0);

	numFaces = 60;
	face = new Face[numFaces];
	//Top face
	for (int i = 0; i<20; i = i + 1){
		face[i].nVerts = 3;
		face[i].vert = new VertexID[face[i].nVerts];
		face[i].vert[0].vertIndex = 40;
		face[i].vert[1].vertIndex = i;
		if (i != 19) face[i].vert[2].vertIndex = i + 1;
		else face[i].vert[2].vertIndex = 0;
	}
	//bottom face
	for (int i = 20; i<40; i = i + 1){
		face[i].nVerts = 3;
		face[i].vert = new VertexID[face[i].nVerts];
		face[i].vert[0].vertIndex = 41;
		face[i].vert[1].vertIndex = i;
		if (i != 39) face[i].vert[2].vertIndex = i + 1;
		else face[i].vert[2].vertIndex = 20;
	}
	//around face
	for (int i = 40; i<60; i = i + 1){
		face[i].nVerts = 4;
		face[i].vert = new VertexID[face[i].nVerts];
		face[i].vert[0].vertIndex = i - 40;//19
		face[i].vert[1].vertIndex = i + 20 - 40;//39
		if (i != 59) {
			face[i].vert[2].vertIndex = i + 21 - 40;
			face[i].vert[3].vertIndex = i + 1 - 40;
		}
		else {
			face[i].vert[2].vertIndex = 20;
			face[i].vert[3].vertIndex = 0;
		}
	}
}

void Mesh::CreateOvan(float len, float height, float R){
	//int i;
	int j = 270;
	numVerts = 100;
	pt = new Point3[numVerts];
	for (int i = 0; i<20; i = i + 1){
		pt[i].set(R*cos(j*PI / 180) + len / 2, height / 2, R*sin(j*PI / 180));
		j = j + 9;
	}
	j = 270;
	for (int i = 20; i<40; i = i + 1){
		pt[i].set(R*cos(j*PI / 180) + len / 2, -height / 2, R*sin(j*PI / 180));
		j = j + 9;
	}
	pt[80].set(len / 2, height / 2, 0); //tam R near
	pt[81].set(len / 2, -height / 2, 0); //tam R far

	//half cylinder far
	j = 90;//bat dau ve o 90
	for (int i = 40; i<60; i = i + 1){
		pt[i].set(R*cos(j*PI / 180) - len / 2, height / 2, R*sin(j*PI / 180));
		j = j + 9;
	}
	j = 90;
	for (int i = 60; i<80; i = i + 1){
		pt[i].set(R*cos(j*PI / 180) - len / 2, -height / 2, R*sin(j*PI / 180));
		j = j + 9;
	}
	pt[82].set(-len / 2, height / 2, 0);
	pt[83].set(-len / 2, -height / 2, 0);

	numFaces = 60;
	face = new Face[numFaces];

	//near face
	for (int i = 0; i<19; i = i + 1){
		face[i].nVerts = 4;
		face[i].vert = new VertexID[face[i].nVerts];
		face[i].vert[0].vertIndex = i;
		face[i].vert[1].vertIndex = i + 20;
		face[i].vert[2].vertIndex = i + 21;
		face[i].vert[3].vertIndex = i + 1;
		for (int j = 0; j<face[i].nVerts; j++)
			face[i].vert[j].colorIndex = i;
	}
	//far face
	for (int i = 40; i<59; i = i + 1){
		face[i].nVerts = 4;
		face[i].vert = new VertexID[face[i].nVerts];
		face[i].vert[0].vertIndex = i;
		face[i].vert[1].vertIndex = i + 20;
		face[i].vert[2].vertIndex = i + 21;
		face[i].vert[3].vertIndex = i + 1;
		for (int j = 0; j<face[i].nVerts; j++)
			face[i].vert[j].colorIndex = i;
	}
	//mat phai
	int i = 20;
	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];
	face[i].vert[0].vertIndex = 0;
	face[i].vert[1].vertIndex = 20;
	face[i].vert[2].vertIndex = 79;
	face[i].vert[3].vertIndex = 59;
	for (int j = 0; j<face[i].nVerts; j++)
		face[i].vert[j].colorIndex = i;
	//mat trai
	i = 21;
	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];
	face[i].vert[0].vertIndex = 40;
	face[i].vert[1].vertIndex = 60;
	face[i].vert[2].vertIndex = 39;
	face[i].vert[3].vertIndex = 19;
	for (int j = 0; j<face[i].nVerts; j++)
		face[i].vert[j].colorIndex = i;

	//under face
	i = 25;
	face[i].nVerts = 40;
	face[i].vert = new VertexID[face[i].nVerts];
	for (int j = 0; j<20; j = j + 1){
		face[i].vert[j].vertIndex = j;
	}
	for (int j = 40; j<60; j = j + 1){
		face[i].vert[j - 20].vertIndex = j;
	}
	for (int j = 0; j<face[i].nVerts; j++)
		face[i].vert[j].colorIndex = i;
	//bottom face
	i = 26;
	face[i].nVerts = 40;
	face[i].vert = new VertexID[face[i].nVerts];
	for (int j = 20; j<40; j = j + 1){
		face[i].vert[j - 20].vertIndex = j;
	}
	for (int j = 60; j<80; j = j + 1){
		face[i].vert[j - 40].vertIndex = j;
	}
	for (int j = 0; j<face[i].nVerts; j++)
		face[i].vert[j].colorIndex = i;
}

void Mesh::CreateOvanHole(float len, float height, float R, float r){
	//int i;
	int j = 270;
	numVerts = 200;
	pt = new Point3[numVerts];
	for (int i = 0; i<20; i = i + 1){
		pt[i].set(R*cos(j*PI / 180) + len / 2, height / 2, R*sin(j*PI / 180));
		j = j + 9;
	}
	j = 270;
	for (int i = 20; i<40; i = i + 1){
		pt[i].set(R*cos(j*PI / 180) + len / 2, -height / 2, R*sin(j*PI / 180));
		j = j + 9;
	}
	pt[80].set(len / 2, height / 2, 0); //tam near
	pt[81].set(len / 2, -height / 2, 0); //tam far

	j = 270;
	for (int i = 100; i<120; i = i + 1){
		pt[i].set(r*cos(j*PI / 180) + len / 2, height / 2, r*sin(j*PI / 180));
		j = j + 9;
	}
	j = 270;
	for (int i = 120; i<140; i = i + 1){
		pt[i].set(r*cos(j*PI / 180) + len / 2, -height / 2, r*sin(j*PI / 180));
		j = j + 9;
	}

	//half cylinder far
	j = 90;//bat dau ve o 90
	for (int i = 40; i<60; i = i + 1){
		pt[i].set(R*cos(j*PI / 180) - len / 2, height / 2, R*sin(j*PI / 180));
		j = j + 9;
	}
	j = 90;
	for (int i = 60; i<80; i = i + 1){
		pt[i].set(R*cos(j*PI / 180) - len / 2, -height / 2, R*sin(j*PI / 180));
		j = j + 9;
	}
	j = 90;//bat dau ve o 90
	for (int i = 140; i<160; i = i + 1){
		pt[i].set(r*cos(j*PI / 180) - len / 2, height / 2, r*sin(j*PI / 180));
		j = j + 9;
	}
	j = 90;
	for (int i = 160; i<180; i = i + 1){
		pt[i].set(r*cos(j*PI / 180) - len / 2, -height / 2, r*sin(j*PI / 180));
		j = j + 9;
	}
	pt[82].set(-len / 2, height / 2, 0);
	pt[83].set(-len / 2, -height / 2, 0);

	numFaces = 300;
	face = new Face[numFaces];


	//near face
	for (int i = 0; i<19; i = i + 1){
		face[i].nVerts = 4;
		face[i].vert = new VertexID[face[i].nVerts];
		face[i].vert[0].vertIndex = i;
		face[i].vert[1].vertIndex = i + 20;
		face[i].vert[2].vertIndex = i + 21;
		face[i].vert[3].vertIndex = i + 1;
		for (int j = 0; j<face[i].nVerts; j++)
			face[i].vert[j].colorIndex = i;
	}
	//near face 2
	for (int i = 100; i<119; i = i + 1){
		face[i].nVerts = 4;
		face[i].vert = new VertexID[face[i].nVerts];
		face[i].vert[0].vertIndex = i;
		face[i].vert[1].vertIndex = i + 20;
		face[i].vert[2].vertIndex = i + 21;
		face[i].vert[3].vertIndex = i + 1;
		for (int j = 0; j<face[i].nVerts; j++)
			face[i].vert[j].colorIndex = i;
	}

	//far face
	for (int i = 40; i<59; i = i + 1){
		face[i].nVerts = 4;
		face[i].vert = new VertexID[face[i].nVerts];
		face[i].vert[0].vertIndex = i;
		face[i].vert[1].vertIndex = i + 20;
		face[i].vert[2].vertIndex = i + 21;
		face[i].vert[3].vertIndex = i + 1;
		for (int j = 0; j<face[i].nVerts; j++)
			face[i].vert[j].colorIndex = i;
	}
	for (int i = 140; i<159; i = i + 1){
		face[i].nVerts = 4;
		face[i].vert = new VertexID[face[i].nVerts];
		face[i].vert[0].vertIndex = i;
		face[i].vert[1].vertIndex = i + 20;
		face[i].vert[2].vertIndex = i + 21;
		face[i].vert[3].vertIndex = i + 1;
		for (int j = 0; j<face[i].nVerts; j++)
			face[i].vert[j].colorIndex = i;
	}


	//mat phai
	int i = 20;
	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];
	face[i].vert[0].vertIndex = 0;
	face[i].vert[1].vertIndex = 20;
	face[i].vert[2].vertIndex = 79;
	face[i].vert[3].vertIndex = 59;
	for (int j = 0; j<face[i].nVerts; j++)
		face[i].vert[j].colorIndex = i;

	i = 21;
	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];
	face[i].vert[0].vertIndex = 100;
	face[i].vert[1].vertIndex = 120;
	face[i].vert[2].vertIndex = 179;
	face[i].vert[3].vertIndex = 159;
	for (int j = 0; j<face[i].nVerts; j++)
		face[i].vert[j].colorIndex = i;


	//mat trai
	i = 22;
	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];
	face[i].vert[0].vertIndex = 40;
	face[i].vert[1].vertIndex = 60;
	face[i].vert[2].vertIndex = 39;
	face[i].vert[3].vertIndex = 19;
	for (int j = 0; j<face[i].nVerts; j++)
		face[i].vert[j].colorIndex = i;
	//mat trai 2
	i = 23;
	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];
	face[i].vert[0].vertIndex = 140;
	face[i].vert[1].vertIndex = 160;
	face[i].vert[2].vertIndex = 139;
	face[i].vert[3].vertIndex = 119;
	for (int j = 0; j<face[i].nVerts; j++)
		face[i].vert[j].colorIndex = i;


	//2 side under
	i = 24;
	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];
	face[i].vert[0].vertIndex = 0;
	face[i].vert[1].vertIndex = 100;
	face[i].vert[2].vertIndex = 159;
	face[i].vert[3].vertIndex = 59;
	for (int j = 0; j<face[i].nVerts; j++)
		face[i].vert[j].colorIndex = i;
	i = 25;
	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];
	face[i].vert[0].vertIndex = 40;
	face[i].vert[1].vertIndex = 140;
	face[i].vert[2].vertIndex = 119;
	face[i].vert[3].vertIndex = 19;
	for (int j = 0; j<face[i].nVerts; j++)
		face[i].vert[j].colorIndex = i;

	i = 26;
	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];
	face[i].vert[0].vertIndex = 60;
	face[i].vert[1].vertIndex = 160;
	face[i].vert[2].vertIndex = 139;
	face[i].vert[3].vertIndex = 39;
	for (int j = 0; j<face[i].nVerts; j++)
		face[i].vert[j].colorIndex = i;
	i = 27;
	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];
	face[i].vert[0].vertIndex = 20;
	face[i].vert[1].vertIndex = 120;
	face[i].vert[2].vertIndex = 179;
	face[i].vert[3].vertIndex = 79;
	for (int j = 0; j<face[i].nVerts; j++)
		face[i].vert[j].colorIndex = i;

	//////***********near
	for (int i = 0; i<19; i = i + 1){
		face[i + 160].nVerts = 4;
		face[i + 160].vert = new VertexID[face[i + 160].nVerts];
		face[i + 160].vert[0].vertIndex = i;
		face[i + 160].vert[1].vertIndex = i + 1;
		face[i + 160].vert[2].vertIndex = i + 101;
		face[i + 160].vert[3].vertIndex = i + 100;
		for (int j = 0; j<face[i + 160].nVerts; j++)
			face[i + 160].vert[j].colorIndex = i + 160;
	}
	for (int i = 20; i<39; i = i + 1){
		face[i + 160].nVerts = 4;
		face[i + 160].vert = new VertexID[face[i + 160].nVerts];
		face[i + 160].vert[0].vertIndex = i;
		face[i + 160].vert[1].vertIndex = i + 1;
		face[i + 160].vert[2].vertIndex = i + 101;
		face[i + 160].vert[3].vertIndex = i + 100;
		for (int j = 0; j<face[i + 160].nVerts; j++)
			face[i + 160].vert[j].colorIndex = i + 160;
	}
	//////// ((( far

	for (int i = 40; i<59; i = i + 1){
		face[i + 160].nVerts = 4;
		face[i + 160].vert = new VertexID[face[i + 160].nVerts];
		face[i + 160].vert[0].vertIndex = i;
		face[i + 160].vert[1].vertIndex = i + 1;
		face[i + 160].vert[2].vertIndex = i + 101;
		face[i + 160].vert[3].vertIndex = i + 100;
		for (int j = 0; j<face[i + 160].nVerts; j++)
			face[i + 160].vert[j].colorIndex = i + 160;
	}
	for (int i = 60; i<79; i = i + 1){
		face[i + 160].nVerts = 4;
		face[i + 160].vert = new VertexID[face[i + 160].nVerts];
		face[i + 160].vert[0].vertIndex = i;
		face[i + 160].vert[1].vertIndex = i + 1;
		face[i + 160].vert[2].vertIndex = i + 101;
		face[i + 160].vert[3].vertIndex = i + 100;
		for (int j = 0; j<face[i + 160].nVerts; j++)
			face[i + 160].vert[j].colorIndex = i + 160;
	}
}

void Mesh::CreateCube(float	fSize)
{
	int i;

	numVerts = 8;
	pt = new Point3[numVerts];
	pt[0].set(-fSize, fSize, fSize);
	pt[1].set(fSize, fSize, fSize);
	pt[2].set(fSize, fSize, -fSize);
	pt[3].set(-fSize, fSize, -fSize);
	pt[4].set(-fSize, -fSize, fSize);
	pt[5].set(fSize, -fSize, fSize);
	pt[6].set(fSize, -fSize, -fSize);
	pt[7].set(-fSize, -fSize, -fSize);


	numFaces = 6;
	face = new Face[numFaces];

	//Left face
	face[0].nVerts = 4;
	face[0].vert = new VertexID[face[0].nVerts];
	face[0].vert[0].vertIndex = 1;
	face[0].vert[1].vertIndex = 5;
	face[0].vert[2].vertIndex = 6;
	face[0].vert[3].vertIndex = 2;
	for (i = 0; i<face[0].nVerts; i++)
		face[0].vert[i].colorIndex = 0;

	//Right face
	face[1].nVerts = 4;
	face[1].vert = new VertexID[face[1].nVerts];
	face[1].vert[0].vertIndex = 0;
	face[1].vert[1].vertIndex = 3;
	face[1].vert[2].vertIndex = 7;
	face[1].vert[3].vertIndex = 4;
	for (i = 0; i<face[1].nVerts; i++)
		face[1].vert[i].colorIndex = 1;

	//top face
	face[2].nVerts = 4;
	face[2].vert = new VertexID[face[2].nVerts];
	face[2].vert[0].vertIndex = 0;
	face[2].vert[1].vertIndex = 1;
	face[2].vert[2].vertIndex = 2;
	face[2].vert[3].vertIndex = 3;
	for (i = 0; i<face[2].nVerts; i++)
		face[2].vert[i].colorIndex = 2;

	//bottom face
	face[3].nVerts = 4;
	face[3].vert = new VertexID[face[3].nVerts];
	face[3].vert[0].vertIndex = 7;
	face[3].vert[1].vertIndex = 6;
	face[3].vert[2].vertIndex = 5;
	face[3].vert[3].vertIndex = 4;
	for (i = 0; i<face[3].nVerts; i++)
		face[3].vert[i].colorIndex = 3;

	//near face
	face[4].nVerts = 4;
	face[4].vert = new VertexID[face[4].nVerts];
	face[4].vert[0].vertIndex = 4;
	face[4].vert[1].vertIndex = 5;
	face[4].vert[2].vertIndex = 1;
	face[4].vert[3].vertIndex = 0;
	for (i = 0; i<face[4].nVerts; i++)
		face[4].vert[i].colorIndex = 4;

	//Far face
	face[5].nVerts = 4;
	face[5].vert = new VertexID[face[5].nVerts];
	face[5].vert[0].vertIndex = 3;
	face[5].vert[1].vertIndex = 2;
	face[5].vert[2].vertIndex = 6;
	face[5].vert[3].vertIndex = 7;
	for (i = 0; i<face[5].nVerts; i++)
		face[5].vert[i].colorIndex = 5;

}

void Mesh::CreateTetrahedron()
{
	int i;
	numVerts = 4;
	pt = new Point3[numVerts];
	pt[0].set(0, 0, 0);
	pt[1].set(1, 0, 0);
	pt[2].set(0, 1, 0);
	pt[3].set(0, 0, 1);

	numFaces = 4;
	face = new Face[numFaces];

	face[0].nVerts = 3;
	face[0].vert = new VertexID[face[0].nVerts];
	face[0].vert[0].vertIndex = 1;
	face[0].vert[1].vertIndex = 2;
	face[0].vert[2].vertIndex = 3;
	for (i = 0; i<face[0].nVerts; i++)
		face[0].vert[i].colorIndex = 0;


	face[1].nVerts = 3;
	face[1].vert = new VertexID[face[1].nVerts];
	face[1].vert[0].vertIndex = 0;
	face[1].vert[1].vertIndex = 2;
	face[1].vert[2].vertIndex = 1;
	for (i = 0; i<face[1].nVerts; i++)
		face[1].vert[i].colorIndex = 1;


	face[2].nVerts = 3;
	face[2].vert = new VertexID[face[2].nVerts];
	face[2].vert[0].vertIndex = 0;
	face[2].vert[1].vertIndex = 3;
	face[2].vert[2].vertIndex = 2;
	for (i = 0; i<face[2].nVerts; i++)
		face[2].vert[i].colorIndex = 2;


	face[3].nVerts = 3;
	face[3].vert = new VertexID[face[3].nVerts];
	face[3].vert[0].vertIndex = 1;
	face[3].vert[1].vertIndex = 3;
	face[3].vert[2].vertIndex = 0;
	for (i = 0; i<face[3].nVerts; i++)
		face[3].vert[i].colorIndex = 3;
}


void Mesh::SetColor(int colorIdx){
	for (int f = 0; f < numFaces; f++){
		for (int v = 0; v < face[f].nVerts; v++){
			face[f].vert[v].colorIndex = colorIdx;
		}
	}
}
void Mesh::DrawWireframe()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for (int f = 0; f < numFaces; f++)
	{
		glBegin(GL_POLYGON);
		for (int v = 0; v < face[f].nVerts; v++)
		{
			int		iv = face[f].vert[v].vertIndex;

			glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
		}
		glEnd();
	}
}

void Mesh::CalculateFacesNorm(){
	for (int f = 0; f < numFaces; f++){
		float x = 0.0, y = 0.0, z = 0.0;
		for (int j = 0; j < face[f].nVerts; j++){
			int vert = face[f].vert[j].vertIndex;
			int next = (j + 1) % (face[f].nVerts);
			int vert_next = face[f].vert[next].vertIndex;

			x += (pt[vert].y - pt[vert_next].y)*(pt[vert].z + pt[vert_next].z);
			y += (pt[vert].z - pt[vert_next].z)*(pt[vert].x + pt[vert_next].x);
			z += (pt[vert].x - pt[vert_next].x)*(pt[vert].y + pt[vert_next].y);
		}
		face[f].facenorm.set(x, y, z);
		face[f].facenorm.normalize();
	}
}
void Mesh::Draw() {
	for (int f = 0; f < numFaces; f++){
		glBegin(GL_POLYGON);
		for (int v = 0; v < face[f].nVerts; v++){
			int		iv = face[f].vert[v].vertIndex;
			glNormal3f(face[f].facenorm.x, face[f].facenorm.y, -face[f].facenorm.z);
			glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
		}
		glEnd();
	}
}
void Mesh::setupMaterial(float ambient[], float diffuse[], float specular[], float shiness)
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shiness);
}

//************* end Mesh.cpp ****************

//Main

int		screenWidth = 600;
int		screenHeight = 600;

bool	bWireFrame = false; //khung or mau
bool	animation = false;

float	rCircle = 0.5;
float	baseHeight = 0.3;
float	baseRotateStep = 5;

float	columnSizeX = 0.1;
float	columnSizeZ = columnSizeX;
float	columnSizeY = 2;

float R = (20 / 13.0)*columnSizeX; //ban kinh ovan
float lenY = 0.2*columnSizeY; //chieu dai thnah truot duoi T
float r = (10 / 13.0)*columnSizeX;
float gd2_len = 0.7*columnSizeY; //chieu dai thanh gia do 2

float tq_ov_len = 0.3*columnSizeY;
float tq_ov_height = 2 * columnSizeZ;
float tq_ov_R = r;

//set camera
//float	fHalfSize=4;
float	camera_angle = 0.01;
float	camera_height = 2;
float	camera_dis = 6;
float	camera_X, camera_Y, camera_Z;
float	lookAt_X = 0, lookAt_Y = camera_height, lookAt_Z = camera_dis;

float	fHalfSize;
bool	b4View = false; //che do 4 goc nhin
bool	light2 = true;


float floorShadow[4][4];	// matrix used to calculate our projected floor shadow
float floorShadow2[4][4];

float groundplane[] = { 0.0f, 1.0f, 0.0f, 0.0f }; // The normal of the floor

GLfloat	lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat	lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat	lightAmbient[] = { 0.4f, 0.4f, 0.4f, 1.0f };
GLfloat light_position1[] = { 6.0f, 6.0f, 6.0f, 0.0f };


GLfloat	lightDiffuse2[] = { 0.24f, 0.24f, 0.24f, 1.0f };
GLfloat	lightSpecular2[] = { 0.53f, 0.53f, 0.53f, 1.0f };
GLfloat	lightAmbient2[] = { 0.14f, 0.14f, 0.14f, 1.0f };
GLfloat light_position2[] = { -6.0f, 6.0f, -6.0f, 0.0f };

Mesh	base;
Mesh	base1;
Mesh	base2;
Mesh	base3;
Mesh	column;
Mesh	column1;
Mesh	column2;
Mesh	column3;
Mesh	column4;
Mesh	tt1;
Mesh	tt_ov;
Mesh	tt_ov1;
Mesh	tt_ov2;
Mesh	tq_tru; //tay quay
Mesh	tq_tru1;
Mesh	tq_tru2;
Mesh	tq_tru3;
Mesh	tq_tru4;


Mesh	tt2;
Mesh	gd2; //gia do2
Mesh	tq_ov;
Mesh	hh_dac1; Mesh hh_rong1;
Mesh	hh_dac2; Mesh hh_rong2;

Texture   floorTex;


void Timer(int value){
	if (animation){
		base.rotateY += 2;
		base1.rotateY += 2;
		base2.rotateY += 2;
		tq_ov.rotateZ += 2;
		if (tq_ov.rotateZ > 360)
			tq_ov.rotateZ -= 360;
		if (base.rotateY > 360)
			base.rotateY -= 360;
		if (base1.rotateY > 360)
			base1.rotateY -= 360;
		if (base2.rotateY > 360)
			base2.rotateY -= 360;
		glutPostRedisplay();
		// 100 milliseconds
		glutTimerFunc(100, Timer, 0);
	}
}


void myKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '1':
		base.rotateY += baseRotateStep;
		base1.rotateY += baseRotateStep;
		base2.rotateY += baseRotateStep;
		if (base.rotateY > 360)
			base.rotateY -= 360;
		if (base1.rotateY > 360)
			base1.rotateY -= 360;
		if (base2.rotateY > 360)
			base2.rotateY -= 360;
		break;
	case '2':
		base.rotateY -= baseRotateStep;
		base1.rotateY -= baseRotateStep;
		base2.rotateY -= baseRotateStep;
		if (base.rotateY < 0)
			base.rotateY += 360;
		if (base1.rotateY < 0)
			base1.rotateY += 360;
		if (base2.rotateY < 0)
			base2.rotateY += 360;
		break;
	case '3':
		tq_ov.rotateZ += baseRotateStep;
		if (tq_ov.rotateZ > 360)
			tq_ov.rotateZ -= 360;
		break;
	case '4':
		tq_ov.rotateZ -= baseRotateStep;
		if (tq_ov.rotateZ < 0)
			tq_ov.rotateZ += 360;
		break;
	case 'R':
	case 'r':
		base.rotateY = 0;
		base1.rotateY = 0;
		base2.rotateY = 0;
		tq_ov.rotateZ = 0;
		break;
	case 'W':
	case 'w':
		bWireFrame = !bWireFrame;
		break;
	case 'a':
	case 'A':
		animation = !animation;
		Timer(0);
		break;
	case '+':
		camera_dis += 0.2;
		fHalfSize += 0.02;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-fHalfSize, fHalfSize, -fHalfSize + 1.5, fHalfSize + 1.5, -1000, 1000);
		break;
	case '-':
		camera_dis -= 2;
		fHalfSize -= 0.02;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-fHalfSize, fHalfSize, -fHalfSize + 1.5, fHalfSize + 1.5, -1000, 1000);
		break;

	case 'V':
	case 'v':
		b4View = !b4View;
		break;

		//Ass2:
	case 'D':
	case 'd':
		light2 = !light2;
		float light_position2[] = { 0, 0, 0, 0 };
		break;
	}
	glutPostRedisplay();
}
void mySpecial(int key, int x, int y){
	switch (key)
	{
	case GLUT_KEY_UP:
		camera_height += 0.1;
		lookAt_Y = camera_height;
		if (camera_height<0) fHalfSize -= 0.02;
		else fHalfSize += 0.02;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-fHalfSize, fHalfSize, -fHalfSize + 1.5, fHalfSize + 1.5, -1000, 1000);
		break;
	case GLUT_KEY_DOWN:
		camera_height -= 0.1;
		lookAt_Y = camera_height;
		if (camera_height<0) fHalfSize += 0.02;
		else fHalfSize -= 0.02;


		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-fHalfSize, fHalfSize, -fHalfSize + 1.5, fHalfSize + 1.5, -1000, 1000);
		break;
	case GLUT_KEY_LEFT:
		camera_angle += 0.7;
		if (camera_angle > 360)
			camera_angle -= 360;
		lookAt_X = camera_dis*sin(camera_angle*PI / 180.0);
		lookAt_Z = camera_dis*cos(camera_angle*PI / 180.0);
		break;
	case GLUT_KEY_RIGHT:
		camera_angle -= 0.7;
		if (camera_angle > 360)
			camera_angle -= 360;
		lookAt_X = camera_dis*sin(camera_angle*PI / 180.0);
		lookAt_Z = camera_dis*cos(camera_angle*PI / 180.0);
		break;
	}
	glutPostRedisplay();
}
void drawAxis()
{
	glColor3f(0, 0, 1);
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);//x
	glVertex3f(5, 0, 0);

	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);//y
	glVertex3f(0, 5, 0);

	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);//z
	glVertex3f(0, 0, 5);
	glEnd();
}

void drawBase()
{
	glPushMatrix();

	base.CalculateFacesNorm();

	glRotatef(base.rotateY, 0, 1, 0);

	GLfloat amb[] = { 0.18f, 0.09f, 0.99f, 1.0f };
	GLfloat diff[] = { 0.67f, 0.66f, 0.66f, 1.0f };
	GLfloat spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat shine = 100.0f;

	base.setupMaterial(amb, diff, spec, shine);


	if (bWireFrame)
		base.DrawWireframe();
	else
		base.Draw();


	glPopMatrix();
}
void drawBase1()
{
	glPushMatrix();

	base1.CalculateFacesNorm();

	glTranslated(0, 0.1, 0); //tinh tien y>0
	glRotatef(base1.rotateY, 0, 1, 0);

	GLfloat amb[] = { 0.18f, 0.09f, 0.99f, 1.0f };
	GLfloat diff[] = { 0.67f, 0.66f, 0.66f, 1.0f };
	GLfloat spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat shine = 100.0f;

	base1.setupMaterial(amb, diff, spec, shine);

	if (bWireFrame)
		base1.DrawWireframe();
	else
		base1.Draw();

	glPopMatrix();
}
void drawBase2()
{
	glPushMatrix();

	base2.CalculateFacesNorm();

	glTranslated(0, 0.6, 0); //tinh tien y>0
	glRotatef(base2.rotateY, 0, 1, 0);

	GLfloat amb[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	GLfloat diff[] = { 0.67f, 0.66f, 0.66f, 1.0f };
	GLfloat spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat shine = 100.0f;

	base2.setupMaterial(amb, diff, spec, shine);

	if (bWireFrame)
		base2.DrawWireframe();
	else
		base2.Draw();

	glPopMatrix();
}
void drawBase3()
{
	glPushMatrix();

	base3.CalculateFacesNorm();

	glTranslated(0, 0.95, 0); //tinh tien y>0
	glRotatef(base2.rotateY, 0, 1, 0);

	GLfloat amb[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	GLfloat diff[] = { 0.67f, 0.66f, 0.66f, 1.0f };
	GLfloat spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat shine = 100.0f;

	base3.setupMaterial(amb, diff, spec, shine);

	if (bWireFrame)
		base3.DrawWireframe();
	else
		base3.Draw();

	glPopMatrix();
}
void drawFaceTable()
{
	glPushMatrix();
	glTranslated(0, 0.85, 0);
	glRotatef(90, 1, 0, 0);

	GLfloat amb[] = { 1.0f, 0.0f, 0.5f, 1.0f };
	GLfloat diff[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat shine = 10.0f;

	column.setupMaterial(amb, diff, spec, shine);

	if (bWireFrame)
		column.DrawWireframe();
	else
		column.Draw();
	glPopMatrix();
}
void drawFaceTable1()
{
	glPushMatrix();
	glTranslated(-1, 0.9, 0);
	glRotatef(90, 1, 0, 0);

	GLfloat amb[] = { 1.0f, 0.9f, 1.0f, 1.0f };
	GLfloat diff[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat shine = 10.0f;

	column1.setupMaterial(amb, diff, spec, shine);

	if (bWireFrame)
		column1.DrawWireframe();
	else
		column1.Draw();
	glPopMatrix();
}
void drawFaceTable2()
{
	glPushMatrix();
	glTranslated(1, 0.9, 0);
	glRotatef(90, 1, 0, 0);

	GLfloat amb[] = { 1.0f, 0.9f, 1.0f, 1.0f };
	GLfloat diff[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat shine = 10.0f;

	column2.setupMaterial(amb, diff, spec, shine);

	if (bWireFrame)
		column2.DrawWireframe();
	else
		column2.Draw();
	glPopMatrix();
}
void drawFaceTable3()
{
	glPushMatrix();
	glTranslated(-1, 1, 0);
	glRotatef(90, 1, 0, 0);

	GLfloat amb[] = { 1.0f, 0.9f, 1.0f, 1.0f };
	GLfloat diff[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat shine = 10.0f;

	column3.setupMaterial(amb, diff, spec, shine);

	if (bWireFrame)
		column3.DrawWireframe();
	else
		column3.Draw();
	glPopMatrix();
}
void drawFaceTable4()
{
	glPushMatrix();
	glTranslated(1, 1, 0);
	glRotatef(90, 1, 0, 0);

	GLfloat amb[] = { 1.0f, 0.9f, 1.0f, 1.0f };
	GLfloat diff[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat shine = 10.0f;

	column4.setupMaterial(amb, diff, spec, shine);

	if (bWireFrame)
		column4.DrawWireframe();
	else
		column4.Draw();
	glPopMatrix();
}
void drawTt_ov(){
	glPushMatrix();
	//glRotatef(base.rotateY, 0, 1, 0);

	//glTranslated(0, -R + 1.1*columnSizeY - 0, 6 * columnSizeZ);
	//glTranslated(0, -(tq_ov_len)*(1 - cos(tq_ov.rotateZ*PI / 180)), 0);

	glTranslatef(0, 1.12, 0);
	glRotatef(90, 0, 1, 0);///quay 90

	GLfloat amb[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	GLfloat diff[] = { 0.25f, 0.25f, 0.25f, 1.0f };
	GLfloat spec[] = { 0.9f, 1.0f, 1.0f, 1.0f };
	GLfloat shine = 30.0f;

	tt_ov.setupMaterial(amb, diff, spec, shine);
	if (bWireFrame)
		tt_ov.DrawWireframe();
	else
		tt_ov.Draw();

	glPopMatrix();
}
void drawTt_ov1(){
	glPushMatrix();
	//glRotatef(base.rotateY, 0, 1, 0);

	//glTranslated(0, -R + 1.1*columnSizeY - 0, 6 * columnSizeZ);
	//glTranslated(0, -(tq_ov_len)*(1 - cos(tq_ov.rotateZ*PI / 180)), 0);

	glTranslatef(-1, 1, 0);
	glRotatef(90, 0, 0, 1);

	GLfloat amb[] = { 1.0f, 0.9f, 1, 1.0f };
	GLfloat diff[] = { 0.25f, 0.25f, 0.25f, 1.0f };
	GLfloat spec[] = { 0.9f, 1.0f, 1.0f, 1.0f };
	GLfloat shine = 30.0f;

	tt_ov1.setupMaterial(amb, diff, spec, shine);
	if (bWireFrame)
		tt_ov1.DrawWireframe();
	else
		tt_ov1.Draw();

	glPopMatrix();
}
void drawTt_ov2(){
	glPushMatrix();
	//glRotatef(base.rotateY, 0, 1, 0);

	//glTranslated(0, -R + 1.1*columnSizeY - 0, 6 * columnSizeZ);
	//glTranslated(0, -(tq_ov_len)*(1 - cos(tq_ov.rotateZ*PI / 180)), 0);

	glTranslatef(1, 1, 0);
	glRotatef(90, 0, 0, 1);

	GLfloat amb[] = { 1.0f, 0.9f, 1, 1.0f };
	GLfloat diff[] = { 0.25f, 0.25f, 0.25f, 1.0f };
	GLfloat spec[] = { 0.9f, 1.0f, 1.0f, 1.0f };
	GLfloat shine = 30.0f;

	tt_ov.setupMaterial(amb, diff, spec, shine);
	if (bWireFrame)
		tt_ov2.DrawWireframe();
	else
		tt_ov2.Draw();

	glPopMatrix();
}
void drawTayquay_tru(){
	glPushMatrix();
	glRotatef(tq_ov.rotateZ, 0, 1, 0);
	glTranslatef(0, 1.12, -0.5);

	GLfloat amb[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat diff[] = { 0.67f, 0.66f, 0.66f, 1.0f };
	GLfloat spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat shine = 100.0f;

	tq_tru.setupMaterial(amb, diff, spec, shine);
	if (bWireFrame)
		tq_tru.DrawWireframe();
	else
		tq_tru.Draw();
	glPopMatrix();
}
void drawTayquay_tru1(){
	glPushMatrix();
	glTranslatef(-1, 1.12, 0);
	glRotatef(90, 0, 0, 1);
	GLfloat amb[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat diff[] = { 0.67f, 0.66f, 0.66f, 1.0f };
	GLfloat spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat shine = 100.0f;
	tq_tru1.setupMaterial(amb, diff, spec, shine);
	if (bWireFrame)
		tq_tru1.DrawWireframe();
	else
		tq_tru1.Draw();
	glPopMatrix();
}
void drawTayquay_tru2(){
	glPushMatrix();
	glTranslatef(1, 1.12, 0);
	glRotatef(90, 0, 0, 1);
	GLfloat amb[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat diff[] = { 0.67f, 0.66f, 0.66f, 1.0f };
	GLfloat spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat shine = 100.0f;
	tq_tru2.setupMaterial(amb, diff, spec, shine);
	if (bWireFrame)
		tq_tru2.DrawWireframe();
	else
		tq_tru2.Draw();
	glPopMatrix();
}void drawTayquay_tru3(){
	glPushMatrix();

	glTranslatef(-0.3, 1.12, 0);
	glRotatef(90, 0, 0, 1);

	GLfloat amb[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat diff[] = { 0.67f, 0.66f, 0.66f, 1.0f };
	GLfloat spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat shine = 100.0f;

	tq_tru3.setupMaterial(amb, diff, spec, shine);
	if (bWireFrame)
		tq_tru3.DrawWireframe();
	else
		tq_tru3.Draw();
	glPopMatrix();
}
void drawTayquay_tru4(){
	glPushMatrix();

	glTranslatef(0.3, 1.12, 0);
	glRotatef(90, 0, 0, 1);

	GLfloat amb[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat diff[] = { 0.67f, 0.66f, 0.66f, 1.0f };
	GLfloat spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat shine = 100.0f;

	tq_tru4.setupMaterial(amb, diff, spec, shine);
	if (bWireFrame)
		tq_tru4.DrawWireframe();
	else
		tq_tru4.Draw();
	glPopMatrix();
}



void drawTt_1(){
	glPushMatrix();

	glRotatef(base.rotateY, 0, 1, 0);
	glTranslated(0, lenY + 1.1*columnSizeY - (0.005) - 0, 6 * columnSizeZ); //tinh tiens
	glTranslated(0, -(tq_ov_len)*(1 - cos(tq_ov.rotateZ*PI / 180)), 0);

	GLfloat amb[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	GLfloat diff[] = { 0.25f, 0.25f, 0.25f, 1.0f };
	GLfloat spec[] = { 0.9f, 1.0f, 1.0f, 1.0f };
	GLfloat shine = 30.0f;

	tt1.setupMaterial(amb, diff, spec, shine);
	if (bWireFrame)
		tt1.DrawWireframe();
	else
		tt1.Draw();
	glPopMatrix();
}


//Thanh truot chu T 2
void drawTt_2(){
	glPushMatrix();

	glRotatef(base.rotateY, 0, 1, 0);					// quay

	glTranslated((lenY + R) - tq_ov_len - (0.005), baseHeight + 0.6*columnSizeY, 4 * columnSizeZ); //tinh tiens

	glTranslated((tq_ov_len)*(1 - sin(tq_ov.rotateZ*PI / 180)), 0, 0);

	glRotatef(90, 0, 0, 1);		//quay 90

	GLfloat amb[] = { 1.0f, 0.0f, 0.63f, 1.0f };
	GLfloat diff[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat shine = 7.0f;

	tt2.setupMaterial(amb, diff, spec, shine);
	if (bWireFrame)
		tt2.DrawWireframe();
	else
		tt2.Draw();

	glPopMatrix();
}

//ve gia do 2
void drawgiado2(){
	glPushMatrix();

	glRotatef(base.rotateY, 0, 1, 0);					// quay

	glTranslated(gd2_len + columnSizeX, baseHeight + 0.6*columnSizeY, 0); //tinh tiens
	glRotatef(90, 0, 0, 1);		//quay 90

	GLfloat amb[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat diff[] = { 0.82f, 0.82f, 0.82f, 1.0f };
	GLfloat spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat shine = 128.0f;
	gd2.setupMaterial(amb, diff, spec, shine);
	if (bWireFrame)
		gd2.DrawWireframe();
	else
		gd2.Draw();

	glPopMatrix();
}
void drawhh_dac2(){
	glPushMatrix();

	glRotatef(base.rotateY, 0, 1, 0);					// quay
	glTranslated((5 / 4.0)*gd2_len, baseHeight + 0.6*columnSizeY, 2 * columnSizeZ); //tinh tiens

	GLfloat amb[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat diff[] = { 0.82f, 0.82f, 0.82f, 1.0f };
	GLfloat spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat shine = 128.0f;
	hh_dac2.setupMaterial(amb, diff, spec, shine);
	if (bWireFrame)
		hh_dac2.DrawWireframe();
	else
		hh_dac2.Draw();

	glPopMatrix();
}
void drawhh_rong2(){
	glPushMatrix();

	glRotatef(base.rotateY, 0, 1, 0);					// quay

	glTranslated((5 / 4.0)*gd2_len, baseHeight + 0.6*columnSizeY, 4 * columnSizeZ); //tinh tiens
	glRotatef(90, 0, 0, 1);		//quay 90

	GLfloat amb[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat diff[] = { 0.82f, 0.82f, 0.82f, 1.0f };
	GLfloat spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat shine = 128.0f;
	hh_rong2.setupMaterial(amb, diff, spec, shine);

	if (bWireFrame)
		hh_rong2.DrawWireframe();
	else
		hh_rong2.Draw();

	glPopMatrix();
}

//ve gia do 1
void drawhh_dac1(){
	glPushMatrix();

	glRotatef(base.rotateY, 0, 1, 0);					// quay
	glTranslated(0, baseHeight + 1.4*columnSizeY, 3 * columnSizeZ); //tinh tiens

	GLfloat amb[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat diff[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat shine = 128.0f;

	hh_dac1.setupMaterial(amb, diff, spec, shine);
	if (bWireFrame)
		hh_dac1.DrawWireframe();
	else
		hh_dac1.Draw();

	glPopMatrix();
}
void drawhh_rong1(){
	glPushMatrix();

	glRotatef(base.rotateY, 0, 1, 0);					// quay
	glTranslated(0, baseHeight + 1.4*columnSizeY, 6 * columnSizeZ); //tinh tiens

	GLfloat amb[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat diff[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat shine = 128.0f;

	hh_rong1.setupMaterial(amb, diff, spec, shine);
	if (bWireFrame)
		hh_rong1.DrawWireframe();
	else
		hh_rong1.Draw();

	glPopMatrix();
}


void drawTayquay_ovan(){
	glPushMatrix();

	glRotatef(base.rotateY, 0, 1, 0);					// quay
	glTranslated(0, (-tq_ov_len - tq_ov_R) + 1.1*columnSizeY - (R - r), 2 * columnSizeZ); //tinh tiens
	glRotatef(90, 0, 1, 0);		//quay 90
	glRotatef(90, 0, 0, 1);

	glRotatef(tq_ov.rotateZ, 0, 1, 0);
	glTranslated(tq_ov_len / 2.0, 0, 0);

	GLfloat amb[] = { 0.0f, 1.0f, 0.0f, 1.0f };
	GLfloat diff[] = { 0.0f, 0.24f, 0.0f, 1.0f };
	GLfloat spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat shine = 128.0f;

	tq_ov.setupMaterial(amb, diff, spec, shine);
	if (bWireFrame)
		tq_ov.DrawWireframe();
	else
		tq_ov.Draw();

	glPopMatrix();
}

void draw(){
	drawBase();
	drawBase1();
	drawBase2();
	drawBase3();
	drawFaceTable();
	drawFaceTable1();
	drawFaceTable2();
	drawFaceTable3();
	drawFaceTable4();
	drawTt_ov();
	drawTt_ov1();
	drawTt_ov2();
	drawTt_2();
	drawTayquay_tru();
	drawTayquay_tru1();
	drawTayquay_tru2();
	drawTayquay_tru3();
	drawTayquay_tru4();

	drawgiado2();
	drawTayquay_ovan();
	drawhh_dac2();
	drawhh_rong2();
	drawhh_dac1();
	drawhh_rong1();

}


void DrawFloor(){
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, floorTex.texID);
	//glColor4f(1, 1, 1, 0.0);

	int x = 12;
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex3f(-x, 0, -x);
	glTexCoord2f(1, 1);
	glVertex3f(x, 0, -x);
	glTexCoord2f(1, 0);
	glVertex3f(x, 0, x);
	glTexCoord2f(0, 0);
	glVertex3f(-x, 0, x);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}

//draw bóng guong
void drawReflect(){
	glColorMask(0, 0, 0, 0);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glDisable(GL_DEPTH_TEST);
	DrawFloor();
	glColorMask(1, 1, 1, 1);
	glStencilFunc(GL_EQUAL, 1, 1);
	//kich hoat che do depth de ko thay mat khuat sau
	glEnable(GL_DEPTH_TEST);

	// Do not need to write stencil, just keep everything there
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	glPushMatrix();// mirror the object against the XZ plane
	glScalef(1, -1, 1);
	draw();
	glPopMatrix();
	// stencil only used for limit the reflected object display on the floor.
	// We do not need it now, so shut it down.
	glDisable(GL_STENCIL_TEST);

	//tr?n cho m? ?o
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glColor4f(1.0f, 1.0f, 1.0f, 0.7f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	DrawFloor();
	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);

}

void shadowmatrix(float shadowMat[4][4], float groundplane[4], float lightpos[4])
{
	// Find the dot product between the light position vector and the ground plane normal
	float dot = groundplane[0] * lightpos[0] + groundplane[1] * lightpos[1] + groundplane[2] * lightpos[2] + groundplane[3] * lightpos[3];

	shadowMat[0][0] = dot - lightpos[0] * groundplane[0];
	shadowMat[1][0] = 0.f - lightpos[0] * groundplane[1];
	shadowMat[2][0] = 0.f - lightpos[0] * groundplane[2];
	shadowMat[3][0] = 0.f - lightpos[0] * groundplane[3];

	shadowMat[0][1] = 0.f - lightpos[1] * groundplane[0];
	shadowMat[1][1] = dot - lightpos[1] * groundplane[1];
	shadowMat[2][1] = 0.f - lightpos[1] * groundplane[2];
	shadowMat[3][1] = 0.f - lightpos[1] * groundplane[3];

	shadowMat[0][2] = 0.f - lightpos[2] * groundplane[0];
	shadowMat[1][2] = 0.f - lightpos[2] * groundplane[1];
	shadowMat[2][2] = dot - lightpos[2] * groundplane[2];
	shadowMat[3][2] = 0.f - lightpos[2] * groundplane[3];

	shadowMat[0][3] = 0.f - lightpos[3] * groundplane[0];
	shadowMat[1][3] = 0.f - lightpos[3] * groundplane[1];
	shadowMat[2][3] = 0.f - lightpos[3] * groundplane[2];
	shadowMat[3][3] = dot - lightpos[3] * groundplane[3];


}


void drawShadow(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // Clear Screen, Depth Buffer and Stencil Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix

	//gluLookAt(0,100,200,  0,0,0,  0,1,0);
	gluLookAt(lookAt_X, lookAt_Y, lookAt_Z, 0, 0, 0, 0, 1, 0);

	// Turn off writing to the Color Buffer and Depth Buffer
	// We want to draw to the Stencil Buffer only

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDepthMask(GL_FALSE);

	// Enable the Stencil Buffer
	glEnable(GL_STENCIL_TEST);

	// Set 1 into the stencil buffer
	glStencilFunc(GL_ALWAYS, 1, 0xFFFFFFFF);
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

	DrawFloor();

	// Turn on Color Buffer and Depth Buffer
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);


	// Only write to the Stencil Buffer where 1 is set
	glStencilFunc(GL_EQUAL, 1, 0xFFFFFFFF);
	// Keep the content of the Stencil Buffer
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	glPushMatrix();
	glColor4f(0.0, 0.0, 0.0, 0.9f);
	// Disable light
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	// Enable blending
	glEnable(GL_BLEND);
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);

	// Calculate the projected shadow
	shadowmatrix(floorShadow, groundplane, light_position1); //lightPosition
	glMultMatrixf((float *)floorShadow);
	draw();

	if (light2){
		shadowmatrix(floorShadow2, groundplane, light_position2); //lightPosition
		glMultMatrixf((float *)floorShadow2);
		draw();
	}

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glDisable(GL_STENCIL_TEST);
	glEnable(GL_DEPTH_TEST);
}



void _4view(){
	if (!b4View){
		glViewport(0, 0, screenWidth, screenHeight);
		glLoadIdentity();
		gluLookAt(lookAt_X, lookAt_Y, lookAt_Z, 0, 0, 0, 0, 1, 0);

		drawShadow();
		drawReflect();
		draw();
	}
	else {//case 'V/v'
		glViewport(0, 0, screenWidth / 2.0, screenHeight / 2.0);
		glLoadIdentity();
		gluLookAt(0, 10, 3, 0, 0, 0, 0, 1, 0);
		draw();

		glViewport(0, screenHeight / 2.0, screenWidth / 2.0, screenHeight / 2.0);
		gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
		glLoadIdentity();
		draw();

		glViewport(screenWidth / 2.0, screenWidth / 2.0, screenWidth / 2.0, screenHeight / 2.0);
		glLoadIdentity();
		gluLookAt(1, 0.5, 0, 0, 0, 0, 0, 1, 0);
		draw();

		glViewport(screenWidth / 2.0, 0, screenWidth / 2.0, screenHeight / 2.0);
		glLoadIdentity();
		gluLookAt(lookAt_X, lookAt_Y, lookAt_Z, 0, 0, 0, 0, 1, 0);
		draw();
	}
}

void myDisplay()
{
	glMatrixMode(GL_MODELVIEW);
	//gluLookAt
	//vi tri camera,ddiem nam tren dth tu tam_camera ra ngoai,vecto chi huong tren camera
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);

	if (light2 == true) {  //on light 2rd

		glLightfv(GL_LIGHT1, GL_POSITION, light_position2);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse2);
		glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient2);
		glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular2);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT1);
	}
	else glDisable(GL_LIGHT1);

	_4view();

	glFlush();
	glutSwapBuffers();
}

void myInit()
{
	fHalfSize = 3.5;//0.7
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-fHalfSize, fHalfSize, -fHalfSize + 1.5, fHalfSize + 1.5, -1000, 1000);//-2,5
}

int _tmain(int argc, _TCHAR* argv[])
{
	glutInit(&argc, (char**)argv); //initialize the tool kit
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);//set the display mode
	glutInitWindowSize(screenWidth, screenHeight); //set window size
	glutInitWindowPosition(100, 100); // set window position on screen
	glutCreateWindow(""); // open the screen window
	base.CreateCylinder(0.1, rCircle - 0.1);
	base1.CreateCylinder(0.5, rCircle - 0.2);
	base2.CreateCylinder(0.5, rCircle - 0.3);
	base3.CreateCylinder(0.15, rCircle + 0.1);

	column.CreateCuboid(2, 1, 0.05);
	column.CalculateFacesNorm();

	column1.CreateCuboid(0.1, 0.6, 0.05);
	column1.CalculateFacesNorm();

	column2.CreateCuboid(0.1, 0.6, 0.05);
	column2.CalculateFacesNorm();

	column3.CreateCuboid(0.1, 0.14, 0.1);
	column3.CalculateFacesNorm();

	column4.CreateCuboid(0.1, 0.14, 0.1);
	column4.CalculateFacesNorm();

	tt_ov.CreateOvanHole(rCircle * 2 + 0.2, 0.2, R, r);
	tt_ov.CalculateFacesNorm();

	tt_ov1.CreateOvanHole(0.2, 0.1, R, r);
	tt_ov1.CalculateFacesNorm();

	tt_ov2.CreateOvanHole(0.2, 0.1, R, r);
	tt_ov2.CalculateFacesNorm();

	tq_tru.CreateCylinder(0.2, tq_ov_R);
	tq_tru.CalculateFacesNorm();

	tq_tru1.CreateCylinder(0.6, tq_ov_R);
	tq_tru1.CalculateFacesNorm();

	tq_tru2.CreateCylinder(0.6, tq_ov_R);
	tq_tru2.CalculateFacesNorm();

	tq_tru3.CreateCylinder(0.6, tq_ov_R);
	tq_tru3.CalculateFacesNorm();

	tq_tru4.CreateCylinder(0.6, tq_ov_R);
	tq_tru4.CalculateFacesNorm();

	tt1.CreateCuboid(columnSizeX, lenY, columnSizeZ); //17,16//mat mau do
	tt1.CalculateFacesNorm();
	tt1.SetColor(17);


	tt2.CreateCuboid(columnSizeX, lenY, columnSizeZ);
	tt2.SetColor(2);
	tt2.CalculateFacesNorm();

	gd2.CreateCuboid(columnSizeX, gd2_len, columnSizeZ);
	gd2.SetColor(7);
	gd2.CalculateFacesNorm();




	hh_dac2.CreateCuboid(columnSizeX, columnSizeX, columnSizeX);
	hh_dac2.SetColor(7);
	hh_dac2.CalculateFacesNorm();

	hh_rong2.CreateCuboidHole(1.5*columnSizeX, 1.5*columnSizeX, 1.5*columnSizeZ, 1.5);
	hh_rong2.SetColor(7);
	hh_rong2.CalculateFacesNorm();

	hh_dac1.CreateCuboid(columnSizeX, columnSizeX, 2 * columnSizeX);
	hh_dac1.SetColor(2);
	hh_dac1.CalculateFacesNorm();

	hh_rong1.CreateCuboidHole(1.5*columnSizeX, 1.5*columnSizeX, 1.5*columnSizeZ, 1.5);
	hh_rong1.SetColor(2);
	hh_rong1.CalculateFacesNorm();


	myInit();
	glutKeyboardFunc(myKeyboard);
	glutSpecialFunc(mySpecial);
	glutDisplayFunc(myDisplay);

	glutMainLoop();
	return 0;
}
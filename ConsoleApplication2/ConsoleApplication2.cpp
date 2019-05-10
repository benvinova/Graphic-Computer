//	TRINH MINH DUNG
//	MSSV : 1510580

#include "stdafx.h"
#include <windows.h>
#include <math.h>
#include <gl.h>
#include <glut.h>

#include <stdio.h>										
#include <stdlib.h>

#include <iostream>
using namespace std;

typedef	struct{
	GLubyte	* imageData;	
	GLuint	bpp;
	GLuint	width;
	GLuint	height;
	GLuint	texID;
	GLuint	type;
} Texture;

class Point3{
public:
	float x, y, z;
	void set(float dx, float dy, float dz){
		x = dx; y = dy; z = dz;
	}
	void set(Point3& p){
		x = p.x; y = p.y; z = p.z;
	}
	Point3() { x = y = z = 0; }
	Point3(float dx, float dy, float dz){
		x = dx; y = dy; z = dz;
	}

};

class Vector3{
public:
	float	x, y, z;
	void set(float dx, float dy, float dz){
		x = dx; y = dy; z = dz;
	}
	void set(Vector3& v){
		x = v.x; y = v.y; z = v.z;
	}
	void flip(){
		x = -x; y = -y; z = -z;
	}
	void normalize();
	Vector3() { x = y = z = 0; }
	Vector3(float dx, float dy, float dz){
		x = dx; y = dy; z = dz;
	}
	Vector3(Vector3& v){
		x = v.x; y = v.y; z = v.z;
	}
	Vector3 cross(Vector3 b);
	float dot(Vector3 b);
};
Vector3 Vector3::cross(Vector3 b){
	Vector3 c(y*b.z - z*b.y, z*b.x - x*b.z, x*b.y - y*b.x);
	return c;
}
float Vector3::dot(Vector3 b){
	return x*b.x + y*b.y + z*b.z;
}
void Vector3::normalize(){
	float temp = sqrt(x*x + y*y + z*z);
	x = x / temp;
	y = y / temp;
	z = z / temp;
}

class VertexID{
public:
	int		vertIndex;
	int		colorIndex;
};

class Face{
public:
	int	nVerts;
	VertexID*vert;
	Vector3	facenorm;

	Face(){
		nVerts = 0;
		vert = NULL;
	}
	~Face(){
		if (vert != NULL)
		{
			delete[] vert;
			vert = NULL;
		}
		nVerts = 0;
	}
};

class Mesh{
public:
	int		numVerts;
	Point3*		pt;

	int		numFaces;
	Face*		face;
public:
	Mesh(){
		numVerts = 0;
		pt = NULL;
		numFaces = 0;
		face = NULL;
	}
	~Mesh(){
		if (pt != NULL){
			delete[] pt;
		}
		if (face != NULL){
			delete[] face;
		}
		numVerts = 0;
		numFaces = 0;
	}
	void DrawWireframe();


	void CreateTetrahedron();
	void CreateCube(float	fSize);
	void Mesh::CreateCuboid(float	fSizeX, float fSizeY, float	fSizeZ);
	void Mesh::CreateCuboidHole(float	fSizeX, float fSizeY, float	fSizeZ, float n);
	void Mesh::CreateCylinder(float height, float R);
	void Mesh::CreateOvan(float len, float height, float R);
	void Mesh::CreateOvanHole(float len, float height, float R, float r);


	float	slideX, slideY, slideZ;
	float	rotateX, rotateY, rotateZ;
	float	scaleX, scaleY, scaleZ;
	void Mesh::SetColor(int colorIdx);
	void Mesh::Draw();
	void Mesh::CalculateFacesNorm();
	void Mesh::setupMaterial(float ambient[], float diffuse[], float specular[], float shiness);
};

//----------------- Mesh.cpp -----------------

#define PI			3.1415926

void Mesh::CreateCuboid(float	fSizeX, float fSizeY, float	fSizeZ){
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

	face[0].nVerts = 4;
	face[0].vert = new VertexID[face[0].nVerts];
	face[0].vert[0].vertIndex = 1;
	face[0].vert[1].vertIndex = 5;
	face[0].vert[2].vertIndex = 6;
	face[0].vert[3].vertIndex = 2;

	face[1].nVerts = 4;
	face[1].vert = new VertexID[face[1].nVerts];
	face[1].vert[0].vertIndex = 0;
	face[1].vert[1].vertIndex = 3;
	face[1].vert[2].vertIndex = 7;
	face[1].vert[3].vertIndex = 4;

	face[2].nVerts = 4;
	face[2].vert = new VertexID[face[2].nVerts];
	face[2].vert[0].vertIndex = 0;
	face[2].vert[1].vertIndex = 1;
	face[2].vert[2].vertIndex = 2;
	face[2].vert[3].vertIndex = 3;

	face[3].nVerts = 4;
	face[3].vert = new VertexID[face[3].nVerts];
	face[3].vert[0].vertIndex = 7;
	face[3].vert[1].vertIndex = 6;
	face[3].vert[2].vertIndex = 5;
	face[3].vert[3].vertIndex = 4;

	face[4].nVerts = 4;
	face[4].vert = new VertexID[face[4].nVerts];
	face[4].vert[0].vertIndex = 4;
	face[4].vert[1].vertIndex = 5;
	face[4].vert[2].vertIndex = 1;
	face[4].vert[3].vertIndex = 0;

	face[5].nVerts = 4;
	face[5].vert = new VertexID[face[5].nVerts];
	face[5].vert[0].vertIndex = 3;
	face[5].vert[1].vertIndex = 2;
	face[5].vert[2].vertIndex = 6;
	face[5].vert[3].vertIndex = 7;
}

void Mesh::CreateCuboidHole(float	fSizeX, float fSizeY, float	fSizeZ, float n){
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

	face[0].nVerts = 4;
	face[0].vert = new VertexID[face[0].nVerts];
	face[0].vert[0].vertIndex = 1;
	face[0].vert[1].vertIndex = 5;
	face[0].vert[2].vertIndex = 6;
	face[0].vert[3].vertIndex = 2;
	for (i = 0; i<face[0].nVerts; i++)
		face[0].vert[i].colorIndex = 0;

	face[1].nVerts = 4;
	face[1].vert = new VertexID[face[1].nVerts];
	face[1].vert[0].vertIndex = 0;
	face[1].vert[1].vertIndex = 3;
	face[1].vert[2].vertIndex = 7;
	face[1].vert[3].vertIndex = 4;
	for (i = 0; i<face[1].nVerts; i++)
		face[1].vert[i].colorIndex = 1;

	face[2].nVerts = 4;
	face[2].vert = new VertexID[face[2].nVerts];
	face[2].vert[0].vertIndex = 0;
	face[2].vert[1].vertIndex = 1;
	face[2].vert[2].vertIndex = 9;
	face[2].vert[3].vertIndex = 8;
	for (i = 0; i<face[2].nVerts; i++)
		face[2].vert[i].colorIndex = 2;

	face[6].nVerts = 4;
	face[6].vert = new VertexID[face[6].nVerts];
	face[6].vert[0].vertIndex = 1;
	face[6].vert[1].vertIndex = 2;
	face[6].vert[2].vertIndex = 10;
	face[6].vert[3].vertIndex = 9;
	for (i = 0; i<face[6].nVerts; i++)
		face[6].vert[i].colorIndex = 6;

	face[7].nVerts = 4;
	face[7].vert = new VertexID[face[7].nVerts];
	face[7].vert[0].vertIndex = 2;
	face[7].vert[1].vertIndex = 3;
	face[7].vert[2].vertIndex = 11;
	face[7].vert[3].vertIndex = 10;
	for (i = 0; i<face[7].nVerts; i++)
		face[7].vert[i].colorIndex = 7;

	face[8].nVerts = 4;
	face[8].vert = new VertexID[face[8].nVerts];
	face[8].vert[0].vertIndex = 3;
	face[8].vert[1].vertIndex = 11;
	face[8].vert[2].vertIndex = 8;
	face[8].vert[3].vertIndex = 0;
	for (i = 0; i<face[8].nVerts; i++)
		face[8].vert[i].colorIndex = 8;

	face[9].nVerts = 4;
	face[9].vert = new VertexID[face[9].nVerts];
	face[9].vert[0].vertIndex = 5;
	face[9].vert[1].vertIndex = 6;
	face[9].vert[2].vertIndex = 14;
	face[9].vert[3].vertIndex = 13;
	for (i = 0; i<face[9].nVerts; i++)
		face[9].vert[i].colorIndex = 9;

	face[10].nVerts = 4;
	face[10].vert = new VertexID[face[10].nVerts];
	face[10].vert[0].vertIndex = 6;
	face[10].vert[1].vertIndex = 7;
	face[10].vert[2].vertIndex = 15;
	face[10].vert[3].vertIndex = 14;
	for (i = 0; i<face[10].nVerts; i++)
		face[10].vert[i].colorIndex = 10;

	face[3].nVerts = 4;
	face[3].vert = new VertexID[face[3].nVerts];
	face[3].vert[0].vertIndex = 7;
	face[3].vert[1].vertIndex = 15;
	face[3].vert[2].vertIndex = 12;
	face[3].vert[3].vertIndex = 4;
	for (i = 0; i<face[3].nVerts; i++)
		face[3].vert[i].colorIndex = 3;

	face[11].nVerts = 4;
	face[11].vert = new VertexID[face[11].nVerts];
	face[11].vert[0].vertIndex = 5;
	face[11].vert[1].vertIndex = 13;
	face[11].vert[2].vertIndex = 12;
	face[11].vert[3].vertIndex = 4;
	for (i = 0; i<face[11].nVerts; i++)
		face[11].vert[i].colorIndex = 11;

	face[12].nVerts = 4;
	face[12].vert = new VertexID[face[12].nVerts];
	face[12].vert[0].vertIndex = 9;
	face[12].vert[1].vertIndex = 10;
	face[12].vert[2].vertIndex = 14;
	face[12].vert[3].vertIndex = 13;
	for (i = 0; i<face[12].nVerts; i++)
		face[12].vert[i].colorIndex = 13;

	face[13].nVerts = 4;
	face[13].vert = new VertexID[face[13].nVerts];
	face[13].vert[0].vertIndex = 10;
	face[13].vert[1].vertIndex = 11;
	face[13].vert[2].vertIndex = 15;
	face[13].vert[3].vertIndex = 14;
	for (i = 0; i<face[13].nVerts; i++)
		face[13].vert[i].colorIndex = 12;

	face[14].nVerts = 4;
	face[14].vert = new VertexID[face[14].nVerts];
	face[14].vert[0].vertIndex = 11;
	face[14].vert[1].vertIndex = 8;
	face[14].vert[2].vertIndex = 12;
	face[14].vert[3].vertIndex = 15;
	for (i = 0; i<face[14].nVerts; i++)
		face[14].vert[i].colorIndex = 19;

	face[15].nVerts = 4;
	face[15].vert = new VertexID[face[15].nVerts];
	face[15].vert[0].vertIndex = 9;
	face[15].vert[1].vertIndex = 8;
	face[15].vert[2].vertIndex = 12;
	face[15].vert[3].vertIndex = 13;
	for (i = 0; i<face[15].nVerts; i++)
		face[15].vert[i].colorIndex = 20;

	face[4].nVerts = 4;
	face[4].vert = new VertexID[face[4].nVerts];
	face[4].vert[0].vertIndex = 4;
	face[4].vert[1].vertIndex = 5;
	face[4].vert[2].vertIndex = 1;
	face[4].vert[3].vertIndex = 0;
	for (i = 0; i<face[4].nVerts; i++)
		face[4].vert[i].colorIndex = 4;

	face[5].nVerts = 4;
	face[5].vert = new VertexID[face[5].nVerts];
	face[5].vert[0].vertIndex = 3;
	face[5].vert[1].vertIndex = 2;
	face[5].vert[2].vertIndex = 6;
	face[5].vert[3].vertIndex = 7;
	for (i = 0; i<face[5].nVerts; i++)
		face[5].vert[i].colorIndex = 5;
}

void Mesh::CreateCylinder(float height, float R){
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
	for (int i = 0; i<20; i = i + 1){
		face[i].nVerts = 3;
		face[i].vert = new VertexID[face[i].nVerts];
		face[i].vert[0].vertIndex = 40;
		face[i].vert[1].vertIndex = i;
		if (i != 19) face[i].vert[2].vertIndex = i + 1;
		else face[i].vert[2].vertIndex = 0;
	}
	for (int i = 20; i<40; i = i + 1){
		face[i].nVerts = 3;
		face[i].vert = new VertexID[face[i].nVerts];
		face[i].vert[0].vertIndex = 41;
		face[i].vert[1].vertIndex = i;
		if (i != 39) face[i].vert[2].vertIndex = i + 1;
		else face[i].vert[2].vertIndex = 20;
	}
	for (int i = 40; i<60; i = i + 1){
		face[i].nVerts = 4;
		face[i].vert = new VertexID[face[i].nVerts];
		face[i].vert[0].vertIndex = i - 40;
		face[i].vert[1].vertIndex = i + 20 - 40;
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
	pt[80].set(len / 2, height / 2, 0);
	pt[81].set(len / 2, -height / 2, 0); 

	j = 90;
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
	face[i].vert[0].vertIndex = 40;
	face[i].vert[1].vertIndex = 60;
	face[i].vert[2].vertIndex = 39;
	face[i].vert[3].vertIndex = 19;
	for (int j = 0; j<face[i].nVerts; j++)
		face[i].vert[j].colorIndex = i;

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
	pt[80].set(len / 2, height / 2, 0); 
	pt[81].set(len / 2, -height / 2, 0);

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
	j = 90;
	for (int i = 40; i<60; i = i + 1){
		pt[i].set(R*cos(j*PI / 180) - len / 2, height / 2, R*sin(j*PI / 180));
		j = j + 9;
	}
	j = 90;
	for (int i = 60; i<80; i = i + 1){
		pt[i].set(R*cos(j*PI / 180) - len / 2, -height / 2, R*sin(j*PI / 180));
		j = j + 9;
	}
	j = 90;
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


	i = 22;
	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];
	face[i].vert[0].vertIndex = 40;
	face[i].vert[1].vertIndex = 60;
	face[i].vert[2].vertIndex = 39;
	face[i].vert[3].vertIndex = 19;
	for (int j = 0; j<face[i].nVerts; j++)
		face[i].vert[j].colorIndex = i;
	i = 23;
	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];
	face[i].vert[0].vertIndex = 140;
	face[i].vert[1].vertIndex = 160;
	face[i].vert[2].vertIndex = 139;
	face[i].vert[3].vertIndex = 119;
	for (int j = 0; j<face[i].nVerts; j++)
		face[i].vert[j].colorIndex = i;

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

void Mesh::CreateCube(float	fSize){
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

	face[0].nVerts = 4;
	face[0].vert = new VertexID[face[0].nVerts];
	face[0].vert[0].vertIndex = 1;
	face[0].vert[1].vertIndex = 5;
	face[0].vert[2].vertIndex = 6;
	face[0].vert[3].vertIndex = 2;
	for (i = 0; i<face[0].nVerts; i++)
		face[0].vert[i].colorIndex = 0;

	face[1].nVerts = 4;
	face[1].vert = new VertexID[face[1].nVerts];
	face[1].vert[0].vertIndex = 0;
	face[1].vert[1].vertIndex = 3;
	face[1].vert[2].vertIndex = 7;
	face[1].vert[3].vertIndex = 4;
	for (i = 0; i<face[1].nVerts; i++)
		face[1].vert[i].colorIndex = 1;

	face[2].nVerts = 4;
	face[2].vert = new VertexID[face[2].nVerts];
	face[2].vert[0].vertIndex = 0;
	face[2].vert[1].vertIndex = 1;
	face[2].vert[2].vertIndex = 2;
	face[2].vert[3].vertIndex = 3;
	for (i = 0; i<face[2].nVerts; i++)
		face[2].vert[i].colorIndex = 2;

	face[3].nVerts = 4;
	face[3].vert = new VertexID[face[3].nVerts];
	face[3].vert[0].vertIndex = 7;
	face[3].vert[1].vertIndex = 6;
	face[3].vert[2].vertIndex = 5;
	face[3].vert[3].vertIndex = 4;
	for (i = 0; i<face[3].nVerts; i++)
		face[3].vert[i].colorIndex = 3;

	face[4].nVerts = 4;
	face[4].vert = new VertexID[face[4].nVerts];
	face[4].vert[0].vertIndex = 4;
	face[4].vert[1].vertIndex = 5;
	face[4].vert[2].vertIndex = 1;
	face[4].vert[3].vertIndex = 0;
	for (i = 0; i<face[4].nVerts; i++)
		face[4].vert[i].colorIndex = 4;

	face[5].nVerts = 4;
	face[5].vert = new VertexID[face[5].nVerts];
	face[5].vert[0].vertIndex = 3;
	face[5].vert[1].vertIndex = 2;
	face[5].vert[2].vertIndex = 6;
	face[5].vert[3].vertIndex = 7;
	for (i = 0; i<face[5].nVerts; i++)
		face[5].vert[i].colorIndex = 5;

}

void Mesh::CreateTetrahedron(){
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
void Mesh::DrawWireframe(){
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for (int f = 0; f < numFaces; f++){
		glBegin(GL_POLYGON);
		for (int v = 0; v < face[f].nVerts; v++){
			int	iv = face[f].vert[v].vertIndex;
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
void Mesh::setupMaterial(float ambient[], float diffuse[], float specular[], float shiness){
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shiness);
}

// ----------------- End Mesh -----------------


int		screenWidth = 600;
int		screenHeight = 600;

bool	bWireFrame = false;
bool	animation = false;

float	rCircle = 0.5;
float	baseHeight = 0.3;
float	baseRotateStep = 5;

float	columnSizeX = 0.1;
float	columnSizeZ = columnSizeX;
float	columnSizeY = 2;

float R = (20 / 13.0)*columnSizeX; 
float lenY = 0.2*columnSizeY; 
float r = (10 / 13.0)*columnSizeX;
float gd2_len = 0.7*columnSizeY;

float tayquay_ov_len = 0.3*columnSizeY;
float tayquay_ov_height = 2 * columnSizeZ;
float tayquay_ov_R = r;

float	camera_angle = 0.01;
float	camera_height = 2;
float	camera_dis = 6;
float	camera_X, camera_Y, camera_Z;
float	lookAt_X = 0, lookAt_Y = camera_height, lookAt_Z = camera_dis;

float	fHalfSize;
bool	b4View = false; 
bool	light2 = true;

float floorShadow[4][4];	
float floorShadow2[4][4];

float groundplane[] = { 0.0f, 1.0f, 0.0f, 0.0f };

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
Mesh	tayquay_ovan;
Mesh	tayquay_ovan1;
Mesh	tayquay_ovan2;
Mesh	tayquay_tru1;
Mesh	tayquay_tru2;
Mesh	tayquay_tru3;
Mesh	tayquay_tru4;
Mesh	tayquay_ov;

Texture   floorTex;


void Timer(int value){
	if (animation){
		base.rotateY += 2;
		base1.rotateY += 2;
		base2.rotateY += 2;
		tayquay_ov.rotateZ += 2;
		if (tayquay_ov.rotateZ > 360)
			tayquay_ov.rotateZ -= 360;
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
void myKeyboard(unsigned char key, int x, int y){
	switch (key){
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
		tayquay_ov.rotateZ += baseRotateStep;
		if (tayquay_ov.rotateZ > 360)
			tayquay_ov.rotateZ -= 360;
		break;
	case '4':
		tayquay_ov.rotateZ -= baseRotateStep;
		if (tayquay_ov.rotateZ < 0)
			tayquay_ov.rotateZ += 360;
		break;
	case 'R':
	case 'r':
		base.rotateY = 0;
		base1.rotateY = 0;
		base2.rotateY = 0;
		tayquay_ov.rotateZ = 0;
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
	switch (key){
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
void drawAxis(){
	glColor3f(0, 2, 1);
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
void drawBase(){
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
void drawBase1(){
	glPushMatrix();
	base1.CalculateFacesNorm();
	glTranslated(0, 0.1, 0);
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
void drawBase2(){
	glPushMatrix();
	base2.CalculateFacesNorm();
	glTranslated(0, 0.6, 0); 
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
void drawBase3(){
	glPushMatrix();
	base3.CalculateFacesNorm();
	glTranslated(0, 0.95, 0);
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
void draw_face_table(){
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
void draw_face_table1(){
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
void draw_face_table2(){
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
void draw_face_table3(){
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
void draw_face_table4(){
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


// Thanh ngang trụ
void draw_tayquay_tru1(){
	glPushMatrix();
	glTranslatef(-1, 1.12, 0);
	glRotatef(90, 0, 0, 1);
	GLfloat amb[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat diff[] = { 0.67f, 0.66f, 0.66f, 1.0f };
	GLfloat spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat shine = 100.0f;

	tayquay_tru1.setupMaterial(amb, diff, spec, shine);
	if (bWireFrame)
		tayquay_tru1.DrawWireframe();
	else
		tayquay_tru1.Draw();
	glPopMatrix();
}
void draw_tayquay_tru2(){
	glPushMatrix();
	glTranslatef(1, 1.12, 0);
	glRotatef(90, 0, 0, 1);
	GLfloat amb[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat diff[] = { 0.67f, 0.66f, 0.66f, 1.0f };
	GLfloat spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat shine = 100.0f;

	tayquay_tru2.setupMaterial(amb, diff, spec, shine);
	if (bWireFrame)
		tayquay_tru2.DrawWireframe();
	else
		tayquay_tru2.Draw();
	glPopMatrix();
}
void draw_tayquay_tru3(){
	glPushMatrix();
	glTranslatef(-0.3, 1.12, 0);
	glRotatef(90, 0, 0, 1);
	GLfloat amb[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat diff[] = { 0.67f, 0.66f, 0.66f, 1.0f };
	GLfloat spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat shine = 100.0f;

	tayquay_tru3.setupMaterial(amb, diff, spec, shine);
	if (bWireFrame)
		tayquay_tru3.DrawWireframe();
	else
		tayquay_tru3.Draw();
	glPopMatrix();
}
void draw_tayquay_tru4(){
	glPushMatrix();
	glTranslatef(0.3, 1.12, 0);
	glRotatef(90, 0, 0, 1);
	GLfloat amb[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat diff[] = { 0.67f, 0.66f, 0.66f, 1.0f };
	GLfloat spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat shine = 100.0f;

	tayquay_tru4.setupMaterial(amb, diff, spec, shine);
	if (bWireFrame)
		tayquay_tru4.DrawWireframe();
	else
		tayquay_tru4.Draw();
	glPopMatrix();
}


void drawtayquay_ovan(){
	glPushMatrix();
	glTranslatef(0, 1.12, 0);
	glRotatef(90, 0, 1, 0);
	GLfloat amb[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	GLfloat diff[] = { 0.25f, 0.25f, 0.25f, 1.0f };
	GLfloat spec[] = { 0.9f, 1.0f, 1.0f, 1.0f };
	GLfloat shine = 30.0f;

	tayquay_ovan.setupMaterial(amb, diff, spec, shine);
	if (bWireFrame)
		tayquay_ovan.DrawWireframe();
	else
		tayquay_ovan.Draw();

	glPopMatrix();
}
void drawtayquay_ovan1(){
	glPushMatrix();
	glTranslatef(-1, 1, 0);
	glRotatef(90, 0, 0, 1);
	GLfloat amb[] = { 1.0f, 0.9f, 1, 1.0f };
	GLfloat diff[] = { 0.25f, 0.25f, 0.25f, 1.0f };
	GLfloat spec[] = { 0.9f, 1.0f, 1.0f, 1.0f };
	GLfloat shine = 30.0f;

	tayquay_ovan1.setupMaterial(amb, diff, spec, shine);
	if (bWireFrame)
		tayquay_ovan1.DrawWireframe();
	else
		tayquay_ovan1.Draw();

	glPopMatrix();
}
void drawtayquay_ovan2(){
	glPushMatrix();
	glTranslatef(1, 1, 0);
	glRotatef(90, 0, 0, 1);
	GLfloat amb[] = { 1.0f, 0.9f, 1, 1.0f };
	GLfloat diff[] = { 0.25f, 0.25f, 0.25f, 1.0f };
	GLfloat spec[] = { 0.9f, 1.0f, 1.0f, 1.0f };
	GLfloat shine = 30.0f;

	tayquay_ovan2.setupMaterial(amb, diff, spec, shine);
	if (bWireFrame)
		tayquay_ovan2.DrawWireframe();
	else
		tayquay_ovan2.Draw();

	glPopMatrix();
}
void draw(){
	drawBase();
	drawBase1();
	drawBase2();
	drawBase3();
	draw_face_table();
	draw_face_table1();
	draw_face_table2();
	draw_face_table3();
	draw_face_table4();
	drawtayquay_ovan();
	drawtayquay_ovan1();
	drawtayquay_ovan2();
	draw_tayquay_tru1();
	draw_tayquay_tru2();
	draw_tayquay_tru3();
	draw_tayquay_tru4();
}
void DrawFloor(){
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, floorTex.texID);
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


void drawReflect(){
	glColorMask(0, 0, 0, 0);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glDisable(GL_DEPTH_TEST);
	DrawFloor();
	glColorMask(1, 1, 1, 1);
	glStencilFunc(GL_EQUAL, 1, 1);
	glEnable(GL_DEPTH_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glPushMatrix();
	glScalef(1, -1, 1);
	draw();
	glPopMatrix();
	glDisable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glColor4f(1.0f, 2.0f, 1.0f, 0.7f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	DrawFloor();
	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);

}

void _4view(){
	if (!b4View){
		glViewport(0, 0, screenWidth, screenHeight);
		glLoadIdentity();
		gluLookAt(lookAt_X, lookAt_Y, lookAt_Z, 0, 0, 0, 0, 1, 0);

		drawReflect();
		draw();
	}
	else {
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

void myDisplay(){
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);

	if (light2 == true) {

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

void myInit(){
	fHalfSize = 3.5;
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-fHalfSize, fHalfSize, -fHalfSize + 1.5, fHalfSize + 1.5, -1000, 1000);
}

int _tmain(int argc, _TCHAR* argv[]){
	glutInit(&argc, (char**)argv); 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(100, 100); 
	glutCreateWindow("Trinh Minh Dung - 1510580");

	cout << "1, 2: Rotate the base" << endl;
	cout << "3, 4: Rotate the arm" << endl;
	cout << "R, r: Reset" << endl;
	cout << "W, w: Switch between wireframe and solid mode" << endl;
	cout << "V, v: Switch between 1 and 4 views" << endl;
	cout << "A, a: Turn on/off animation" << endl;
	cout << "-   : to increase camera distance" << endl;
	cout << "+  : to decrease camera distance" << endl;
	cout << "up arrow  : to increase camera height" << endl;
	cout << "down arrow: to decrease camera height" << endl;
	cout << "<-        : to rotate camera clockwise" << endl;
	cout << "->        : to rotate camera counterclockwise" << endl;
	base.CreateCylinder(0.3, rCircle - 0.2);
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

	tayquay_ovan.CreateOvanHole(rCircle * 2 + 0.2, 0.2, R, r);
	tayquay_ovan.CalculateFacesNorm();

	tayquay_ovan1.CreateOvanHole(0.2, 0.1, R, r);
	tayquay_ovan1.CalculateFacesNorm();

	tayquay_ovan2.CreateOvanHole(0.2, 0.1, R, r);
	tayquay_ovan2.CalculateFacesNorm();

	tayquay_tru1.CreateCylinder(1.4, tayquay_ov_R);
	tayquay_tru1.CalculateFacesNorm();

	tayquay_tru2.CreateCylinder(1.4, tayquay_ov_R);
	tayquay_tru2.CalculateFacesNorm();

	tayquay_tru3.CreateCylinder(0.3, tayquay_ov_R * 1.5);
	tayquay_tru3.CalculateFacesNorm();

	tayquay_tru4.CreateCylinder(0.3, tayquay_ov_R * 1.5);
	tayquay_tru4.CalculateFacesNorm();

	myInit();
	glutKeyboardFunc(myKeyboard);
	glutSpecialFunc(mySpecial);
	glutDisplayFunc(myDisplay);

	glutMainLoop();
	return 0;
}
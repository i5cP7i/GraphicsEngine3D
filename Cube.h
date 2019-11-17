#pragma once
#include "Polygon3D.h"
class Cube :
	public Polygon3D
{
public:
	Cube();
	~Cube();
public:
	void demoCube();
protected:
	void CreateMesh();
	virtual void setProjectionMatrix(mat4x4 &matrix, float fZn, float fZf, float fFOV, float fAspectRatio, float fFOVRad);
private:
	mesh meshCube;
	mat4x4 matProj, matRotZ, matRotX;
	float fTheta;

	float fZn;
	float fZf;
	float fFOV;
	float fAspectRatio;
	float fFOVRad;
};


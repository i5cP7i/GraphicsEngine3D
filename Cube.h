#pragma once
#include "Polygon3D.h"

class Instance;
class Cube :
	public Polygon3D
{
public:
	Cube();
public:
    void demoCube(Instance*);
    void setProjectionMatrix(Instance *i, float fZn, float fZf, float fFOV);
protected:
    void CreateMesh() override;
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


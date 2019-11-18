#pragma once
#include "Polygon3D.h"

class Instance;
class Cube :
    public Polygon3D
{
public:
    Cube(float fSideLength, float fOriginX, float fOriginY, float fOriginZ);
    virtual ~Cube();
public:
    void demoCube(Instance*);
    void setProjectionMatrix(Instance *i, float fZn, float fZf, float fFOV);
    void setDrawingMode();
protected:
    void CreateMesh() override;
private:
    mesh meshCube;
    mat4x4 matProj, matRotZ, matRotX;
    vector tempVec1;
    vector tempVec2;
    vector normal;
    vector vCamera;

    float fTheta;
    float fZn;
    float fZf;
    float fFOV;
    float fAspectRatio;
    float fFOVRad;

    float fSideLength;
    float fOriginX;
    float fOriginY;
    float fOriginZ;
};

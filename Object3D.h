#pragma once
#include "Polygon3D.h"
#include <fstream>
#include <strstream>

class Instance;

class Object3D :
    public Polygon3D
{
public:
    Object3D(std::string ifName);
    virtual ~Object3D();
public:
    void setProjectionMatrix(Instance *i, float fZn, float fZf, float fFOV);
    void demoCustomObject(Instance*);
protected:
   virtual void CreateMesh();
private:
    mesh meshObject;
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

    bool LoadObjectFile(std::string ifName);
    std::vector<vertex> vertices;
    std::string ifName;
};


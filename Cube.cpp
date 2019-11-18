#include "Cube.h"

Cube::Cube(float fSideLength, float fOriginX, float fOriginY, float fOriginZ)
{
    this->fSideLength = fSideLength;
    this->fOriginX = fOriginX;
    this->fOriginY = fOriginY;
    this->fOriginZ = fOriginZ;
    CreateMesh();
}

Cube::~Cube()
{

}

void Cube::demoCube(Instance* i)
{
    fTheta += 1.0f * i->fElapsedTime;
    //Rotation z
    matRotZ.m[0][0] = cosf(fTheta);
    matRotZ.m[0][1] = sinf(fTheta);
    matRotZ.m[1][0] = -sinf(fTheta);
    matRotZ.m[1][1] = cosf(fTheta);
    matRotZ.m[2][2] = 1.0f;
    matRotZ.m[3][3] = 1.0f;

    //Rotation x
    matRotX.m[0][0] = 1.0f;
    matRotX.m[1][1] = cosf(fTheta * 0.5f);
    matRotX.m[1][2] = sinf(fTheta * 0.5f);
    matRotX.m[2][1] = -sinf(fTheta * 0.5f);
    matRotX.m[2][2] = cosf(fTheta * 0.5f);
    matRotX.m[3][3] = 1.0f;

    //Draw triangles
    for (auto tri : meshCube.tri)
    {
        triangle triProjected, triTrans, triRotZ, triRotZX;

        //Rotate z
        MatrixMultiplier(tri.p[0], triRotZ.p[0], matRotZ);
        MatrixMultiplier(tri.p[1], triRotZ.p[1], matRotZ);
        MatrixMultiplier(tri.p[2], triRotZ.p[2], matRotZ);

        //Rotate x
        MatrixMultiplier(triRotZ.p[0], triRotZX.p[0], matRotX);
        MatrixMultiplier(triRotZ.p[1], triRotZX.p[1], matRotX);
        MatrixMultiplier(triRotZ.p[2], triRotZX.p[2], matRotX);

        //Translate tris
        triTrans = triRotZX;
        triTrans.p[0].z = triRotZX.p[0].z + 3.0f;
        triTrans.p[1].z = triRotZX.p[1].z + 3.0f;
        triTrans.p[2].z = triRotZX.p[2].z + 3.0f;

        tempVec1.p[0].x = triTrans.p[0].x;
        tempVec1.p[1].x = triTrans.p[1].x;
        tempVec1.p[0].y = triTrans.p[0].y;
        tempVec1.p[1].y = triTrans.p[1].y;
        tempVec1.p[0].z = triTrans.p[0].z;
        tempVec1.p[1].z = triTrans.p[1].z;

        tempVec1.xi = tempVec1.p[1].x - tempVec1.p[0].x;
        tempVec1.yj = tempVec1.p[1].y - tempVec1.p[0].y;
        tempVec1.zk = tempVec1.p[1].z - tempVec1.p[0].z;

        tempVec2.p[0].x = triTrans.p[0].x;
        tempVec2.p[1].x = triTrans.p[2].x;
        tempVec2.p[0].y = triTrans.p[0].y;
        tempVec2.p[1].y = triTrans.p[2].y;
        tempVec2.p[0].z = triTrans.p[0].z;
        tempVec2.p[1].z = triTrans.p[2].z;

        tempVec2.xi = tempVec2.p[1].x - tempVec2.p[0].x;
        tempVec2.yj = tempVec2.p[1].y - tempVec2.p[0].y;
        tempVec2.zk = tempVec2.p[1].z - tempVec2.p[0].z;

        normal = getNormalVector(tempVec1, tempVec2);
        normal.xi /= getVectorMagnitude(normal);
        normal.yj /= getVectorMagnitude(normal);
        normal.zk /= getVectorMagnitude(normal);



        if (fCalcDotProduct(normal.xi, normal.yj, normal.zk,
                            triTrans.p[0].x - vCamera.xi,
                            triTrans.p[0].y - vCamera.yj,
                            triTrans.p[0].z - vCamera.zk) < 0.0f)
        {
            //Projection Matrix
            MatrixMultiplier(triTrans.p[0], triProjected.p[0], matProj);
            MatrixMultiplier(triTrans.p[1], triProjected.p[1], matProj);
            MatrixMultiplier(triTrans.p[2], triProjected.p[2], matProj);

            //scale
            triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f;
            triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f;
            triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f;

            triProjected.p[0].x *= 0.5f * static_cast<float>(i->getConsoleWindowWidth());
            triProjected.p[0].y *= 0.5f * static_cast<float>(i->getConsoleWindowHeight());
            triProjected.p[1].x *= 0.5f * static_cast<float>(i->getConsoleWindowWidth());
            triProjected.p[1].y *= 0.5f * static_cast<float>(i->getConsoleWindowHeight());
            triProjected.p[2].x *= 0.5f * static_cast<float>(i->getConsoleWindowWidth());
            triProjected.p[2].y *= 0.5f * static_cast<float>(i->getConsoleWindowHeight());

            i->drawTriangle(triProjected.p[0].x, triProjected.p[0].y,
                triProjected.p[1].x, triProjected.p[1].y,
                triProjected.p[2].x, triProjected.p[2].y,
                i->PIXEL_SOLID, i->FG_WHITE);
        }
    }
}

void Cube::CreateMesh()
{
    meshCube.tri = {

        // FRONT
        { 0.0f-fOriginX, 0.0f-fOriginY,0.0f-fOriginZ,   0.0f-fOriginX, fSideLength, 0.0f-fOriginZ,    fSideLength, fSideLength, 0.0f-fOriginZ },
        { 0.0f-fOriginX, 0.0f-fOriginY, 0.0f-fOriginZ,    fSideLength, fSideLength, 0.0f-fOriginZ,    fSideLength, 0.0f-fOriginY, 0.0f-fOriginZ },

        // LEFT
        { fSideLength, 0.0f-fOriginY, 0.0f-fOriginZ,   fSideLength, fSideLength, 0.0f-fOriginZ,    fSideLength, fSideLength, fSideLength },
        { fSideLength, 0.0f-fOriginY, 0.0f-fOriginZ,    fSideLength, fSideLength, fSideLength,    fSideLength, 0.0f-fOriginY, fSideLength },

        // BACK
        { fSideLength, 0.0f-fOriginY, fSideLength,    fSideLength, fSideLength, fSideLength,    0.0f-fOriginX, fSideLength, fSideLength },
        { fSideLength, 0.0f-fOriginY, fSideLength,    0.0f-fOriginX, fSideLength, fSideLength,    0.0f-fOriginX, 0.0f-fOriginY, fSideLength },

        // RIGHT
        { 0.0f-fOriginX, 0.0f-fOriginY, fSideLength,    0.0f-fOriginX, fSideLength, fSideLength,    0.0f-fOriginX, fSideLength, 0.0f-fOriginZ },
        { 0.0f-fOriginX, 0.0f-fOriginY, fSideLength,    0.0f-fOriginX, fSideLength, 0.0f-fOriginZ,    0.0f-fOriginX, 0.0f-fOriginY, 0.0f-fOriginZ },

        // TOP
        { 0.0f-fOriginX, fSideLength, 0.0f-fOriginZ,    0.0f-fOriginX, fSideLength, fSideLength,    fSideLength, fSideLength, fSideLength },
        { 0.0f-fOriginX, fSideLength, 0.0f-fOriginZ,    fSideLength, fSideLength, fSideLength,    fSideLength, fSideLength, 0.0f-fOriginZ },

        // BOTTOM
        { fSideLength, 0.0f-fOriginY, fSideLength,    0.0f-fOriginX, 0.0f-fOriginY, fSideLength,   0.0f-fOriginX, 0.0f-fOriginY, 0.0f-fOriginZ },
        { fSideLength, 0.0f-fOriginY, fSideLength,   0.0f-fOriginX, 0.0f-fOriginY, 0.0f-fOriginZ,    fSideLength, 0.0f-fOriginY, 0.0f-fOriginZ }

    };
}
void Cube::setProjectionMatrix(Instance *i, float fZn, float fZf, float fFOV)
{
    this->fAspectRatio = static_cast<float>(i->getConsoleWindowHeight()) / static_cast<float>(i->getConsoleWindowWidth());
    this->fFOVRad = 1.0f / tanf(fFOV * 0.5f / 180.0f * 3.14159f);

    matProj.m[0][0] = fAspectRatio * fFOVRad;
    matProj.m[1][1] = fFOVRad;
    matProj.m[2][2] = 1.0f / (1.0f - fZn / fZf);
    matProj.m[2][3] = 1.0f;
    matProj.m[3][2] = -(fZn) / (1.0f - fZn / fZf);
    matProj.m[3][3] = 0.0f;
}

#include "Cube.h"
#include "math.h"

Cube::Cube()
{
}


Cube::~Cube()
{

}

void Cube::demoCube()
{
    //fTheta += 1.0f * getInstance()->fElapsedTime;
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

        //Projection Matrix
        MatrixMultiplier(triTrans.p[0], triProjected.p[0], matProj);
        MatrixMultiplier(triTrans.p[1], triProjected.p[1], matProj);
        MatrixMultiplier(triTrans.p[2], triProjected.p[2], matProj);


        //scale
        triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f;
        triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f;
        triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f;

        //triProjected.p[0].x *= 0.5f * static_cast<float>(getInstance()->getConsoleWindowWidth());
        //triProjected.p[0].y *= 0.5f * static_cast<float>(getInstance()->getConsoleWindowHeight());
        //triProjected.p[1].x *= 0.5f * static_cast<float>(getInstance()->getConsoleWindowWidth());
        //triProjected.p[1].y *= 0.5f * static_cast<float>(getInstance()->getConsoleWindowHeight());
        //triProjected.p[2].x *= 0.5f * static_cast<float>(getInstance()->getConsoleWindowWidth());
        //triProjected.p[2].y *= 0.5f * static_cast<float>(getInstance()->getConsoleWindowHeight());
        /*
        getInstance()->drawTriangle(triProjected.p[0].x, triProjected.p[0].y,
            triProjected.p[1].x, triProjected.p[1].y,
            triProjected.p[2].x, triProjected.p[2].y,
            getInstance()->PIXEL_SOLID, getInstance()->FG_WHITE);
        */
    }
}

void Cube::CreateMesh()
{
    meshCube.tri = {

        // SOUTH
        { 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

        // EAST
        { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
        { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

        // NORTH
        { 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
        { 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

        // WEST
        { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

        // TOP
        { 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
        { 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

        // BOTTOM
        { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
        { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f }

    };
}
void Cube::setProjectionMatrix(mat4x4 &matrix, float fZn, float fZf, float fFOV, float fAspectRatio, float fFOVRad)
{
    //this->fAspectRatio = static_cast<float>(getInstance()->getConsoleWindowHeight()) / static_cast<float>(getInstance()->getConsoleWindowWidth());
    this->fFOVRad = 1.0f / tanf(fFOV * 0.5f / 180.0f * 3.14159f);

    matrix.m[0][0] = fAspectRatio * fFOVRad;
    matrix.m[1][1] = fFOVRad;
    matrix.m[2][2] = 1.0f / (1.0f - fZn / fZf);
    matrix.m[2][3] = 1.0f;
    matrix.m[3][2] = -(fZn) / (1.0f - fZn / fZf);
    matrix.m[3][3] = 0.0f;
}

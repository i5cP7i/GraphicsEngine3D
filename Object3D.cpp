#include "Object3D.h"
#include <algorithm>

gre3d::Object3D::Object3D(std::string ifName)
{
    this->ifName = ifName;
    CreateMesh();
}

gre3d::Object3D::~Object3D()
{

}

void gre3d::Object3D::setProjectionMatrix(gre3d::Instance *i, float fZn, float fZf, float fFOV)
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

void gre3d::Object3D::demoCustomObject(gre3d::Instance *i)
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

    std::vector<triangle> storeTrisToRaster;

    //Draw triangles
    for (auto tri : meshObject.tri)
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
        float normalMagnitude = getVectorMagnitude(normal);
        normal.xi /= normalMagnitude;
        normal.yj /= normalMagnitude;
        normal.zk /= normalMagnitude;

        if (fCalcDotProduct(normal.xi, normal.yj, normal.zk,
                            triTrans.p[0].x - vCamera.xi,
                            triTrans.p[0].y - vCamera.yj,
                            triTrans.p[0].z - vCamera.zk)  < 0.0f)
        {
            //Single direction lighting
            vector lightDirection;
            lightDirection.p[0].x = 0.0f;
            lightDirection.p[0].y = 0.0f;
            lightDirection.p[0].z = 0.0f;
            lightDirection.p[1].x = 0.0f;
            lightDirection.p[1].y = 0.0f;
            lightDirection.p[1].z = -1.0f;
            lightDirection.xi = 0.0f;
            lightDirection.yj = 0.0f;
            lightDirection.zk = -1.0f;
            float l = getVectorMagnitude(lightDirection);
            lightDirection.xi /= l;
            lightDirection.yj /= l;
            lightDirection.zk /= l;

            float normalizationLightDirection = fCalcDotProduct(normal, lightDirection);

            CHAR_INFO c = i->getColorFromLux(normalizationLightDirection);
            triTrans.cl = c.Attributes;
            triTrans.pxt = c.Char.UnicodeChar;

            //Projection Matrix
            MatrixMultiplier(triTrans.p[0], triProjected.p[0], matProj);
            MatrixMultiplier(triTrans.p[1], triProjected.p[1], matProj);
            MatrixMultiplier(triTrans.p[2], triProjected.p[2], matProj);
            triProjected.cl = triTrans.cl;
            triProjected.pxt = triTrans.pxt;

            // Scale
            triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f;
            triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f;
            triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f;

            triProjected.p[0].x *= 0.5f * static_cast<float>(i->getConsoleWindowWidth());
            triProjected.p[0].y *= 0.5f * static_cast<float>(i->getConsoleWindowHeight());
            triProjected.p[1].x *= 0.5f * static_cast<float>(i->getConsoleWindowWidth());
            triProjected.p[1].y *= 0.5f * static_cast<float>(i->getConsoleWindowHeight());
            triProjected.p[2].x *= 0.5f * static_cast<float>(i->getConsoleWindowWidth());
            triProjected.p[2].y *= 0.5f * static_cast<float>(i->getConsoleWindowHeight());

            // Store
            storeTrisToRaster.push_back(triProjected);
        }
    }
    // Sort
    std::sort(storeTrisToRaster.begin(), storeTrisToRaster.end(), [](triangle &t1, triangle &t2)
    {
        float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
        float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
        return z1 > z2;
    });
    for (auto &triProjected : storeTrisToRaster)
    {
        i->FillTriangle(i, triProjected.p[0].x, triProjected.p[0].y,
            triProjected.p[1].x, triProjected.p[1].y,
            triProjected.p[2].x, triProjected.p[2].y,
            static_cast<GraphicsEngine3D::PIXEL_TYPE>(triProjected.pxt), static_cast<GraphicsEngine3D::COLOR>(triProjected.cl));

        i->drawTriangle(triProjected.p[0].x, triProjected.p[0].y,
            triProjected.p[1].x, triProjected.p[1].y,
            triProjected.p[2].x, triProjected.p[2].y,
            i->PIXEL_SOLID, i->FG_BLACK);
    }
}

void gre3d::Object3D::CreateMesh()
{
    LoadObjectFile(this->ifName);
}

bool gre3d::Object3D::LoadObjectFile(std::string ifName)
{
    std::ifstream f(ifName);
    //std::ifstream in(ifName, std::ios::binary);
    //constexpr size_t bufferSize = 1024*1024;
    //std::unique_ptr<char[]> buffer(new char[bufferSize]);

    if (!f.is_open())
        return false;

    while(!f.eof())
    {
        char line[128];
        f.getline(line, 128);

        std::strstream strs;
        strs << line;

        char token;

        if (line[0] == 'v')
        {
            vertex v;
            strs >> token >> v.x >> v.y >> v.z;
            vertices.push_back(v);
        }

        if (line[0] == 'f')
        {
            int f[3];
            strs >> token >> f[0] >> f[1] >> f[2];
            //std::cout << f[0] << f[1] << f[2] << std::endl;
            meshObject.tri.push_back({vertices[f[0]-1], vertices[f[1]-1], vertices[f[2]-1]});
        }
    }
    return true;
}

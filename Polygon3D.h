#pragma once
#include "GraphicsEngine3D.h"
#include "Instance.h"
//operator overloading
//Polygon translation
class Polygon3D
{
public:
protected:
    struct vertex
    {
        float x;
        float y;
        float z;
    };
    class vector
    {
    public:
        vertex p[2];
        float xi;
        float yj;
        float zk;
    };
    struct triangle
    {
        vertex p[3];
        uint16_t pxt;
        uint16_t cl;
    };
    struct mesh
    {
        std::vector<triangle> tri;
    };
    struct mat4x4
    {
        float m[4][4] = { 0 };
    };
    virtual void CreateMesh() = 0;

public:
    void MatrixMultiplier(vertex &i, vertex &o, mat4x4 &m)
    {
        o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
        o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
        o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
        float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

        if (w != 0.0f) // evade absurd results
        {
            o.x /= w; o.y /= w; o.z /= w;
        }
    }
    vector getNormalVector(vector &u, vector &v)
    {
        vector w;
        w.p[0].x = u.p[0].x;
        w.p[0].y = u.p[0].y;
        w.p[0].z = u.p[0].z;

        CalcDeterminant(u, v, w.xi, w.yj, w.zk);

        w.p[1].x = w.xi;
        w.p[1].y = w.yj;
        w.p[1].z = w.zk;
        return w;
    }

    float
    getVectorMagnitude(vector &u)
    {
        float x = u.p[1].x-u.p[0].x;
        float y = u.p[1].y-u.p[0].y;
        float z = u.p[1].z-u.p[0].z;
        return sqrtf(x*x + y*y + z*z);
    }

    float
    getVectorMagnitude(float x1, float y1, float z1, float x0, float y0, float z0)
    {
        float x = x1-x0;
        float y = y1-y0;
        float z = z1-z0;
        return sqrtf(x*x + y*y + z*z);
    }

    void
    CalcDeterminant(float u1, float u2, float u3, float v1, float v2, float v3, float &xi, float &yj, float &zk)
    {
        xi = ((u2 * v3) - (u3 * v2));
        yj = -((u1 * v3) - (u3 * v1));
        zk = ((u1 * v2) - (u2 * v1));
    }

    void
    CalcDeterminant(vector u, vector v, float &xi, float &yj, float &zk)
    {
        xi = ((u.yj * v.zk) - (u.zk * v.yj));
        yj = -((u.xi * v.zk) - (u.zk * v.xi));
        zk = ((u.xi * v.yj) - (u.yj * v.xi));
    }

    float
    fCalcDotProduct(float u1, float u2, float u3, float v1, float v2, float v3)
    {
        return u1*v1 + u2*v2 + u3*v3;;
    }
    float
    fCalcDotProduct(vector u, vector v)
    {
        return u.xi*v.xi + u.yj*v.yj + u.zk*v.zk;
    }
};

#pragma once
#include "GraphicsEngine3D.h"
#include "Instance.h"

class Polygon3D
{
protected:
	struct vertex
	{
		float x;
		float y;
		float z;
	};
	struct triangle
	{
		vertex p[3];
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
	virtual void setProjectionMatrix(mat4x4 &matrix, float fZn, float fZf, float fFOV, float fAspectRatio, float fFOVRad) = 0;
	
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
	
};


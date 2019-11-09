#include "GraphicsEngine3D.h"
#include "Instance.h"

int main()
{
	Instance GraphicsEngine;
	if (GraphicsEngine.CreateConsoleWindow(360, 180, 4, 4))
	{
		GraphicsEngine.StartThread(GraphicsEngine);
	}
}
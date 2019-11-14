#include "GraphicsEngine3D.h"
#include "Instance.h"
#include "Initialize.h"

int main()
{
	Initialize init;
	for (unsigned char i = 0; i < 1; i++)
	{
		init.initInstance(new Instance(i), i);
	}
}
#include "GraphicsEngine3D.h"
#include "Instance.h"
#include "Initialize.h"

int main()
{
    gre3d::Initialize init;
    for (unsigned char i = 0; i < 1; i++)
    {
        init.initInstance(new gre3d::Instance(i), i);
    }
    return 0;
}

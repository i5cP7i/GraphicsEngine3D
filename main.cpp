#include "GraphicsEngine3D.h"
#include "Instance.h"
#include "Initialize.h"
/*
    Examen C++
    Dynamic allocation Object* Obj = new Obj[n]; delete[] Obj; => n = aantal
    Container class std::vector, inheritance, polymorphism
*/

// std register qualifier

//Branching: unlikely
int main()
{
    Initialize init;
    for (unsigned char i = 0; i < 1; i++)
    {
        init.initInstance(new Instance(i), i);
    }
}

#include "Initialize.h"

gre3d::Initialize::Initialize()
{

}

gre3d::Initialize::~Initialize()
{
    for (auto iterator = instances.begin(); iterator != instances.end(); ++iterator)
    {
        instances.erase(iterator);
    }
}

void gre3d::Initialize::initInstance(Instance * newInstance, uint8_t ID)
{
    newInstance->c_AppName = ID;
    if (newInstance->CreateConsoleWindow(360, 180, 4, 4))
    {
        instances.push_back(newInstance);
        newInstance->StartThread(*newInstance);
    }
}

std::vector<gre3d::Instance *> gre3d::Initialize::getInstance() const
{
    return std::vector<Instance*>(instances);
}

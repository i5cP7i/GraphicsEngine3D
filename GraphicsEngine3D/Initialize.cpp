#include "Initialize.h"

Initialize::Initialize()
{

}

Initialize::~Initialize()
{
}

void Initialize::initInstance(Instance * newInstance, uint8_t ID)
{
	newInstance->c_AppName = ID;
	if (newInstance->CreateConsoleWindow(360, 180, 4, 4))
	{
		instances.push_back(newInstance);
		newInstance->StartThread(*newInstance);
	}
}

std::vector<Instance*> Initialize::getInstance() const
{
	return std::vector<Instance*>(instances);
}

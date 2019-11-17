#pragma once
#include "Instance.h"

class Initialize
{
public:
	Initialize();
	~Initialize();
public:
	void initInstance(Instance * newInstance, uint8_t ID);
	std::vector<Instance *> getInstance() const;
private:
	std::vector<Instance *> instances;
};


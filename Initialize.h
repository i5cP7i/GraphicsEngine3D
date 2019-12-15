#pragma once
#include "Instance.h"

namespace gre3d {
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
}


#pragma once

#include <memory>
#include <vector>

#include "Coconut/ECS/Compoent.h"


namespace Coconut {

    class Entity {
    private:
        Coconut::Manager& m_manager;
        bool m_active = true;
        std::vector<std::unique_ptr<Component>> 
    }
        
}
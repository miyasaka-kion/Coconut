#pragma once

#include <memory>

#ifdef CC_PLATFORM_WINDOWS
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include "Coconut/Core.h"
#include "Coconut/Game.h"
#include "Coconut/Log.h"

namespace Coconut {

class CC_API Application {
public:
    Application();
    virtual ~Application();

    void run();
};

}  // namespace Coconut

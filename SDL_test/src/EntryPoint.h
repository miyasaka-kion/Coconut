#pragma once
// This is where the whole program start


#include <memory>

// This ignores all warnings raised inside External headers
#ifdef CC_PLATFORM_WINDOWS
#include <SDL.h>
#else 
#include <SDL2/SDL.h>
#endif

#include "Coconut/Application.h"



// >>> this should be move to CMake process in the future!
#define CC_PLATFORM_MACOS
#define BUILD_WITH_CMAKE
// <<< this should be move to CMake process in the future!


#ifdef CC_PLATFORM_MACOS
	#define CC_PLATFORM_OSX
#endif

#if defined CC_PLATFORM_WINDOWS || defined CC_PLATFORM_MACOS

int main(int argc, char* argv[]) {
    auto gameApp = std::make_unique<Coconut::Application>();
	gameApp->run();
	return 0;
}

#endif
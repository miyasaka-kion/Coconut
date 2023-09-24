#pragma once
// This is where the whole program start


#include <memory>

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <SDL2/SDL.h>
#pragma warning()

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
	std::unique_ptr<Coconut::Application> gameApp = std::make_unique<Coconut::Application>();
	gameApp->run();
	return 0;
}

#endif
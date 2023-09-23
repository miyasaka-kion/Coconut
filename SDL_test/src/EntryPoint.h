#pragma once
// This is where the whole program start


#include <memory>

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <SDL2/SDL.h>
#pragma warning()

#include "Coconut/Application.h"


// this line is for OS X debug only!
// should be removed in later version!
#define CC_PLATFORM_WINDOWS

#ifdef CC_PLATFORM_WINDOWS

int main(int argc, char* argv[]) {
	std::unique_ptr<Coconut::Application> gameApp = std::make_unique<Coconut::Application>();
	gameApp->run();


	return 0;
}

#endif
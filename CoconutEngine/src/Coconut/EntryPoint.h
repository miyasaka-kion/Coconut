#pragma once
// This is where the whole program start

#include <memory>

#include <SDL.h>

#include "Coconut/Application.h"

#ifdef CC_PLATFORM_WINDOWS


int main(int argc, char* argv[]) {
	std::unique_ptr<Coconut::Application> gameApp = std::make_unique<Coconut::Application>();
	gameApp->run();


	return 0;
}

#endif
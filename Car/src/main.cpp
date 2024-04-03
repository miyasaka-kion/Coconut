#include <iostream>

#include "Core/Log.h"
#include "Game.h"

int main(int argc, char* argv[]) {
    // Coconut::Log::init();
    auto app = std::make_unique< Game >();
    app->Run();

    return 0;
}
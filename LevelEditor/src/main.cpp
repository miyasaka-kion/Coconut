#include <iostream>

#include "Log.h"
#include "Scene.h"

int main(int argc, char* argv[]) {
    Coconut::Log::init();
    auto app = std::make_unique< Scene >();
    app->Run();

    return 0;
}
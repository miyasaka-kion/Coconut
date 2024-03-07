#include <iostream>

#include "Core/Log.h"
#include "Application.h"

int main(int argc, char* argv[]) {
    Coconut::Log::init();
    auto app = std::make_unique< Application >();
    app->Run();

    return 0;
}
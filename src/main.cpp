#include <memory>

#include "Application.h"
#include "Log.h"

int main(int argc, char* argv[]) {
    Coconut::Log::init();
    std::unique_ptr<Application> app = std::make_unique<Application>();
    app->run();

    return 0;
}
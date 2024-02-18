#include <SDL_log.h>
#include <memory>

#include "Scene.h"
#include "Log.h"

#include <SDL.h>

int main(int argc, char* argv[]) {
    Coconut::Log::init();
    std::unique_ptr<Scene> scene = std::make_unique<Scene>();
    scene->Run();

    return 0;
}
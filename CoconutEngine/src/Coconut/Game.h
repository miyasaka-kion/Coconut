#pragma once

#include <memory>
#include <string>

#include <box2d/box2d.h>
// This ignores all warnings raised inside External headers
#ifdef CC_PLATFORM_WINDOWS
#include <SDL.h>
#include <SDL_image.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif

namespace Coconut {
class Game {
public:
    Game();
    ~Game();

    void gameInit(std::string title, int x, int y, int width, int height, bool fullscreen);

    void handleEvents();
    void update();
    void render();
    void clean();

    inline bool running() const {
        return m_isRunning;
    }

    static SDL_Renderer* renderer;
    static SDL_Event     event;

private:
    static std::unique_ptr<b2World> m_world;

private:
    bool        m_isRunning;
    int         m_gameCounter;
    SDL_Window* m_window;
};
}  // namespace Coconut
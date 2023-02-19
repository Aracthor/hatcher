#pragma once

#include <SDL2/SDL_video.h>

struct SDL_Window;

namespace hatcher
{

class GLContext
{
public:
    GLContext(SDL_Window* window);
    ~GLContext();

    void EnableDepthTest();
    void DisableDepthTest();

    SDL_GLContext* SDLContext() { return &m_contextId; }

private:
    SDL_GLContext m_contextId;
};

} // namespace hatcher

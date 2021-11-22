#pragma once

#include <SDL2/SDL_video.h>

class SDL_Window;

namespace hatcher
{

class GLContext
{
public:
    GLContext(SDL_Window* window);
    ~GLContext();

private:
    SDL_GLContext m_contextId;
};

} // namespace hatcher

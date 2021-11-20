#include "Window.hpp"

#include "GLContext.hpp"
#include <SDL2/SDL.h>
#include <iostream>

namespace hatcher
{

Window::Window(const char* name, int width, int height)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "could not initialize sdl2: " << SDL_GetError() << std::endl;
        std::terminate();
    }
    m_window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width,
                                height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    m_context = new GLContext(m_window);
}

Window::~Window()
{
    delete m_context;
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void Window::Refresh()
{
    SDL_GL_SwapWindow(m_window);
}

} // namespace hatcher
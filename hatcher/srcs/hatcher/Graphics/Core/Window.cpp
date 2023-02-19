#include "Window.hpp"

#include <SDL2/SDL.h>
#include <iostream>

#include "GLContext.hpp"
#include "ImGuiIntegration.hpp"
#include "gl.hpp"

namespace hatcher
{

Window::Window(const char* name, int width, int height)
    : m_width(width)
    , m_height(height)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "could not initialize sdl2: " << SDL_GetError() << std::endl;
        std::terminate();
    }
    m_window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width,
                                height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    m_context.reset(new GLContext(m_window));
    m_imguiIntegration = std::make_unique<ImGuiIntegration>(m_window, m_context->SDLContext());
}

Window::~Window()
{
    m_context.release();
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void Window::EnableDepthTest()
{
    m_context->EnableDepthTest();
}

void Window::DisableDepthTest()
{
    m_context->DisableDepthTest();
}

void Window::Clear()
{
    m_imguiIntegration->NewFrame();
    GL_CHECK(glViewport(0, 0, m_width, m_height));
    GL_CHECK(glClearColor(0.f, 0.f, 0.f, 1.f));
    GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Window::Refresh()
{
    m_imguiIntegration->Render();
    SDL_GL_SwapWindow(m_window);
}

} // namespace hatcher
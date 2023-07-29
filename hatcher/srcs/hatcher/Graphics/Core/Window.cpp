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
    m_window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height,
                                SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    m_context = std::make_unique<GLContext>(m_window);
    m_imguiIntegration = std::make_unique<ImGuiIntegration>(m_window, m_context->SDLContext());
}

Window::~Window()
{
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

std::vector<SDL_Event> Window::PollEvents()
{
    std::vector<SDL_Event> events;
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        m_imguiIntegration->ProcessEvent(event);
        if (m_imguiIntegration->IsInterfaceWindowHovered())
            continue;

        // We invert Y axis for mouse events because there seems to be an inversion between
        // OpenGL and SDL2.
        const SDL_EventType eventType = static_cast<SDL_EventType>(event.type);
        if (eventType == SDL_MOUSEBUTTONDOWN || eventType == SDL_MOUSEBUTTONUP)
            event.button.y = m_height - event.button.y;
        if (eventType == SDL_MOUSEMOTION)
            event.motion.y = m_height - event.motion.y;

        events.push_back(event);
    }
    return events;
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
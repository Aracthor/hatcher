#include "GLContext.hpp"

#include <iostream>

#include "gl.hpp"

namespace hatcher
{

GLContext::GLContext(SDL_Window* window)
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);

    m_contextId = SDL_GL_CreateContext(window);
    if (!m_contextId)
    {
        std::cerr << "Failed to create GL context: " << SDL_GetError() << std::endl;
        std::terminate();
    }
    if (SDL_GL_MakeCurrent(window, m_contextId) < 0)
    {
        std::cerr << "Failed to make GL context current: " << SDL_GetError() << std::endl;
        std::terminate();
    }

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
        std::cerr << "GLEW not initialized correctly" << std::endl;

    GL_CHECK(glEnable(GL_BLEND));
    GL_CHECK(glEnable(GL_DEPTH_TEST));
    GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

GLContext::~GLContext()
{
    SDL_GL_DeleteContext(m_contextId);
}

void GLContext::EnableDepthTest()
{
    GL_CHECK(glEnable(GL_DEPTH_TEST));
}

void GLContext::DisableDepthTest()
{
    GL_CHECK(glDisable(GL_DEPTH_TEST));
}

} // namespace hatcher
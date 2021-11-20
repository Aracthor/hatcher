#include "DemoApplication.hpp"

#include "hatcher/Graphics/ShaderProgram.hpp"
#include "hatcher/Graphics/VertexArrayObject.hpp"
#include "hatcher/Graphics/VertexBufferObject.hpp"

#include <SDL2/SDL.h>
#include <iostream>

DemoApplication::DemoApplication()
    : hatcher::GameApplication("hatcher - demo", 800, 600)
{
    float points[] = {0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f};
    m_vbo.reset(new hatcher::VertexBufferObject());
    m_vbo->SetData(points, 9);

    m_vao.reset(new hatcher::VertexArrayObject());
    m_vao->AttribVBO(*m_vbo, 0);

    m_program.reset(
        new hatcher::ShaderProgram("shaders/hello_world.vert", "shaders/hello_world.frag"));
}

DemoApplication::~DemoApplication() = default;

void DemoApplication::HandleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            Stop();
        }
        if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                Stop();
            }
        }
    }
}

void DemoApplication::RenderUpdate()
{
    m_program->Use();
    m_vao->Draw();
}

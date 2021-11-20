#include "SquareDisplayUpdater.hpp"

#include "hatcher/Graphics/ShaderProgram.hpp"
#include "hatcher/Graphics/VertexArrayObject.hpp"
#include "hatcher/Graphics/VertexBufferObject.hpp"
#include "hatcher/World.hpp"

SquareDisplayUpdater::SquareDisplayUpdater()
{
    float points[] = {0.5f, 0.5f, 0.0f, 0.5f,  -0.5f, 0.0f, -0.5f, -0.5f, 0.0f,
                      0.5f, 0.5f, 0.0f, -0.5f, 0.5f,  0.0f, -0.5f, -0.5f, 0.0f};
    m_vbo.reset(new hatcher::VertexBufferObject());
    m_vbo->SetData(points, 18);

    m_vao.reset(new hatcher::VertexArrayObject());
    m_vao->AttribVBO(*m_vbo, 0);

    m_program.reset(
        new hatcher::ShaderProgram("shaders/hello_world.vert", "shaders/hello_world.frag"));
}

SquareDisplayUpdater::~SquareDisplayUpdater() = default;

void SquareDisplayUpdater::Update(hatcher::World& world)
{
    (void)world;
    m_program->Use();
    m_vao->Draw();
}

#include "SquareDisplayUpdater.hpp"

#include "hatcher/Graphics/ShaderProgram.hpp"
#include "hatcher/Graphics/VertexArrayObject.hpp"
#include "hatcher/Graphics/VertexBufferObject.hpp"
#include "hatcher/World.hpp"
#include "hatcher/glm_pure.hpp"

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
    glm::mat4 viewMatrix = glm::mat4(1.f);
    viewMatrix[0][0] = 600.f / 800.f;
    m_program->SetMatrix4Uniform("uniViewMatrix", glm::value_ptr(viewMatrix));

    m_vao->Draw();
}

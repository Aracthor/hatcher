#include "SquareDisplayUpdater.hpp"

#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/ShaderProgram.hpp"
#include "hatcher/Graphics/VertexArrayObject.hpp"
#include "hatcher/Graphics/VertexBufferObject.hpp"
#include "hatcher/glm_pure.hpp"

#include "Position2DComponent.hpp"

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

void SquareDisplayUpdater::Update(hatcher::ComponentManager* componentManager)
{
    m_program->Use();
    glm::mat4 viewMatrix = glm::mat4(1.f);
    viewMatrix[0][0] = 600.f / 800.f;
    m_program->SetMatrix4Uniform("uniViewMatrix", glm::value_ptr(viewMatrix));

    glm::mat4 modelMatrix = glm::mat4(1.f);

    for (const std::optional<Position2DComponent> component :
         componentManager->GetComponents<Position2DComponent>())
    {
        if (component)
        {
            modelMatrix[3][0] = component->Position.x;
            modelMatrix[3][1] = component->Position.y;
            m_program->SetMatrix4Uniform("uniModelMatrix", glm::value_ptr(modelMatrix));
            m_vao->Draw();
        }
    }
}

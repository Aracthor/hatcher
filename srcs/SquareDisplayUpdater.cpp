#include "SquareDisplayUpdater.hpp"

#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/Graphics/Mesh.hpp"
#include "hatcher/Graphics/MeshBuilder.hpp"
#include "hatcher/glm_pure.hpp"

#include "Position2DComponent.hpp"

SquareDisplayUpdater::SquareDisplayUpdater(const std::unique_ptr<hatcher::MeshBuilder>& meshBuilder)
{
    // clang-format off
    float points[] =
    {
         0.5f,  0.5f,
         0.5f, -0.5f,
        -0.5f, -0.5f,
        -0.5f,  0.5f,
    };
    hatcher::ushort indices[] =
    {
        0, 1, 2,
        0, 3, 2
    };
    // clang-format on
    meshBuilder->SetPrimitive(hatcher::Primitive::Triangles);
    meshBuilder->SetProgram("shaders/hello_world.vert", "shaders/hello_world.frag");

    m_mesh.reset(meshBuilder->Create());
    m_mesh->SetPositions(points, std::size(points));
    m_mesh->SetIndices(indices, std::size(indices));
}

SquareDisplayUpdater::~SquareDisplayUpdater() = default;

void SquareDisplayUpdater::Update(const hatcher::ComponentManager* componentManager,
                                  const hatcher::Clock& clock,
                                  hatcher::IFrameRenderer& frameRenderer)
{
    (void)clock;
    glm::mat4 modelMatrix = glm::mat4(1.f);

    for (const std::optional<Position2DComponent> component :
         componentManager->GetComponents<Position2DComponent>())
    {
        if (component)
        {
            modelMatrix[3][0] = component->Position.x;
            modelMatrix[3][1] = component->Position.y;
            frameRenderer.AddMeshToRender(m_mesh.get(), modelMatrix);
        }
    }
}

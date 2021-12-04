#include "SelectedRenderUpdater.hpp"

#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/Graphics/Mesh.hpp"
#include "hatcher/Graphics/MeshBuilder.hpp"

#include "Position2DComponent.hpp"
#include "Selectable2DComponent.hpp"

SelectedRenderUpdater::SelectedRenderUpdater(
    const std::unique_ptr<hatcher::MeshBuilder>& meshBuilder)
{
    meshBuilder->SetPrimitive(hatcher::Primitive::Lines);
    meshBuilder->SetProgram("shaders/hello_world.vert", "shaders/hello_world.frag");
    m_mesh.reset(meshBuilder->Create());

    float positions[] = {
        -1.f, -1.f,

        1.f,  -1.f,

        1.f,  1.f,

        -1.f, 1.f,
    };
    hatcher::ushort indices[] = {0, 1, 1, 2, 2, 3, 3, 0};

    m_mesh->SetPositions(positions, std::size(positions));
    m_mesh->SetIndices(indices, std::size(indices));
}

SelectedRenderUpdater::~SelectedRenderUpdater() = default;

void SelectedRenderUpdater::Update(const hatcher::ComponentManager* componentManager,
                                   const hatcher::Clock& clock,
                                   hatcher::IFrameRenderer& frameRenderer)
{
    auto selectableComponents = componentManager->GetComponents<Selectable2DComponent>();
    auto positionComponents = componentManager->GetComponents<Position2DComponent>();
    HATCHER_ASSERT(selectableComponents.size() == positionComponents.size());
    for (uint i = 0; i < selectableComponents.size(); i++)
    {
        const std::optional<Selectable2DComponent>& selectableComponent = selectableComponents[i];
        const std::optional<Position2DComponent>& positionComponent = positionComponents[i];
        if (selectableComponent && selectableComponent->Selected)
        {
            HATCHER_ASSERT(positionComponent);
            // const hatcher::Box2f box = selectableComponent->Box;
            const glm::vec3 position = glm::vec3(positionComponent->Position, 0.f);
            const glm::mat4 modelMatrix = glm::translate(glm::mat4(1.f), position);
            frameRenderer.AddMeshToRender(m_mesh.get(), modelMatrix);
        }
    }
    (void)clock;
}

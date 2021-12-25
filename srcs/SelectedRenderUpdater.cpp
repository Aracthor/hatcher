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
    meshBuilder->SetProgram("shaders/selection.vert", "shaders/selection.frag");
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
                                   hatcher::ComponentManager* renderComponentManager,
                                   const hatcher::Clock& clock,
                                   hatcher::IFrameRenderer& frameRenderer)
{
    (void)renderComponentManager;

    auto selectableComponents = componentManager->GetComponents<Selectable2DComponent>();
    auto positionComponents = componentManager->GetComponents<Position2DComponent>();
    HATCHER_ASSERT(selectableComponents.size() == positionComponents.size());
    for (hatcher::uint i = 0; i < selectableComponents.size(); i++)
    {
        const std::optional<Selectable2DComponent>& selectableComponent = selectableComponents[i];
        const std::optional<Position2DComponent>& positionComponent = positionComponents[i];
        if (selectableComponent && selectableComponent->Selected)
        {
            HATCHER_ASSERT(positionComponent);
            const hatcher::Box2f box = selectableComponent->Box;
            glm::mat4 modelMatrix =
                glm::inverse(glm::ortho(box.Min().x, box.Max().x, box.Min().y, box.Max().y));
            modelMatrix[3][0] += positionComponent->Position.x;
            modelMatrix[3][1] += positionComponent->Position.y;
            frameRenderer.AddMeshToRender(m_mesh.get(), modelMatrix);
        }
    }
    (void)clock;
}

#include "SelectedRenderUpdater.hpp"

#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/Graphics/IRendering.hpp"
#include "hatcher/Graphics/Mesh.hpp"
#include "hatcher/Graphics/MeshBuilder.hpp"

#include "Position2DComponent.hpp"
#include "Selectable2DComponent.hpp"

SelectedRenderUpdater::SelectedRenderUpdater(
    const std::unique_ptr<hatcher::MeshBuilder>& meshBuilder)
{
    meshBuilder->SetPrimitive(hatcher::Primitive::Lines);
    meshBuilder->SetMaterial(
        meshBuilder->CreateMaterial("shaders/selection.vert", "shaders/selection.frag"));
    m_mesh.reset(meshBuilder->Create());

    float positions[] = {
        -1.f, -1.f,

        1.f,  -1.f,

        1.f,  1.f,

        -1.f, 1.f,
    };
    hatcher::ushort indices[] = {0, 1, 1, 2, 2, 3, 3, 0};

    m_mesh->Set2DPositions(positions, std::size(positions));
    m_mesh->SetIndices(indices, std::size(indices));
}

SelectedRenderUpdater::~SelectedRenderUpdater() = default;

void SelectedRenderUpdater::Update(const hatcher::ComponentManager* componentManager,
                                   hatcher::ComponentManager* renderComponentManager,
                                   const hatcher::IRendering& rendering,
                                   hatcher::IFrameRenderer& frameRenderer)
{
    auto selectableComponents = componentManager->GetComponents<Selectable2DComponent>();
    auto positionComponents = componentManager->GetComponents<Position2DComponent>();
    HATCHER_ASSERT(selectableComponents.size() == positionComponents.size());
    for (hatcher::uint i = 0; i < selectableComponents.size(); i++)
    {
        const std::optional<Selectable2DComponent>& selectableComponent = selectableComponents[i];
        const std::optional<Position2DComponent>& positionComponent = positionComponents[i];
        if (selectableComponent && selectableComponent->selected)
        {
            const glm::mat4 modelMatrix =
                glm::translate(glm::vec3(positionComponent->position, 0.f));
            const hatcher::Box2f selectionBox =
                rendering.ProjectBox3DToWindowCoords(selectableComponent->box, modelMatrix);

            const glm::vec2 rectangleCenter = selectionBox.Center();
            const glm::vec2 rectangleSize = selectionBox.Extents();

            const glm::vec3 position = {rectangleCenter.x, rectangleCenter.y, 0.f};
            const glm::vec3 scale = {rectangleSize.x / 2.f, rectangleSize.y / 2.f, 0.f};

            glm::mat4 selectedModelMatrix = glm::mat4(1.f);
            selectedModelMatrix = glm::translate(selectedModelMatrix, position);
            selectedModelMatrix = glm::scale(selectedModelMatrix, scale);

            frameRenderer.AddUIMeshToRender(m_mesh.get(), selectedModelMatrix);
        }
    }
}

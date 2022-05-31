#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/Graphics/IRendering.hpp"
#include "hatcher/Graphics/MaterialFactory.hpp"
#include "hatcher/Graphics/Mesh.hpp"
#include "hatcher/Graphics/MeshBuilder.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"

#include "Movement2DComponent.hpp"
#include "Position2DComponent.hpp"
#include "Selectable2DComponent.hpp"
#include "TransformationHelper.hpp"

namespace
{

class SelectedRenderUpdater final : public hatcher::RenderUpdater
{
public:
    SelectedRenderUpdater(const hatcher::IRendering* rendering)
    {
        hatcher::MeshBuilder* meshBuilder = rendering->GetMeshBuilder().get();
        meshBuilder->SetPrimitive(hatcher::Primitive::Lines);
        meshBuilder->SetMaterial(rendering->GetMaterialFactory()->CreateMaterial(
            "shaders/selection.vert", "shaders/selection.frag"));
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

    ~SelectedRenderUpdater() = default;

    void Update(const hatcher::ComponentManager* componentManager,
                hatcher::ComponentManager* renderComponentManager,
                hatcher::IFrameRenderer& frameRenderer) override
    {
        auto selectableComponents = renderComponentManager->GetComponents<Selectable2DComponent>();
        auto positionComponents = componentManager->GetComponents<Position2DComponent>();
        auto movementComponents = componentManager->GetComponents<Movement2DComponent>();
        HATCHER_ASSERT(selectableComponents.size() == positionComponents.size());
        HATCHER_ASSERT(selectableComponents.size() == movementComponents.size());
        for (hatcher::uint i = 0; i < selectableComponents.size(); i++)
        {
            const std::optional<Selectable2DComponent>& selectableComponent =
                selectableComponents[i];
            const std::optional<Position2DComponent>& positionComponent = positionComponents[i];
            const std::optional<Movement2DComponent>& movementComponent = movementComponents[i];
            if (selectableComponent && selectableComponent->selected)
            {
                const glm::mat4 modelMatrix =
                    TransformationHelper::ModelFromComponents(positionComponent, movementComponent);

                const hatcher::Box2f selectionBox =
                    frameRenderer.ProjectBox3DToWindowCoords(selectableComponent->box, modelMatrix);

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

private:
    std::unique_ptr<hatcher::Mesh> m_mesh;
};

const int dummy = hatcher::RegisterRenderUpdater<SelectedRenderUpdater>("Selected");

} // namespace

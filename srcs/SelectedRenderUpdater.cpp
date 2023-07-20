#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/Graphics/IRendering.hpp"
#include "hatcher/Graphics/MaterialFactory.hpp"
#include "hatcher/Graphics/Mesh.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"

#include "Movement2DComponent.hpp"
#include "Position2DComponent.hpp"
#include "SelectableComponent.hpp"
#include "TransformationHelper.hpp"

using namespace hatcher;

namespace
{

class SelectedRenderUpdater final : public RenderUpdater
{
public:
    SelectedRenderUpdater(const IRendering* rendering, IEventUpdater* eventUpdater)
    {
        const std::shared_ptr<Material> material =
            rendering->GetMaterialFactory()->CreateMaterial("shaders/selection.vert", "shaders/selection.frag");
        m_mesh = std::make_unique<Mesh>(material, Primitive::Lines);

        float positions[] = {
            -1.f, -1.f,

            1.f,  -1.f,

            1.f,  1.f,

            -1.f, 1.f,
        };
        ushort indices[] = {0, 1, 1, 2, 2, 3, 3, 0};

        m_mesh->Set2DPositions(positions, std::size(positions));
        m_mesh->SetIndices(indices, std::size(indices));
    }

    ~SelectedRenderUpdater() = default;

    void Update(const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                IFrameRenderer& frameRenderer) override
    {
        auto selectableComponents = renderComponentManager->ReadComponents<SelectableComponent>();
        auto positionComponents = componentManager->ReadComponents<Position2DComponent>();
        auto movementComponents = componentManager->ReadComponents<Movement2DComponent>();
        for (int i = 0; i < componentManager->Count(); i++)
        {
            const std::optional<SelectableComponent>& selectableComponent = selectableComponents[i];
            const std::optional<Position2DComponent>& positionComponent = positionComponents[i];
            const std::optional<Movement2DComponent>& movementComponent = movementComponents[i];
            if (selectableComponent && selectableComponent->selected)
            {
                const glm::mat4 modelMatrix =
                    TransformationHelper::ModelFromComponents(positionComponent, movementComponent);

                const Box2f selectionBox =
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
    std::unique_ptr<Mesh> m_mesh;
};

const int dummy = RegisterRenderUpdater<SelectedRenderUpdater>();

} // namespace

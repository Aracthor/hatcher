#include "RenderUpdaterOrder.hpp"

#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/Graphics/IRendering.hpp"
#include "hatcher/Graphics/Material.hpp"
#include "hatcher/Graphics/MaterialFactory.hpp"
#include "hatcher/Graphics/Mesh.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/unique_ptr.hpp"

#include "Components/Position2DComponent.hpp"
#include "RenderComponents/SelectableComponent.hpp"
#include "TransformationHelper.hpp"

using namespace hatcher;

namespace
{

class SelectedRenderUpdater final : public RenderUpdater
{
public:
    SelectedRenderUpdater(const IRendering* rendering)
    {
        m_material =
            rendering->GetMaterialFactory()->CreateMaterial("shaders/selection.vert", "shaders/selection.frag");
        m_mesh = make_unique<Mesh>(m_material.get(), Primitive::Lines);

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

    void Update(IApplication* application, const ComponentManager* componentManager,
                ComponentManager* renderComponentManager, IFrameRenderer& frameRenderer) override
    {
        frameRenderer.DisableDepthTest();
        frameRenderer.PrepareUIDraw(m_material.get());

        auto selectableComponents = renderComponentManager->ReadComponents<SelectableComponent>();
        auto positionComponents = componentManager->ReadComponents<Position2DComponent>();
        for (int i = 0; i < componentManager->Count(); i++)
        {
            const std::optional<SelectableComponent>& selectableComponent = selectableComponents[i];
            const std::optional<Position2DComponent>& positionComponent = positionComponents[i];
            if (positionComponent && selectableComponent && selectableComponent->selected)
            {
                const glm::mat4 modelMatrix = TransformationHelper::ModelFromComponents(positionComponent);

                const Box2f selectionBox =
                    frameRenderer.ProjectBox3DToWindowCoords(selectableComponent->box, modelMatrix);

                const glm::vec2 rectangleCenter = selectionBox.Center();
                const glm::vec2 rectangleSize = selectionBox.Extents();

                const glm::vec3 position = {rectangleCenter.x, rectangleCenter.y, 0.f};
                const glm::vec3 scale = {rectangleSize.x / 2.f, rectangleSize.y / 2.f, 0.f};

                glm::mat4 selectedModelMatrix = glm::mat4(1.f);
                selectedModelMatrix = glm::translate(selectedModelMatrix, position);
                selectedModelMatrix = glm::scale(selectedModelMatrix, scale);

                m_mesh->Draw(selectedModelMatrix);
            }
        }
        frameRenderer.EnableDepthTest();
    }

private:
    unique_ptr<Material> m_material;
    unique_ptr<Mesh> m_mesh;
};

RenderUpdaterRegisterer<SelectedRenderUpdater> registerer((int)ERenderUpdaterOrder::Interface);

} // namespace

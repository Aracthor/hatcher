#include "RenderUpdaterOrder.hpp"

#include "hatcher/Clock.hpp"
#include "hatcher/ComponentAccessor.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/Graphics/IRendering.hpp"
#include "hatcher/Graphics/Material.hpp"
#include "hatcher/Graphics/MaterialFactory.hpp"
#include "hatcher/Graphics/Mesh.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/unique_ptr.hpp"

#include "Components/ObstacleComponent.hpp"
#include "Components/Position2DComponent.hpp"
#include "RenderComponents/SelectableComponent.hpp"
#include "utils/TransformationHelper.hpp"

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
        m_mesh = make_unique<Mesh>(m_material.get(), Primitive::TriangleStrip);

        float positions[] = {
            -0.5f, -0.5f, 0.0f,

            -0.5f, -0.5f, 0.5f,

            -0.5f, 0.5f,  0.0f,

            -0.5f, 0.5f,  0.5f,

            0.5f,  0.5f,  0.0f,

            0.5f,  0.5f,  0.5f,

            0.5f,  -0.5f, 0.0f,

            0.5f,  -0.5f, 0.5f,
        };
        ushort indices[] = {0, 1, 2, 3, 4, 5, 6, 7, 0, 1};

        m_mesh->Set3DPositions(positions, std::size(positions));
        m_mesh->SetIndices(indices, std::size(indices));
    }

    ~SelectedRenderUpdater() = default;

    void Update(IApplication* application, const ComponentAccessor* componentAccessor,
                ComponentAccessor* renderComponentAccessor, IFrameRenderer& frameRenderer) override
    {
        m_elapsedTime += frameRenderer.GetClock()->GetElapsedTime() / 1000.f;
        m_material->SetUniform("uniElapsedTime", m_elapsedTime);
        frameRenderer.PrepareSceneDraw(m_material.get());

        auto selectableComponents = renderComponentAccessor->ReadComponents<SelectableComponent>();
        auto positionComponents = componentAccessor->ReadComponents<Position2DComponent>();
        auto obstacleComponents = componentAccessor->ReadComponents<ObstacleComponent>();
        for (int i = 0; i < componentAccessor->Count(); i++)
        {
            const std::optional<SelectableComponent>& selectableComponent = selectableComponents[i];
            const std::optional<Position2DComponent>& positionComponent = positionComponents[i];
            if (positionComponent && selectableComponent && selectableComponent->selected)
            {
                glm::mat4 modelMatrix = TransformationHelper::ModelFromComponents(*positionComponent);
                if (obstacleComponents[i])
                {
                    const glm::ivec2 boxExtent = obstacleComponents[i]->area.Extents();
                    const glm::vec2 scaleXY = static_cast<glm::vec2>(boxExtent) + glm::vec2(1.f, 1.f);
                    const glm::vec3 scale = glm::vec3(scaleXY, 1.f) * 1.1f;
                    modelMatrix = glm::scale(modelMatrix, scale);
                }

                m_mesh->Draw(modelMatrix);
            }
        }
    }

private:
    unique_ptr<Material> m_material;
    unique_ptr<Mesh> m_mesh;
    float m_elapsedTime = 0.f;
};

RenderUpdaterRegisterer<SelectedRenderUpdater> registerer((int)ERenderUpdaterOrder::Interface);

} // namespace

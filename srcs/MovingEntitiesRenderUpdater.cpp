#include <vector>

#define _USE_MATH_DEFINES
#include <cmath>

#include "Movement2DComponent.hpp"
#include "Position2DComponent.hpp"
#include "TransformationHelper.hpp"

#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/Graphics/IRendering.hpp"
#include "hatcher/Graphics/MaterialFactory.hpp"
#include "hatcher/Graphics/Mesh.hpp"
#include "hatcher/Graphics/MeshBuilder.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/assert.hpp"
#include "hatcher/glm_pure.hpp"

using namespace hatcher;

namespace
{

class MovingEntitiesRenderUpdater final : public RenderUpdater
{
public:
    MovingEntitiesRenderUpdater(const IRendering* rendering)
    {
        const int circleVertexCount = 20;
        std::vector<float> positions;
        std::vector<ushort> indices;
        positions.reserve((circleVertexCount + 2) * 2);
        indices.reserve(circleVertexCount * 2 + 2);
        for (int i = 0; i < circleVertexCount; i++)
        {
            const float ratio = static_cast<float>(i) / static_cast<float>(circleVertexCount);
            const float angle = M_PI * 2.f * ratio;
            const float x = ::cosf(angle);
            const float y = ::sinf(angle);
            positions.push_back(x);
            positions.push_back(y);
            const ushort startIndex = i;
            const ushort endIndex = (i == circleVertexCount - 1) ? 0 : (i + 1);
            indices.push_back(startIndex);
            indices.push_back(endIndex);
        }

        const float orientationLinePositions[] = {0.f, 0.f, 1.f, 0.f};
        positions.insert(positions.end(), orientationLinePositions,
                         orientationLinePositions + std::size(orientationLinePositions));
        indices.push_back(circleVertexCount);
        indices.push_back(circleVertexCount + 1);

        MeshBuilder* meshBuilder = rendering->GetMeshBuilder().get();

        meshBuilder->SetPrimitive(Primitive::Lines);
        meshBuilder->SetMaterial(rendering->GetMaterialFactory()->CreateMaterial(
            "shaders/hello_world_2D.vert", "shaders/hello_world.frag"));

        m_mesh.reset(meshBuilder->Create());
        m_mesh->Set2DPositions(positions.data(), std::size(positions));
        m_mesh->SetIndices(indices.data(), std::size(indices));
    }

    ~MovingEntitiesRenderUpdater() = default;

    void Update(const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                IFrameRenderer& frameRenderer) override
    {
        ComponentReader<Position2DComponent> positions =
            componentManager->ReadComponents<Position2DComponent>();
        ComponentReader<Movement2DComponent> movements =
            componentManager->ReadComponents<Movement2DComponent>();

        for (int i = 0; i < componentManager->Count(); i++)
        {
            if (movements[i])
            {
                HATCHER_ASSERT(positions[i]);
                const glm::mat4 modelMatrix =
                    TransformationHelper::ModelFromComponents(positions[i], movements[i]);

                frameRenderer.AddMeshToRender(m_mesh.get(), modelMatrix);
            }
        }
    }

private:
    std::unique_ptr<Mesh> m_mesh;
};

const int dummy = RegisterRenderUpdater<MovingEntitiesRenderUpdater>("MovingEntities");

} // namespace

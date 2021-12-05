#include "MovingEntitiesRenderUpdater.hpp"

#include <span>
#include <vector>

#define _USE_MATH_DEFINES
#include <cmath>

#include "Movement2DComponent.hpp"
#include "Position2DComponent.hpp"

#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/Graphics/Mesh.hpp"
#include "hatcher/Graphics/MeshBuilder.hpp"
#include "hatcher/Maths/vectors.hpp"
#include "hatcher/assert.hpp"
#include "hatcher/glm_pure.hpp"

MovingEntitiesRenderUpdater::MovingEntitiesRenderUpdater(
    const std::unique_ptr<hatcher::MeshBuilder>& meshBuilder)
{
    const int circleVertexCount = 20;
    std::vector<float> positions;
    std::vector<hatcher::ushort> indices;
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
        const hatcher::ushort startIndex = i;
        const hatcher::ushort endIndex = (i == circleVertexCount - 1) ? 0 : (i + 1);
        indices.push_back(startIndex);
        indices.push_back(endIndex);
    }

    const float orientationLinePositions[] = {0.f, 0.f, 1.f, 0.f};
    positions.insert(positions.end(), orientationLinePositions,
                     orientationLinePositions + std::size(orientationLinePositions));
    indices.push_back(circleVertexCount);
    indices.push_back(circleVertexCount + 1);

    meshBuilder->SetPrimitive(hatcher::Primitive::Lines);
    meshBuilder->SetProgram("shaders/hello_world.vert", "shaders/hello_world.frag");

    m_mesh.reset(meshBuilder->Create());
    m_mesh->SetPositions(positions.data(), std::size(positions));
    m_mesh->SetIndices(indices.data(), std::size(indices));
}

MovingEntitiesRenderUpdater::~MovingEntitiesRenderUpdater() = default;

void MovingEntitiesRenderUpdater::Update(const hatcher::ComponentManager* componentManager,
                                         const hatcher::Clock& clock,
                                         hatcher::IFrameRenderer& frameRenderer)
{
    (void)clock;

    const std::span<const std::optional<Position2DComponent>> positions =
        componentManager->GetComponents<Position2DComponent>();
    const std::span<const std::optional<Movement2DComponent>> movements =
        componentManager->GetComponents<Movement2DComponent>();

    HATCHER_ASSERT(positions.size() == movements.size());
    for (size_t i = 0; i < positions.size(); i++)
    {
        if (movements[i])
        {
            HATCHER_ASSERT(positions[i]);
            const Position2DComponent& position2D = *positions[i];
            const Movement2DComponent& movement2D = *movements[i];
            const float angle = hatcher::angle(movement2D.Orientation, glm::vec2(1.f, 0.f));
            glm::mat4 modelMatrix = glm::mat4(1.f);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(position2D.Position, 0.f));
            modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0.f, 0.f, 1.f));
            frameRenderer.AddMeshToRender(m_mesh.get(), modelMatrix);
        }
    }
}

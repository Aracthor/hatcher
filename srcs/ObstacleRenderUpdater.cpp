#include "ObstacleRenderUpdater.hpp"

#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/Graphics/Mesh.hpp"
#include "hatcher/Graphics/MeshBuilder.hpp"
#include "hatcher/glm_pure.hpp"

#include "Obstacle2DComponent.hpp"
#include "ObstacleMeshComponent.hpp"

ObstacleRenderUpdater::ObstacleRenderUpdater(
    const std::unique_ptr<hatcher::MeshBuilder>& meshBuilder)
    : m_meshBuilder(meshBuilder)
{
}

ObstacleRenderUpdater::~ObstacleRenderUpdater() = default;

void ObstacleRenderUpdater::Update(const hatcher::ComponentManager* componentManager,
                                   hatcher::ComponentManager* renderComponentManager,
                                   const hatcher::IRendering& rendering,
                                   hatcher::IFrameRenderer& frameRenderer)
{
    const hatcher::span<const std::optional<Obstacle2DComponent>> obstacleComponents =
        componentManager->GetComponents<Obstacle2DComponent>();
    hatcher::span<std::optional<ObstacleMeshComponent>> obstacleMeshComponents =
        renderComponentManager->GetComponents<ObstacleMeshComponent>();

    for (hatcher::uint i = 0; i < obstacleComponents.size(); i++)
    {
        const std::optional<Obstacle2DComponent> obstacle2D = obstacleComponents[i];
        std::optional<ObstacleMeshComponent>& obstacleMesh = obstacleMeshComponents[i];
        if (obstacle2D)
        {
            if (!obstacleMesh)
            {
                obstacleMesh = std::make_optional<ObstacleMeshComponent>();
                obstacleMesh->mesh = CreateMeshFromObstacle(*obstacle2D);
            }
            frameRenderer.AddMeshToRender(obstacleMesh->mesh.get(), glm::mat4(1.f));
        }
    }
}

std::shared_ptr<hatcher::Mesh>
ObstacleRenderUpdater::CreateMeshFromObstacle(const Obstacle2DComponent& obstacleComponent)
{
    std::shared_ptr<hatcher::Mesh> result;

    m_meshBuilder->SetPrimitive(hatcher::Primitive::Lines);
    m_meshBuilder->SetMaterial(
        m_meshBuilder->CreateMaterial("shaders/hello_world_2D.vert", "shaders/hello_world.frag"));
    result.reset(m_meshBuilder->Create());

    auto obstacleCorners = obstacleComponent.corners;
    int cornersCount = obstacleCorners.size();

    std::vector<float> positions;
    std::vector<hatcher::ushort> indices;
    positions.reserve(cornersCount * 2);
    indices.reserve(cornersCount * 2);
    for (int i = 0; i < cornersCount; i++)
    {
        positions.push_back(obstacleCorners[i].x);
        positions.push_back(obstacleCorners[i].y);
        indices.push_back(i);
        indices.push_back(i == cornersCount - 1 ? 0 : i + 1);
    }

    result->Set2DPositions(positions.data(), std::size(positions));
    result->SetIndices(indices.data(), std::size(indices));

    return result;
}

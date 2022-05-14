#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/Graphics/IRendering.hpp"
#include "hatcher/Graphics/Material.hpp"
#include "hatcher/Graphics/MaterialFactory.hpp"
#include "hatcher/Graphics/Mesh.hpp"
#include "hatcher/Graphics/MeshBuilder.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/glm_pure.hpp"

#include "Obstacle2DComponent.hpp"
#include "ObstacleMeshComponent.hpp"

namespace
{

class ObstacleRenderUpdater final : public hatcher::RenderUpdater
{
public:
    ObstacleRenderUpdater(const hatcher::IRendering* rendering)
        : m_meshBuilder(rendering->GetMeshBuilder())
        , m_material(rendering->GetMaterialFactory()->CreateMaterial("shaders/hello_world_2D.vert",
                                                                     "shaders/hello_world.frag"))
    {
    }

    ~ObstacleRenderUpdater() = default;

    void Update(const hatcher::ComponentManager* componentManager,
                hatcher::ComponentManager* renderComponentManager,
                hatcher::IFrameRenderer& frameRenderer) override
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

private:
    std::shared_ptr<hatcher::Mesh>
    CreateMeshFromObstacle(const Obstacle2DComponent& obstacleComponent)
    {
        std::shared_ptr<hatcher::Mesh> result;

        m_meshBuilder->SetPrimitive(hatcher::Primitive::Lines);
        m_meshBuilder->SetMaterial(m_material);
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

    const std::unique_ptr<hatcher::MeshBuilder>& m_meshBuilder;
    std::shared_ptr<hatcher::Material> m_material;
};

const int dummy = hatcher::RegisterRenderUpdater<ObstacleRenderUpdater>("Obstacle");

} // namespace
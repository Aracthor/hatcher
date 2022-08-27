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

using namespace hatcher;

namespace
{

class ObstacleRenderUpdater final : public RenderUpdater
{
public:
    ObstacleRenderUpdater(const IRendering* rendering)
        : m_meshBuilder(rendering->GetMeshBuilder())
        , m_material(rendering->GetMaterialFactory()->CreateMaterial("shaders/hello_world_2D.vert",
                                                                     "shaders/hello_world.frag"))
    {
    }

    ~ObstacleRenderUpdater() = default;

    void Update(const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                IFrameRenderer& frameRenderer) override
    {
        ComponentReader<Obstacle2DComponent> obstacleComponents =
            componentManager->ReadComponents<Obstacle2DComponent>();
        ComponentWriter<ObstacleMeshComponent> obstacleMeshComponents =
            renderComponentManager->WriteComponents<ObstacleMeshComponent>();

        for (int i = 0; i < componentManager->Count(); i++)
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
    std::shared_ptr<Mesh> CreateMeshFromObstacle(const Obstacle2DComponent& obstacleComponent)
    {
        std::shared_ptr<Mesh> result;

        m_meshBuilder->SetPrimitive(Primitive::Lines);
        m_meshBuilder->SetMaterial(m_material);
        result.reset(m_meshBuilder->Create());

        auto obstacleCorners = obstacleComponent.corners;
        int cornersCount = obstacleCorners.size();

        std::vector<float> positions;
        std::vector<ushort> indices;
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

    const std::unique_ptr<MeshBuilder>& m_meshBuilder;
    std::shared_ptr<Material> m_material;
};

const int dummy = RegisterRenderUpdater<ObstacleRenderUpdater>("Obstacle");

} // namespace
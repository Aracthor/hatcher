#pragma once

#include <memory>

#include "hatcher/Graphics/RenderUpdater.hpp"

namespace hatcher
{
class Material;
class Mesh;
class MeshBuilder;
class World;
} // namespace hatcher
struct Obstacle2DComponent;

class ObstacleRenderUpdater final : public hatcher::RenderUpdater
{
public:
    ObstacleRenderUpdater(const hatcher::IRendering* rendering);
    ~ObstacleRenderUpdater();

    void Update(const hatcher::ComponentManager* componentManager,
                hatcher::ComponentManager* renderComponentManager,
                const hatcher::IRendering& rendering,
                hatcher::IFrameRenderer& frameRenderer) override;

private:
    std::shared_ptr<hatcher::Mesh>
    CreateMeshFromObstacle(const Obstacle2DComponent& obstacleComponent);

    const std::unique_ptr<hatcher::MeshBuilder>& m_meshBuilder;
    std::shared_ptr<hatcher::Material> m_material;
};

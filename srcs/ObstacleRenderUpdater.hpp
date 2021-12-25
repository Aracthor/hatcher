#pragma once

#include <memory>

#include "hatcher/Graphics/RenderUpdater.hpp"

namespace hatcher
{
class Mesh;
class MeshBuilder;
class World;
} // namespace hatcher
struct Obstacle2DComponent;

class ObstacleRenderUpdater final : public hatcher::RenderUpdater
{
public:
    ObstacleRenderUpdater(const std::unique_ptr<hatcher::MeshBuilder>& meshBuilder);
    ~ObstacleRenderUpdater();

    void Update(const hatcher::ComponentManager* componentManager,
                hatcher::ComponentManager* renderComponentManager, const hatcher::Clock& clock,
                hatcher::IFrameRenderer& frameRenderer) override;

private:
    std::shared_ptr<hatcher::Mesh>
    CreateMeshFromObstacle(const Obstacle2DComponent& obstacleComponent);

    const std::unique_ptr<hatcher::MeshBuilder>& m_meshBuilder;
};

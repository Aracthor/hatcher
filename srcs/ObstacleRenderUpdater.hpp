#pragma once

#include <memory>

#include "hatcher/Graphics/RenderUpdater.hpp"

namespace hatcher
{
class MeshBuilder;
class World;
} // namespace hatcher

class ObstacleRenderUpdater final : public hatcher::RenderUpdater
{
public:
    ObstacleRenderUpdater(const std::unique_ptr<hatcher::MeshBuilder>& meshBuilder);
    ~ObstacleRenderUpdater();

    void Update(const hatcher::ComponentManager* componentManager, const hatcher::Clock& clock,
                hatcher::IFrameRenderer& frameRenderer) override;

private:
    const std::unique_ptr<hatcher::MeshBuilder>& m_meshBuilder;
};

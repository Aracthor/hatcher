#pragma once

#include <memory>

#include "hatcher/Graphics/RenderUpdater.hpp"

namespace hatcher
{
class Clock;
class ComponentManager;
class IFrameRenderer;
class Mesh;
class MeshBuilder;
} // namespace hatcher

class MovingEntitiesRenderUpdater final : public hatcher::RenderUpdater
{
public:
    MovingEntitiesRenderUpdater(const std::unique_ptr<hatcher::MeshBuilder>& meshBuilder);
    ~MovingEntitiesRenderUpdater();

    void Update(const hatcher::ComponentManager* componentManager,
                hatcher::ComponentManager* renderComponentManager, const hatcher::Clock& clock,
                hatcher::IFrameRenderer& frameRenderer) override;

private:
    std::unique_ptr<hatcher::Mesh> m_mesh;
};

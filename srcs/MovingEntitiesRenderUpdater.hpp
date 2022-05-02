#pragma once

#include <memory>

#include "hatcher/Graphics/RenderUpdater.hpp"

namespace hatcher
{
class Clock;
class ComponentManager;
class IFrameRenderer;
class IRendering;
class Mesh;
} // namespace hatcher

class MovingEntitiesRenderUpdater final : public hatcher::RenderUpdater
{
public:
    MovingEntitiesRenderUpdater(const hatcher::IRendering* rendering);
    ~MovingEntitiesRenderUpdater();

    void Update(const hatcher::ComponentManager* componentManager,
                hatcher::ComponentManager* renderComponentManager,
                const hatcher::IRendering& rendering,
                hatcher::IFrameRenderer& frameRenderer) override;

private:
    std::unique_ptr<hatcher::Mesh> m_mesh;
};

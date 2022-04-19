#pragma once

namespace hatcher
{
class ComponentManager;
class IFrameRenderer;
class IRendering;

class RenderUpdater
{
public:
    virtual ~RenderUpdater() = default;

    virtual void Update(const ComponentManager* componentManager,
                        ComponentManager* renderComponentManager, const IRendering& rendering,
                        IFrameRenderer& frameRenderer) = 0;
};

} // namespace hatcher

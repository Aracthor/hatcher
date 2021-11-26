#pragma once

namespace hatcher
{
class ComponentManager;
class IFrameRenderer;

class RenderUpdater
{
public:
    virtual ~RenderUpdater() = default;

    virtual void Update(const ComponentManager* componentManager,
                        IFrameRenderer& frameRenderer) = 0;
};

} // namespace hatcher

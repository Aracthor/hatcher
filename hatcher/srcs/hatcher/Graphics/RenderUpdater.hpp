#pragma once

namespace hatcher
{
class Clock;
class ComponentManager;
class IFrameRenderer;

class RenderUpdater
{
public:
    virtual ~RenderUpdater() = default;

    virtual void Update(const ComponentManager* componentManager, const Clock& clock,
                        IFrameRenderer& frameRenderer) = 0;
};

} // namespace hatcher

#pragma once

namespace hatcher
{
class Clock;
class ComponentManager;
class IFrameRenderer;

class IEventUpdater
{
public:
    virtual ~IEventUpdater() = default;

    virtual void Update(ComponentManager* componentManager, const Clock& clock,
                        IFrameRenderer& frameRenderer) = 0;
};

} // namespace hatcher

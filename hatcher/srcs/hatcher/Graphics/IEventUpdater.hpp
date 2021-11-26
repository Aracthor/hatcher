#pragma once

namespace hatcher
{
class ComponentManager;
class IFrameRenderer;

class IEventUpdater
{
public:
    virtual ~IEventUpdater() = default;

    virtual void Update(ComponentManager* componentManager, IFrameRenderer& frameRenderer) = 0;
};

} // namespace hatcher

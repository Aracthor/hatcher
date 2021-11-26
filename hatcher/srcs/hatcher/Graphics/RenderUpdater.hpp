#pragma once

namespace hatcher
{
class ComponentManager;
class IFrameRenderer;

class RenderUpdater
{
public:
    virtual ~RenderUpdater() {}

    virtual void Update(ComponentManager* componentManager, IFrameRenderer& frameRenderer) = 0;
};

} // namespace hatcher

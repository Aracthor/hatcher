#pragma once

namespace hatcher
{
class ComponentManager;
class IEventUpdater;
class IFrameRenderer;
class IRendering;

class RenderUpdater
{
public:
    virtual ~RenderUpdater() = default;

    virtual void Update(const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                        IFrameRenderer& frameRenderer) = 0;
};

class IRenderUpdaterCreator
{
public:
    virtual RenderUpdater* Create(const IRendering* rendering, IEventUpdater* eventUpdater) const = 0;
};

void RegisterRenderUpdater(const IRenderUpdaterCreator* creator);

template <class RenderUpdaterClass>
class RenderUpdaterRegisterer final : public IRenderUpdaterCreator
{
public:
    RenderUpdaterRegisterer() { RegisterRenderUpdater(this); }

    RenderUpdater* Create(const IRendering* rendering, IEventUpdater* eventUpdater) const override
    {
        return new RenderUpdaterClass(rendering, eventUpdater);
    }
};

} // namespace hatcher

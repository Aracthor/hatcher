#pragma once

namespace hatcher
{
class ComponentManager;
class IApplication;
class IEventUpdater;
class IFrameRenderer;
class IRendering;

class RenderUpdater
{
public:
    virtual ~RenderUpdater() = default;

    virtual void Update(IApplication* application, const ComponentManager* componentManager,
                        ComponentManager* renderComponentManager, IFrameRenderer& frameRenderer) = 0;
};

class IRenderUpdaterCreator
{
public:
    virtual RenderUpdater* Create(const IRendering* rendering) const = 0;
};

void RegisterRenderUpdater(const IRenderUpdaterCreator* creator, int order);

template <class RenderUpdaterClass>
class RenderUpdaterRegisterer final : public IRenderUpdaterCreator
{
public:
    RenderUpdaterRegisterer(int order = 0) { RegisterRenderUpdater(this, order); }

    RenderUpdater* Create(const IRendering* rendering) const override { return new RenderUpdaterClass(rendering); }
};

} // namespace hatcher

#pragma once

#include "IEventListener.hpp"

namespace hatcher
{
class ComponentManager;
class IEventUpdater;
class IFrameRenderer;
class IRendering;

class RenderUpdater : public IEventListener
{
public:
    virtual ~RenderUpdater() = default;

    virtual void Update(const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                        IFrameRenderer& frameRenderer) = 0;

    virtual void GetEvent(const SDL_Event& event, ICommandManager* commandManager,
                          const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                          const IFrameRenderer& frameRenderer) {};
};

class IRenderUpdaterCreator
{
public:
    virtual RenderUpdater* Create(const IRendering* rendering) const = 0;
};

void RegisterRenderUpdater(const IRenderUpdaterCreator* creator);

template <class RenderUpdaterClass>
class RenderUpdaterRegisterer final : public IRenderUpdaterCreator
{
public:
    RenderUpdaterRegisterer() { RegisterRenderUpdater(this); }

    RenderUpdater* Create(const IRendering* rendering) const override { return new RenderUpdaterClass(rendering); }
};

} // namespace hatcher

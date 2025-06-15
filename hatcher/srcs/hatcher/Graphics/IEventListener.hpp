#pragma once

#include <SDL2/SDL_events.h>

namespace hatcher
{
class ComponentManager;
class IApplication;
class ICommandManager;
class IFrameRenderer;

class IEventListener
{
public:
    virtual ~IEventListener() = default;

    virtual void GetEvent(const SDL_Event& event, IApplication* application, ICommandManager* commandManager,
                          const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                          const IFrameRenderer& frameRenderer) = 0;
};

class IEventListenerCreator
{
public:
    virtual IEventListener* Create() const = 0;
};

void RegisterEventListener(const IEventListenerCreator* creator);

template <class EventListenerClass>
class EventListenerRegisterer final : public IEventListenerCreator
{
public:
    EventListenerRegisterer() { RegisterEventListener(this); }

    IEventListener* Create() const override { return new EventListenerClass(); }
};

} // namespace hatcher

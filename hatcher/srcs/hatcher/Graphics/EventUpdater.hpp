#pragma once

#include <map>
#include <memory>
#include <vector>

#include <SDL2/SDL_events.h>

#include "IEventUpdater.hpp"

namespace hatcher
{
class ComponentManager;
class IApplication;
class IEntityManager;
class IEventListener;
class IFrameRenderer;

class EventUpdater final : public IEventUpdater
{
public:
    void PollEvents(IApplication* application, IEntityManager* entityManager,
                    ComponentManager* componentManager, ComponentManager* renderComponentManager,
                    const IFrameRenderer& frameRenderer);

    void RegisterListener(const std::shared_ptr<IEventListener>& eventListener) override;

private:
    std::map<SDL_EventType, std::vector<std::shared_ptr<IEventListener>>> m_eventListeners;
};

} // namespace hatcher

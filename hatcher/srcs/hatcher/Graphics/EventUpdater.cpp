#include "EventUpdater.hpp"

#include "IEventListener.hpp"
#include "hatcher/DataLoader.hpp"
#include "hatcher/DataSaver.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/IEntityManager.hpp"
#include "hatcher/WorldSettings.hpp"

#include <iostream>

namespace hatcher
{

void EventUpdater::ProcessApplicationEvents(span<const SDL_Event> events, WorldSettings& settings,
                                            IEntityManager* entityManager)
{
    for (const SDL_Event& event : events)
    {
        const SDL_EventType eventType = static_cast<SDL_EventType>(event.type);
        if (eventType == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_F5)
        {
            DataSaver saver;
            saver << settings;
            entityManager->Save(saver);
            m_saveState = saver.Result();
            std::cout << "State saved." << std::endl;
        }
        else if (eventType == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_F9)
        {
            if (!m_saveState.empty())
            {
                DataLoader loader(m_saveState);
                loader >> settings;
                entityManager->Load(loader);
                std::cout << "State loaded." << std::endl;
            }
            else
            {
                std::cout << "No state to load." << std::endl;
            }
        }
    }
}

void EventUpdater::ProcessEventListeners(span<const SDL_Event> events, IApplication* application,
                                         ICommandManager* commandManager, const ComponentManager* componentManager,
                                         ComponentManager* renderComponentManager, const IFrameRenderer& frameRenderer)
{
    for (const SDL_Event& event : events)
    {
        for (auto& listener : m_eventListeners)
            listener->GetEvent(event, application, commandManager, componentManager, renderComponentManager,
                               frameRenderer);
    }
}

void EventUpdater::RegisterListener(IEventListener* eventListener)
{
    m_eventListeners.push_back(eventListener);
}

} // namespace hatcher

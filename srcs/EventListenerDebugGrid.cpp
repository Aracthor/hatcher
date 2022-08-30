#include "GridDisplay.hpp"

#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/IEventListener.hpp"
#include "hatcher/assert.hpp"

namespace
{

using namespace hatcher;

class EventListenerDebugGrid final : public IEventListener
{
public:
    void GetEvent(const SDL_Event& event, IApplication* gameApplication,
                  IEntityManager* entityManager, ComponentManager* componentManager,
                  ComponentManager* renderComponentManager) override
    {
        HATCHER_ASSERT(event.type == SDL_KEYDOWN);
        if (event.key.keysym.scancode == SDL_SCANCODE_U)
        {
            GridDisplay* gridDisplay = renderComponentManager->WriteWorldComponent<GridDisplay>();
            gridDisplay->SetEnabled(!gridDisplay->Enabled());
        }
    }

    span<const SDL_EventType> EventTypesToListen() const override
    {
        static const SDL_EventType events[] = {
            SDL_KEYDOWN,
        };
        return span<const SDL_EventType>(events, std::size(events));
    }
};

const int dummy = RegisterEventListener<EventListenerDebugGrid>("DebugGrid");

} // namespace
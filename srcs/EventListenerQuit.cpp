#include "hatcher/Graphics/IEventListener.hpp"
#include "hatcher/IApplication.hpp"

namespace
{

using namespace hatcher;

class EventListenerQuit final : public IEventListener
{
public:
    void GetEvent(const SDL_Event& event, IApplication* gameApplication,
                  IEntityManager* entityManager, ComponentManager* componentManager,
                  ComponentManager* renderComponentManager,
                  const IFrameRenderer& frameRenderer) override
    {
        if (event.type == SDL_QUIT ||
            (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE))
            gameApplication->Stop();
    }

    span<const SDL_EventType> EventTypesToListen() const override
    {
        static const SDL_EventType events[] = {
            SDL_QUIT,
            SDL_KEYDOWN,
        };
        return span<const SDL_EventType>(events, std::size(events));
    }
};

const int dummy = RegisterEventListener<EventListenerQuit>("Quit");

} // namespace
#include "hatcher/Graphics/IEventListener.hpp"
#include "hatcher/IApplication.hpp"

using namespace hatcher;

namespace
{

class PlayerControlUpdater final : public IEventListener
{
public:
    PlayerControlUpdater() {}

    void GetEvent(const SDL_Event& event, IApplication* application, ICommandManager* commandManager,
                  const ComponentAccessor* componentAccessor, ComponentAccessor* renderComponentAccessor,
                  const IFrameRenderer& frameRenderer) override
    {
        if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_R)
            application->Restart();
    }
};

EventListenerRegisterer<PlayerControlUpdater> registerer;
} // namespace

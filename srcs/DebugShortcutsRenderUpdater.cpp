#include "hatcher/ComponentManager.hpp"
#include "hatcher/EntityManager.hpp"
#include "hatcher/Graphics/IEventListener.hpp"
#include "hatcher/Graphics/IEventUpdater.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/assert.hpp"

#include "Selectable2DComponent.hpp"

#include "imgui.h"

using namespace hatcher;

namespace
{

class DebugShortcutsEventListener final : public IEventListener
{
public:
    void GetEvent(const SDL_Event& event, IEntityManager* entityManager,
                  ComponentManager* componentManager, ComponentManager* renderComponentManager,
                  const IFrameRenderer& frameRenderer) override
    {
        HATCHER_ASSERT(event.type == SDL_KEYDOWN);
        if (event.key.keysym.scancode == SDL_SCANCODE_DELETE && event.key.keysym.mod & KMOD_ALT)
        {
            const auto selectableComponents =
                renderComponentManager->ReadComponents<Selectable2DComponent>();

            for (int i = 0; i < renderComponentManager->Count(); i++)
            {
                if (selectableComponents[i] && selectableComponents[i]->selected)
                {
                    // TODO find a better way to convert an int to Entity ?
                    entityManager->DeleteEntity(Entity(i));
                }
            }
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

class DebugShortcutsRenderUpdater final : public RenderUpdater
{
public:
    DebugShortcutsRenderUpdater(const IRendering* rendering, IEventUpdater* eventUpdater)
    {
        eventUpdater->RegisterListener(std::make_shared<DebugShortcutsEventListener>());
    }

    void Update(const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                IFrameRenderer& frameRenderer) override
    {
    }
};

const int dummy = RegisterRenderUpdater<DebugShortcutsRenderUpdater>("DebugShortcuts");

} // namespace

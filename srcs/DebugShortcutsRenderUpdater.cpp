#include "hatcher/ComponentManager.hpp"
#include "hatcher/EntityManager.hpp"
#include "hatcher/Graphics/IEventListener.hpp"
#include "hatcher/Graphics/IEventUpdater.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/ICommand.hpp"
#include "hatcher/ICommandManager.hpp"
#include "hatcher/assert.hpp"

#include "SelectableComponent.hpp"

#include "imgui.h"

using namespace hatcher;

namespace
{

class DeleteEntityCommand final : public ICommand
{
public:
    DeleteEntityCommand(Entity entity)
        : m_entity(entity)
    {
    }

    void Execute(IEntityManager* entityManager, ComponentManager* componentManager,
                 ComponentManager* renderingComponentManager) override
    {
        entityManager->DeleteEntity(m_entity);
    }

private:
    const Entity m_entity;
};

class DebugShortcutsEventListener final : public IEventListener
{
public:
    void GetEvent(const SDL_Event& event, ICommandManager* commandManager, const ComponentManager* componentManager,
                  ComponentManager* renderComponentManager, const IFrameRenderer& frameRenderer) override
    {
        HATCHER_ASSERT(event.type == SDL_KEYDOWN);
        if (event.key.keysym.scancode == SDL_SCANCODE_DELETE && event.key.keysym.mod & KMOD_ALT)
        {
            const auto selectableComponents = renderComponentManager->ReadComponents<SelectableComponent>();

            for (int i = 0; i < renderComponentManager->Count(); i++)
            {
                if (selectableComponents[i] && selectableComponents[i]->selected)
                {
                    // TODO find a better way to convert an int to Entity ?
                    commandManager->AddCommand(new DeleteEntityCommand(Entity(i)));
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

RenderUpdaterRegisterer<DebugShortcutsRenderUpdater> registerer;

} // namespace

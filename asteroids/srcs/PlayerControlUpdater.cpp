#include "hatcher/CommandManager.hpp"
#include "hatcher/ComponentManager.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/assert.hpp"

#include "PlayerComponent.hpp"

using namespace hatcher;

namespace
{

class PlayerControlCommand final : public ICommand
{
    using Functor = std::function<void(PlayerComponent& component)>;

public:
    PlayerControlCommand(Functor functor)
        : m_functor(functor)
    {
    }

    void Execute(IEntityManager* entityManager, ComponentManager* componentManager,
                 ComponentManager* renderingComponentManager) override
    {
        auto playerComponents = componentManager->WriteComponents<PlayerComponent>();
        for (int i = 0; i < componentManager->Count(); i++)
        {
            if (playerComponents[i])
                m_functor(*playerComponents[i]);
        }
    }

private:
    Functor m_functor;
};

class PlayerControlUpdater final : public RenderUpdater
{
public:
    PlayerControlUpdater(const IRendering* rendering) {}

    void Update(const ComponentManager* componentManager, ComponentManager* renderComponentManager,
                IFrameRenderer& frameRenderer) override
    {
    }

    void GetEvent(const SDL_Event& event, ICommandManager* commandManager, const ComponentManager* componentManager,
                  ComponentManager* renderComponentManager, const IFrameRenderer& frameRenderer) override
    {
        HATCHER_ASSERT(event.type == SDL_KEYDOWN || event.type == SDL_KEYUP);
        const bool action = (event.type == SDL_KEYDOWN);
        if (event.key.keysym.scancode == SDL_SCANCODE_LEFT)
        {
            auto functor = [action](PlayerComponent& component) { component.turningLeft = action; };
            commandManager->AddCommand(new PlayerControlCommand(functor));
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT)
        {
            auto functor = [action](PlayerComponent& component) { component.turningRight = action; };
            commandManager->AddCommand(new PlayerControlCommand(functor));
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_UP)
        {
            auto functor = [action](PlayerComponent& component) { component.accelerating = action; };
            commandManager->AddCommand(new PlayerControlCommand(functor));
        }
    }

    span<const SDL_EventType> EventTypesToListen() const override
    {
        static const SDL_EventType events[] = {
            SDL_KEYDOWN,
            SDL_KEYUP,
        };
        return span<const SDL_EventType>(events, std::size(events));
    }
};

RenderUpdaterRegisterer<PlayerControlUpdater> registerer;

} // namespace

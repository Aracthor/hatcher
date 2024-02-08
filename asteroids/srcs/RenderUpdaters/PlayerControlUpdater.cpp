#include "hatcher/CommandManager.hpp"
#include "hatcher/ComponentManager.hpp"
#include "hatcher/EntityDescriptorID.hpp"
#include "hatcher/EntityEgg.hpp"
#include "hatcher/EntityManager.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/assert.hpp"

#include "Components/CollidableComponent.hpp"
#include "Components/PlayerComponent.hpp"
#include "Components/PositionComponent.hpp"
#include "Components/ProjectileComponent.hpp"
#include "Components/ShooterComponent.hpp"

using namespace hatcher;

namespace
{

Entity GetPlayerEntityID(ComponentManager* componentManager)
{
    auto playerComponents = componentManager->WriteComponents<PlayerComponent>();
    for (int i = 0; i < componentManager->Count(); i++)
    {
        if (playerComponents[i])
            return Entity(i);
    }
    return Entity::Invalid();
}

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
        const Entity playerEntity = GetPlayerEntityID(componentManager);
        if (playerEntity != Entity::Invalid())
        {
            PlayerComponent& playerComponent = *componentManager->WriteComponents<PlayerComponent>()[playerEntity];
            m_functor(playerComponent);
        }
    }

private:
    Functor m_functor;
};

class PlayerShootCommand final : public ICommand
{
public:
    void Execute(IEntityManager* entityManager, ComponentManager* componentManager,
                 ComponentManager* renderingComponentManager) override
    {
        const Entity playerEntity = GetPlayerEntityID(componentManager);
        if (playerEntity == Entity::Invalid())
            return;

        const auto& positionComponent = componentManager->ReadComponents<PositionComponent>()[playerEntity];
        auto& shooterComponent = componentManager->WriteComponents<ShooterComponent>()[playerEntity];
        auto& collidableComponent = componentManager->ReadComponents<CollidableComponent>()[playerEntity];
        HATCHER_ASSERT(positionComponent);
        HATCHER_ASSERT(shooterComponent);
        if (shooterComponent->shoots.size() < 4)
        {
            const glm::vec2 direction = {glm::cos(positionComponent->angle), glm::sin(positionComponent->angle)};
            const glm::vec2 start = positionComponent->position + direction * (collidableComponent->size + 3);
            const glm::vec2 startSpeed = positionComponent->speed + direction * 8.f;
            EntityEgg newProjectile = entityManager->CreateNewEntity(EntityDescriptorID::Create("Shoot"));
            newProjectile.GetComponent<ProjectileComponent>()->shooter = playerEntity.ID();
            auto& projectilePositionComponent = newProjectile.GetComponent<PositionComponent>();
            projectilePositionComponent->position = start;
            projectilePositionComponent->speed = startSpeed;
            shooterComponent->shoots.push_back(newProjectile.NewEntityID().ID());
        }
    }
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
        if (event.key.keysym.scancode == SDL_SCANCODE_SPACE && event.type == SDL_KEYDOWN)
        {
            commandManager->AddCommand(new PlayerShootCommand());
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

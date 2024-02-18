#include "hatcher/ComponentManager.hpp"
#include "hatcher/EntityDescriptorID.hpp"
#include "hatcher/EntityEgg.hpp"
#include "hatcher/EntityManager.hpp"
#include "hatcher/Graphics/IFrameRenderer.hpp"
#include "hatcher/Graphics/RenderUpdater.hpp"
#include "hatcher/ICommand.hpp"
#include "hatcher/ICommandManager.hpp"
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

class PlayerTurnLeftCommand final : public ICommand
{
public:
    PlayerTurnLeftCommand(bool action)
        : m_action(action)
    {
    }

    void Execute(IEntityManager* entityManager, ComponentManager* componentManager,
                 ComponentManager* renderingComponentManager) override
    {
        const Entity playerEntity = GetPlayerEntityID(componentManager);
        if (playerEntity != Entity::Invalid())
            componentManager->WriteComponents<PlayerComponent>()[playerEntity]->turningLeft = m_action;
    }

private:
    const bool m_action;
};

class PlayerTurnRightCommand final : public ICommand
{
public:
    PlayerTurnRightCommand(bool action)
        : m_action(action)
    {
    }

    void Execute(IEntityManager* entityManager, ComponentManager* componentManager,
                 ComponentManager* renderingComponentManager) override
    {
        const Entity playerEntity = GetPlayerEntityID(componentManager);
        if (playerEntity != Entity::Invalid())
            componentManager->WriteComponents<PlayerComponent>()[playerEntity]->turningRight = m_action;
    }

private:
    const bool m_action;
};

class PlayerAccelerateCommand final : public ICommand
{
public:
    PlayerAccelerateCommand(bool action)
        : m_action(action)
    {
    }

    void Execute(IEntityManager* entityManager, ComponentManager* componentManager,
                 ComponentManager* renderingComponentManager) override
    {
        const Entity playerEntity = GetPlayerEntityID(componentManager);
        if (playerEntity != Entity::Invalid())
            componentManager->WriteComponents<PlayerComponent>()[playerEntity]->accelerating = m_action;
    }

private:
    const bool m_action;
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
            commandManager->AddCommand(new PlayerTurnLeftCommand(action));
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT)
        {
            commandManager->AddCommand(new PlayerTurnRightCommand(action));
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_UP)
        {
            commandManager->AddCommand(new PlayerAccelerateCommand(action));
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

#include "hatcher/ComponentManager.hpp"
#include "hatcher/EntityDescriptor.hpp"
#include "hatcher/EntityEgg.hpp"
#include "hatcher/IEntityManager.hpp"
#include "hatcher/Updater.hpp"

#include "CollidableComponent.hpp"
#include "MeshComponent.hpp"
#include "PlayerComponent.hpp"
#include "PositionComponent.hpp"
#include "ShooterComponent.hpp"

using namespace hatcher;

namespace
{

class PlayerCreatorUpdater final : public Updater
{
public:
    PlayerCreatorUpdater()
    {
        EntityDescriptorBuilder builder;

        PositionComponent position;
        position.position = {400.f, 300.f};
        position.angle = M_PI / 2.f;
        position.speed = {0.f, 0.f};
        builder.AddComponent<>(position);

        PlayerComponent player;
        player.turningLeft = false;
        player.turningRight = false;
        player.accelerating = false;
        builder.AddComponent<>(player);

        CollidableComponent collidable;
        collidable.size = 24.f;
        builder.AddComponent<>(collidable);

        ShooterComponent shooter;
        builder.AddComponent<>(shooter);

        MeshComponent mesh;
        mesh.ID = MeshComponent::Player;
        builder.AddRenderingComponent<>(mesh);

        m_playerEntityDescriptor = builder.CreateDescriptor();
    }

private:
    void Update(IEntityManager* entityManager, ComponentManager* componentManager) override
    {
        bool hasPlayer = false;
        auto playerComponents = componentManager->ReadComponents<PlayerComponent>();
        for (int i = 0; i < componentManager->Count(); i++)
        {
            if (playerComponents[i])
                hasPlayer = true;
        }

        if (!hasPlayer)
        {
            entityManager->CreateNewEntity(m_playerEntityDescriptor.get());
        }
    }

    unique_ptr<IEntityDescriptor> m_playerEntityDescriptor;
};

UpdaterRegisterer<PlayerCreatorUpdater> registerer;

} // namespace

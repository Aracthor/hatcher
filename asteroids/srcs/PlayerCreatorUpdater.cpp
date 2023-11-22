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
        m_playerEntityDescriptor = CreateEntityDescriptor(
            {
                CollidableComponent{
                    .size = 24.f,
                },
                PlayerComponent{
                    .turningLeft = false,
                    .turningRight = false,
                    .accelerating = false,
                },
                PositionComponent{
                    .position = {400.f, 300.f},
                    .angle = M_PI / 2.f,
                    .speed = {0.f, 0.f},
                },
                ShooterComponent{},
            },
            {
                MeshComponent{
                    .ID = MeshComponent::Player,
                },
            });
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
            if (m_cyclesToWait > 0)
            {
                m_cyclesToWait -= 1;
            }
            else
            {
                m_cyclesToWait = 50;
                entityManager->CreateNewEntity(m_playerEntityDescriptor.get());
            }
        }
    }

    unique_ptr<IEntityDescriptor> m_playerEntityDescriptor;
    int m_cyclesToWait = 0;
};

UpdaterRegisterer<PlayerCreatorUpdater> registerer;

} // namespace

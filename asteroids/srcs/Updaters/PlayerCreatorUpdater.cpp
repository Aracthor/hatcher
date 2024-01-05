#include "hatcher/ComponentManager.hpp"
#include "hatcher/EntityDescriptorID.hpp"
#include "hatcher/EntityEgg.hpp"
#include "hatcher/IEntityManager.hpp"
#include "hatcher/Updater.hpp"

#include "Components/PlayerComponent.hpp"
#include "WorldComponents/Lives.hpp"

using namespace hatcher;

namespace
{

class PlayerCreatorUpdater final : public Updater
{
    void Update(IEntityManager* entityManager, ComponentManager* componentManager) override
    {
        const Lives* lives = componentManager->ReadWorldComponent<Lives>();
        if (lives->remaining == 0)
            return;

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
                entityManager->CreateNewEntity(EntityDescriptorID::Create("Player"));
            }
        }
    }

    int m_cyclesToWait = 0;
};

UpdaterRegisterer<PlayerCreatorUpdater> registerer;

} // namespace

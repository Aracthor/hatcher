#include "hatcher/ComponentAccessor.hpp"
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
    void Update(IEntityManager* entityManager, ComponentAccessor* componentAccessor) override
    {
        Lives* lives = componentAccessor->WriteWorldComponent<Lives>();
        if (lives->remaining == 0)
            return;

        bool hasPlayer = false;
        auto playerComponents = componentAccessor->ReadComponents<PlayerComponent>();
        for (int i = 0; i < componentAccessor->Count(); i++)
        {
            if (playerComponents[i])
                hasPlayer = true;
        }

        if (!hasPlayer)
        {
            if (lives->cyclesToWait > 0)
            {
                lives->cyclesToWait -= 1;
            }
            else
            {
                lives->cyclesToWait = 50;
                entityManager->CreateNewEntity(EntityDescriptorID::Create("Player"));
            }
        }
    }
};

UpdaterRegisterer<PlayerCreatorUpdater> registerer;

} // namespace

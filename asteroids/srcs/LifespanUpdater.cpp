#include "hatcher/ComponentManager.hpp"
#include "hatcher/IEntityManager.hpp"
#include "hatcher/Updater.hpp"

#include "LifespanComponent.hpp"

using namespace hatcher;

namespace
{

class LifespanUpdater final : public Updater
{
    void Update(IEntityManager* entityManager, ComponentManager* componentManager) override
    {
        auto lifespanComponents = componentManager->WriteComponents<LifespanComponent>();
        for (int i = 0; i < componentManager->Count(); i++)
        {
            auto& lifespanComponent = lifespanComponents[i];
            if (lifespanComponent)
            {
                lifespanComponent->duration -= 1;
                if (lifespanComponent->duration <= 0.f)
                    entityManager->DeleteEntity(Entity(i));
            }
        }
    }
};

UpdaterRegisterer<LifespanUpdater> registerer;

} // namespace

#include "hatcher/ComponentAccessor.hpp"
#include "hatcher/IEntityManager.hpp"
#include "hatcher/Updater.hpp"

#include "Components/LifespanComponent.hpp"

using namespace hatcher;

namespace
{

class LifespanUpdater final : public Updater
{
    void Update(WorldSettings& settings, IEntityManager* entityManager, ComponentAccessor* componentAccessor) override
    {
        auto lifespanComponents = componentAccessor->WriteComponents<LifespanComponent>();
        for (int i = 0; i < componentAccessor->Count(); i++)
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

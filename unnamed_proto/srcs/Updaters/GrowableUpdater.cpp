#include "Components/GrowableComponent.hpp"

#include "hatcher/ComponentManager.hpp"
#include "hatcher/Updater.hpp"

using namespace hatcher;

namespace
{

class GrowableUpdater final : public Updater
{
    void Update(WorldSettings& settings, IEntityManager* entityManager, ComponentManager* componentManager) override
    {
        ComponentWriter<GrowableComponent> growables = componentManager->WriteComponents<GrowableComponent>();

        for (int i = 0; i < componentManager->Count(); i++)
        {
            if (growables[i])
            {
                GrowableComponent& growable = *growables[i];
                growable.maturity = std::min(growable.maturity + 1.f / (float)growable.growthTime, 1.f);
            }
        }
    }
};

UpdaterRegisterer<GrowableUpdater> registerer;

} // namespace

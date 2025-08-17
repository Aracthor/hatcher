#include "Components/GrowableComponent.hpp"

#include "hatcher/ComponentAccessor.hpp"
#include "hatcher/Updater.hpp"

using namespace hatcher;

namespace
{

class GrowableUpdater final : public Updater
{
    void Update(WorldSettings& settings, IEntityManager* entityManager, ComponentAccessor* componentAccessor) override
    {
        ComponentWriter<GrowableComponent> growables = componentAccessor->WriteComponents<GrowableComponent>();

        for (int i = 0; i < componentAccessor->Count(); i++)
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

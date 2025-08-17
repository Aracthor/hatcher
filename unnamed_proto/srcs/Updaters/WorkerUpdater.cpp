#include "Components/WorkerComponent.hpp"

#include "hatcher/ComponentAccessor.hpp"
#include "hatcher/IEntityManager.hpp"
#include "hatcher/Updater.hpp"
#include "hatcher/assert.hpp"

#include <functional>

using namespace hatcher;

namespace
{

using Work = std::function<void(IEntityManager* entityManager, ComponentAccessor* componentAccessor,
                                std::optional<Entity> target)>;

void ChopTree(IEntityManager* entityManager, ComponentAccessor* componentAccessor, std::optional<Entity> target)
{
    HATCHER_ASSERT(target);
    entityManager->DeleteEntity(*target);
}

Work works[] = {
    ChopTree,
};

class WorkerUpdater final : public Updater
{
    void Update(WorldSettings& settings, IEntityManager* entityManager, ComponentAccessor* componentAccessor) override
    {
        ComponentWriter<WorkerComponent> workers = componentAccessor->WriteComponents<WorkerComponent>();

        for (int i = 0; i < componentAccessor->Count(); i++)
        {
            if (workers[i] && workers[i]->workIndex)
            {
                WorkerComponent& worker = *workers[i];
                worker.workedTicks += 1;
                if (worker.workedTicks >= worker.workLength)
                {
                    EWork workIndex = *worker.workIndex;
                    works[static_cast<int>(workIndex)](entityManager, componentAccessor, worker.target);
                    worker.workIndex = {};
                }
            }
        }
    }
};

UpdaterRegisterer<WorkerUpdater> registerer;

} // namespace

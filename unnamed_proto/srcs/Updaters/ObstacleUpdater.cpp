#include "Components/ObstacleComponent.hpp"
#include "Components/Position2DComponent.hpp"
#include "WorldComponents/SquareGrid.hpp"

#include "hatcher/ComponentAccessor.hpp"
#include "hatcher/Updater.hpp"
#include "hatcher/assert.hpp"

using namespace hatcher;

namespace
{
class ObstacleUpdater final : public Updater
{
    void Update(WorldSettings& settings, IEntityManager* entityManager, ComponentAccessor* componentAccessor) override
    {
    }

    virtual void OnCreatedEntity(Entity entity, WorldSettings& settings, IEntityManager* entityManager,
                                 ComponentAccessor* componentAccessor)
    {
        const auto obstacle = componentAccessor->ReadComponents<ObstacleComponent>()[entity];
        if (obstacle)
        {
            const glm::vec2 position = componentAccessor->ReadComponents<Position2DComponent>()[entity]->position;
            SquareGrid* grid = componentAccessor->WriteWorldComponent<SquareGrid>();
            HATCHER_ASSERT(grid->GetTileData(position).walkable);
            grid->SetTileWalkable(position, false);
        }
    }

    virtual void OnDeletedEntity(Entity entity, WorldSettings& settings, IEntityManager* entityManager,
                                 ComponentAccessor* componentAccessor)
    {
        const auto obstacle = componentAccessor->ReadComponents<ObstacleComponent>()[entity];
        if (obstacle)
        {
            const glm::vec2 position = componentAccessor->ReadComponents<Position2DComponent>()[entity]->position;
            SquareGrid* grid = componentAccessor->WriteWorldComponent<SquareGrid>();
            HATCHER_ASSERT(!grid->GetTileData(position).walkable);
            grid->SetTileWalkable(position, true);
        }
    }
};

UpdaterRegisterer<ObstacleUpdater> registerer;
} // namespace

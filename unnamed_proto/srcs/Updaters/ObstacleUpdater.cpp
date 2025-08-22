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
                                 ComponentAccessor* componentAccessor) override
    {
        const auto obstacle = componentAccessor->ReadComponents<ObstacleComponent>()[entity];
        if (obstacle)
        {
            const glm::vec2 position = componentAccessor->ReadComponents<Position2DComponent>()[entity]->position;
            SquareGrid* grid = componentAccessor->WriteWorldComponent<SquareGrid>();
            const glm::vec2 positionMin = position + static_cast<glm::vec2>(obstacle->area.Min());
            const glm::vec2 positionMax = position + static_cast<glm::vec2>(obstacle->area.Max());
            for (float y = positionMin.y; y <= positionMax.y; y++)
            {
                for (float x = positionMin.x; x <= positionMax.x; x++)
                {
                    const glm::vec2 tilePosition(x, y);
                    HATCHER_ASSERT(grid->GetTileData(tilePosition).walkable);
                    grid->SetTileWalkable(tilePosition, false);
                }
            }
        }
    }

    virtual void OnDeletedEntity(Entity entity, WorldSettings& settings, IEntityManager* entityManager,
                                 ComponentAccessor* componentAccessor) override
    {
        const auto obstacle = componentAccessor->ReadComponents<ObstacleComponent>()[entity];
        if (obstacle)
        {
            const glm::vec2 position = componentAccessor->ReadComponents<Position2DComponent>()[entity]->position;
            SquareGrid* grid = componentAccessor->WriteWorldComponent<SquareGrid>();
            const glm::vec2 positionMin = position + static_cast<glm::vec2>(obstacle->area.Min());
            const glm::vec2 positionMax = position + static_cast<glm::vec2>(obstacle->area.Max());
            for (float y = positionMin.y; y <= positionMax.y; y++)
            {
                for (float x = positionMin.x; x <= positionMax.x; x++)
                {
                    const glm::vec2 tilePosition(x, y);
                    HATCHER_ASSERT(!grid->GetTileData(tilePosition).walkable);
                    grid->SetTileWalkable(tilePosition, true);
                }
            }
        }
    }
};

UpdaterRegisterer<ObstacleUpdater> registerer;
} // namespace

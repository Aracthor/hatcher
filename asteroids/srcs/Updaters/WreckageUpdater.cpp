#include "hatcher/ComponentAccessor.hpp"
#include "hatcher/IEntityManager.hpp"
#include "hatcher/Updater.hpp"
#include "hatcher/WorldSettings.hpp"

#include "Components/CollidableComponent.hpp"
#include "Components/LifespanComponent.hpp"
#include "Components/PositionComponent.hpp"
#include "Components/WreckageGeneratorComponent.hpp"

using namespace hatcher;

namespace
{
class WreckageUpdater final : public Updater
{
    void Update(WorldSettings& settings, IEntityManager* entityManager, ComponentAccessor* componentAccessor) override
    {
    }

    void OnDeletedEntity(Entity entity, WorldSettings& settings, IEntityManager* entityManager,
                         ComponentAccessor* componentAccessor) override
    {
        auto wreckageGeneratorComponent = componentAccessor->ReadComponents<WreckageGeneratorComponent>()[entity];
        if (wreckageGeneratorComponent)
        {
            RandomGenerator& random = settings.randomGenerator;
            for (int i = 0; i < wreckageGeneratorComponent->count; i++)
            {
                const glm::vec2 position = componentAccessor->ReadComponents<PositionComponent>()[entity]->position;
                const float size = componentAccessor->ReadComponents<CollidableComponent>()[entity]->size;
                EntityEgg wreckage = entityManager->CreateNewEntity(wreckageGeneratorComponent->WreckageID);
                wreckage.GetComponent<PositionComponent>()->position = position;
                wreckage.GetComponent<PositionComponent>()->angle = random.RandomAngle();
                wreckage.GetComponent<PositionComponent>()->speed =
                    random.RandomDirection() * random.RandomFloat(0.5f, 1.f);
                wreckage.GetComponent<LifespanComponent>()->duration = random.RandomInt(0, size * 3);
            }
        }
    }
};

UpdaterRegisterer<WreckageUpdater> registerer;

} // namespace

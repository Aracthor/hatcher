#include "hatcher/ComponentAccessor.hpp"
#include "hatcher/IEntityManager.hpp"
#include "hatcher/Updater.hpp"

#include "Components/CollidableComponent.hpp"
#include "Components/LifespanComponent.hpp"
#include "Components/PositionComponent.hpp"
#include "Components/WreckageGeneratorComponent.hpp"
#include "WorldComponents/RandomDirector.hpp"

using namespace hatcher;

namespace
{
class WreckageUpdater final : public Updater
{
    void Update(IEntityManager* entityManager, ComponentAccessor* componentAccessor) override {}

    void OnDeletedEntity(Entity entity, IEntityManager* entityManager, ComponentAccessor* componentAccessor) override
    {
        auto wreckageGeneratorComponent = componentAccessor->ReadComponents<WreckageGeneratorComponent>()[entity];
        if (wreckageGeneratorComponent)
        {
            RandomDirector* randomDirector = componentAccessor->WriteWorldComponent<RandomDirector>();
            for (int i = 0; i < wreckageGeneratorComponent->count; i++)
            {
                const glm::vec2 position = componentAccessor->ReadComponents<PositionComponent>()[entity]->position;
                const float size = componentAccessor->ReadComponents<CollidableComponent>()[entity]->size;
                EntityEgg wreckage = entityManager->CreateNewEntity(wreckageGeneratorComponent->WreckageID);
                wreckage.GetComponent<PositionComponent>()->position = position;
                wreckage.GetComponent<PositionComponent>()->angle = randomDirector->RandomAngle();
                wreckage.GetComponent<PositionComponent>()->speed = randomDirector->RandomDirection(0.5f, 1.f);
                wreckage.GetComponent<LifespanComponent>()->duration = randomDirector->RandomInt(0, size * 3);
            }
        }
    }
};

UpdaterRegisterer<WreckageUpdater> registerer;

} // namespace

#include "hatcher/ComponentManager.hpp"
#include "hatcher/IEntityManager.hpp"
#include "hatcher/Updater.hpp"

#include "CollidableComponent.hpp"
#include "LifespanComponent.hpp"
#include "PositionComponent.hpp"
#include "RandomDirector.hpp"
#include "WreckageGeneratorComponent.hpp"

using namespace hatcher;

namespace
{
class WreckageUpdater final : public Updater
{
    void Update(IEntityManager* entityManager, ComponentManager* componentManager) override {}

    void OnDeletedEntity(Entity entity, IEntityManager* entityManager, ComponentManager* componentManager) override
    {
        auto wreckageGeneratorComponent = componentManager->ReadComponents<WreckageGeneratorComponent>()[entity];
        if (wreckageGeneratorComponent)
        {
            RandomDirector* randomDirector = componentManager->WriteWorldComponent<RandomDirector>();
            for (int i = 0; i < wreckageGeneratorComponent->count; i++)
            {
                const glm::vec2 position = componentManager->ReadComponents<PositionComponent>()[entity]->position;
                const float size = componentManager->ReadComponents<CollidableComponent>()[entity]->size;
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

#include "hatcher/Updater.hpp"

class MovingEntitiesUpdater final : public hatcher::Updater
{
public:
    MovingEntitiesUpdater();
    ~MovingEntitiesUpdater();

    void Update(hatcher::ComponentManager* componentManager) override;
};

#include "PlayerComponent.hpp"

#include "hatcher/ComponentRegisterer.hpp"
#include "hatcher/ISaveLoader.hpp"

void operator<<(ISaveLoader& saveLoader, PlayerComponent& component)
{
    saveLoader << component.turningLeft;
    saveLoader << component.turningRight;
    saveLoader << component.accelerating;
}

namespace
{
ComponentTypeRegisterer<PlayerComponent, EComponentList::Gameplay> registerer;
} // namespace

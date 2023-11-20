#include "ProjectileComponent.hpp"

#include "hatcher/ComponentRegisterer.hpp"
#include "hatcher/ISaveLoader.hpp"

using namespace hatcher;

void operator<<(ISaveLoader& saveLoader, ProjectileComponent& component)
{
    saveLoader << component.shooter;
}

namespace
{
ComponentTypeRegisterer<ProjectileComponent, EComponentList::Gameplay> registerer;
} // namespace

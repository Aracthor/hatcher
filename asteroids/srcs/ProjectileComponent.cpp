#include "ProjectileComponent.hpp"

#include "hatcher/ComponentRegisterer.hpp"

using namespace hatcher;

namespace
{
ComponentTypeRegisterer<ProjectileComponent, EComponentList::Gameplay> registerer;
} // namespace

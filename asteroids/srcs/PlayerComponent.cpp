#include "PlayerComponent.hpp"

#include "hatcher/ComponentRegisterer.hpp"

using namespace hatcher;

namespace
{
ComponentTypeRegisterer<PlayerComponent, EComponentList::Gameplay> registerer;
} // namespace

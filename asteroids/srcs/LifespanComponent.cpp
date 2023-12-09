#include "LifespanComponent.hpp"

#include "hatcher/ComponentRegisterer.hpp"

using namespace hatcher;

namespace
{
ComponentTypeRegisterer<LifespanComponent, EComponentList::Gameplay> registerer;
} // namespace

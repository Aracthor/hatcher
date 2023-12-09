#include "SelectableComponent.hpp"

#include "hatcher/ComponentRegisterer.hpp"

using namespace hatcher;

namespace
{
ComponentTypeRegisterer<SelectableComponent, EComponentList::Rendering> registerer;
} // namespace

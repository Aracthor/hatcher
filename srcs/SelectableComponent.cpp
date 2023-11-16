#include "SelectableComponent.hpp"

#include "hatcher/ComponentRegisterer.hpp"
#include "hatcher/ISaveLoader.hpp"

void operator<<(ISaveLoader& saveLoader, SelectableComponent& component)
{
    saveLoader << component.box;
    saveLoader << component.selected;
}

namespace
{
ComponentTypeRegisterer<SelectableComponent, EComponentList::Rendering> registerer;
} // namespace

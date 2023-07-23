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
RenderComponentTypeRegisterer<SelectableComponent> registerer;
} // namespace

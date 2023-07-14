#include "SelectableComponent.hpp"

#include "hatcher/ISaveLoader.hpp"

void operator<<(ISaveLoader& saveLoader, SelectableComponent& component)
{
    saveLoader << component.box;
    saveLoader << component.selected;
}

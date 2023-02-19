#include "Selectable2DComponent.hpp"

#include "hatcher/ISaveLoader.hpp"

void operator<<(ISaveLoader& saveLoader, Selectable2DComponent& component)
{
    saveLoader << component.box;
    saveLoader << component.selected;
}

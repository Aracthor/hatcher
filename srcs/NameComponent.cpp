#include "NameComponent.hpp"

#include "hatcher/ComponentRegisterer.hpp"
#include "hatcher/ISaveLoader.hpp"

void operator<<(ISaveLoader& saveLoader, NameComponent& component)
{
    saveLoader << component.name;
}

namespace
{
ComponentTypeRegisterer<NameComponent, EComponentList::Gameplay> registerer;
} // namespace

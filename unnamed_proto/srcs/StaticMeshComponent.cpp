#include "StaticMeshComponent.hpp"

#include "hatcher/ComponentRegisterer.hpp"
#include "hatcher/ISaveLoader.hpp"

void operator<<(ISaveLoader& saveLoader, StaticMeshComponent& component)
{
    saveLoader << component.type;
}

namespace
{
ComponentTypeRegisterer<StaticMeshComponent, EComponentList::Rendering> registerer;
} // namespace

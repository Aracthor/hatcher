#include "MeshComponent.hpp"

#include "hatcher/ComponentRegisterer.hpp"
#include "hatcher/ISaveLoader.hpp"

using namespace hatcher;

void operator<<(ISaveLoader& saveLoader, MeshComponent& component)
{
    saveLoader << component.ID;
}

namespace
{
ComponentTypeRegisterer<MeshComponent, EComponentList::Rendering> registerer;
} // namespace

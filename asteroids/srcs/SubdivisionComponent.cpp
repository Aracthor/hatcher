#include "SubdivisionComponent.hpp"

#include "hatcher/ComponentRegisterer.hpp"
#include "hatcher/ISaveLoader.hpp"

using namespace hatcher;

void operator<<(ISaveLoader& saveLoader, SubdivisionComponent& component)
{
    saveLoader << component.SubdivisionID;
    saveLoader << component.count;
}

namespace
{
ComponentTypeRegisterer<SubdivisionComponent, EComponentList::Gameplay> registerer;
} // namespace

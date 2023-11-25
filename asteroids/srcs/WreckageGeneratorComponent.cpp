#include "WreckageGeneratorComponent.hpp"

#include "hatcher/ComponentRegisterer.hpp"
#include "hatcher/ISaveLoader.hpp"

using namespace hatcher;

void operator<<(ISaveLoader& saveLoader, WreckageGeneratorComponent& component)
{
    saveLoader << component.WreckageID;
    saveLoader << component.count;
}

namespace
{
ComponentTypeRegisterer<WreckageGeneratorComponent, EComponentList::Gameplay> registerer;
} // namespace

#include "ShooterComponent.hpp"

#include "hatcher/ComponentRegisterer.hpp"
#include "hatcher/ISaveLoader.hpp"

using namespace hatcher;

void operator<<(ISaveLoader& saveLoader, ShooterComponent& component)
{
    saveLoader << component.shoots;
}

namespace
{
ComponentTypeRegisterer<ShooterComponent, EComponentList::Gameplay> registerer;
} // namespace

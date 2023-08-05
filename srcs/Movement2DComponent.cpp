#include "Movement2DComponent.hpp"

#include "hatcher/ComponentRegisterer.hpp"
#include "hatcher/ISaveLoader.hpp"

void operator<<(ISaveLoader& saveLoader, Movement2DComponent& component)
{
    saveLoader << component.path;
}

namespace
{
ComponentTypeRegisterer<Movement2DComponent> registerer;
} // namespace

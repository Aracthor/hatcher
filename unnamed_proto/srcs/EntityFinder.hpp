#pragma once

#include <functional>

#include "hatcher/Entity.hpp"

namespace hatcher
{
class ComponentManager;
} // namespace hatcher

using namespace hatcher;

Entity FindNearestEntity(const ComponentManager* componentManager, Entity sourceEntity,
                         std::function<bool(const ComponentManager*, Entity entity)> pred);

#pragma once

#include <functional>

#include "hatcher/Entity.hpp"

namespace hatcher
{
class ComponentAccessor;
} // namespace hatcher

using namespace hatcher;

Entity FindNearestEntity(const ComponentAccessor* componentAccessor, Entity sourceEntity,
                         std::function<bool(const ComponentAccessor*, Entity entity)> pred);

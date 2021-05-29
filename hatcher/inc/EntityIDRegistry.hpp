#pragma once

#include "Entity.hpp"

namespace hatcher
{

class EntityIDRegistry
{
  private:
    using TUnusedEntityIdStack = std::stack<Entity::IdType, std::vector<Entity::IdType>>;
};

} // namespace hatcher

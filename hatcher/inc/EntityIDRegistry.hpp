#pragma once

#include "Entity.hpp"

namespace hatcher
{

class EntityIDRegistry
{
  private:
    using TUnusedEntityIDStack = std::stack<Entity::IDType, std::vector<Entity::IDType>>;
};

} // namespace hatcher

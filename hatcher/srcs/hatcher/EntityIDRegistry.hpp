#pragma once

#include <stack>
#include <vector>

#include "Entity.hpp"

namespace hatcher
{

class EntityIDRegistry
{
private:
    using EntityID = Entity::IDType;
    using UnusedEntityIDStack = std::stack<EntityID, std::vector<EntityID>>;

    UnusedEntityIDStack m_unusedEntityIDStack;
    EntityID m_currentLargestEntityID = 0;

public:
    EntityID GetNewID();
    void UnregisterEntityID(EntityID entityID);
};

} // namespace hatcher

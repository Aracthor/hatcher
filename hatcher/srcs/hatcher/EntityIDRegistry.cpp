#include <stack>
#include <vector>

#include "EntityIDRegistry.hpp"

namespace hatcher
{

EntityIDRegistry::EntityID EntityIDRegistry::GetNewID()
{
    if (!m_unusedEntityIDStack.empty())
    {
        EntityID unusedEntityID = m_unusedEntityIDStack.top();
        m_unusedEntityIDStack.pop();
        return unusedEntityID;
    }

    return m_currentLargestEntityID++;
}

int EntityIDRegistry::EntityCount() const
{
    return m_currentLargestEntityID - m_unusedEntityIDStack.size();
}

void EntityIDRegistry::ResetEntityCount(int count)
{
    m_currentLargestEntityID = count;
    m_unusedEntityIDStack = {};
}

void EntityIDRegistry::UnregisterEntityID(EntityID entityID)
{
    m_unusedEntityIDStack.push(entityID);
}

} // namespace hatcher

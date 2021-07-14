#include <stack>
#include <vector>

#include "EntityIDRegistry.hpp"

namespace hatcher
{

EntityIDRegistry::EntityID EntityIDRegistry::getNewID()
{
    if (!m_unusedEntityIDStack.empty())
    {
        EntityID unusedEntityID = m_unusedEntityIDStack.top();
        m_unusedEntityIDStack.pop();
        return unusedEntityID;
    }

    m_currentLargestEntityID++;
    return m_currentLargestEntityID;
}

void EntityIDRegistry::unregisterEntityID(EntityID entityID)
{
    m_unusedEntityIDStack.push(entityID);
}

} // namespace hatcher

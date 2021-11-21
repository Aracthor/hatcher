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

void EntityIDRegistry::UnregisterEntityID(EntityID entityID)
{
    m_unusedEntityIDStack.push(entityID);
}

} // namespace hatcher

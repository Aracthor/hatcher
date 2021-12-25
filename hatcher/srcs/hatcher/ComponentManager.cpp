#include "ComponentManager.hpp"

#include "ComponentList.hpp"

namespace hatcher
{

ComponentManager::ComponentManager() = default;

ComponentManager::~ComponentManager()
{
    // TODO it would be smart to use smart pointers here...
    for (auto it : m_componentLists)
    {
        delete it.second;
    }
}

void ComponentManager::AddEntities(int count)
{
    for (auto it : m_componentLists)
    {
        it.second->AddEntities(count);
    }
}

} // namespace hatcher

#include "ComponentManager.hpp"

#include "ComponentList.hpp"

namespace hatcher
{

ComponentManager::ComponentManager() = default;

ComponentManager::~ComponentManager() = default;

void ComponentManager::AddEntities(int count)
{
    for (auto& it : m_componentLists)
    {
        it.second->AddEntities(count);
    }
}

} // namespace hatcher

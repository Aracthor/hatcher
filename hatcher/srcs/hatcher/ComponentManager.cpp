#include "ComponentManager.hpp"

#include "ComponentList.hpp"
#include "IWorldComponent.hpp"

namespace hatcher
{

ComponentManager::ComponentManager() = default;

ComponentManager::~ComponentManager() = default;

void ComponentManager::AddEntities(int count)
{
    m_entityCount += count;
    for (auto& it : m_componentLists)
    {
        it.second->AddEntities(count);
    }
}

void ComponentManager::SaveLoad(ISaveLoader& saveLoader)
{
    for (auto& worldComponent : m_worldComponents)
    {
        worldComponent.second->SaveLoad(saveLoader);
    }
}

} // namespace hatcher

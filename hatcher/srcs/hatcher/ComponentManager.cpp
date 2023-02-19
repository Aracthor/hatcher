#include "ComponentManager.hpp"

#include "ComponentList.hpp"
#include "ISaveLoader.hpp"
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

void ComponentManager::ClearEntities()
{
    m_entityCount = 0;
    for (auto& it : m_componentLists)
    {
        it.second->ClearEntities();
    }
}

void ComponentManager::Save(ISaveLoader& saveLoader)
{
    for (auto& worldComponent : m_worldComponents)
    {
        worldComponent.second->SaveLoad(saveLoader);
    }

    for (int entityID = 0; entityID < m_entityCount; entityID++)
    {
        int componentCount = 0;
        for (auto& componentList : m_componentLists)
        {
            componentCount += componentList.second->HasComponent(entityID);
        }
        saveLoader << componentCount;
        saveLoader << '\n';
        for (auto& componentList : m_componentLists)
        {
            if (componentList.second->HasComponent(entityID))
            {
                uint componentID = componentList.first;
                saveLoader << componentID;
                saveLoader << '\n';
                componentList.second->SaveLoad(entityID, saveLoader);
                saveLoader << '\n';
            }
        }
    }
}

void ComponentManager::Load(ISaveLoader& saveLoader)
{
    for (auto& worldComponent : m_worldComponents)
    {
        worldComponent.second->SaveLoad(saveLoader);
    }

    for (int entityID = 0; entityID < m_entityCount; entityID++)
    {
        int componentCount;
        saveLoader << componentCount;
        saveLoader << '\n';
        for (int i = 0; i < componentCount; i++)
        {
            uint componentID;
            saveLoader << componentID;
            saveLoader << '\n';
            m_componentLists[componentID]->SaveLoad(entityID, saveLoader);
            saveLoader << '\n';
        }
    }
}

} // namespace hatcher

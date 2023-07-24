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

void ComponentManager::RemoveEntity(Entity entity)
{
    for (auto& it : m_componentLists)
    {
        it.second->RemoveEntity(entity.ID());
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
        saveLoader.separator('\n');
        for (auto& componentList : m_componentLists)
        {
            if (componentList.second->HasComponent(entityID))
            {
                uint componentID = componentList.first;
                saveLoader << componentID;
                saveLoader.separator('\n');
                componentList.second->SaveLoad(entityID, saveLoader);
                saveLoader.separator('\n');
            }
        }
    }
}

void ComponentManager::Load(ISaveLoader& saveLoader)
{
    for (auto& worldComponent : m_worldComponents)
    {
        worldComponent.second->SaveLoad(saveLoader);
        worldComponent.second->PostLoad();
    }

    for (int entityID = 0; entityID < m_entityCount; entityID++)
    {
        int componentCount;
        saveLoader << componentCount;
        saveLoader.separator('\n');
        for (int i = 0; i < componentCount; i++)
        {
            uint componentID;
            saveLoader << componentID;
            saveLoader.separator('\n');
            m_componentLists[componentID]->SaveLoad(entityID, saveLoader);
            saveLoader.separator('\n');
        }
    }
}

} // namespace hatcher

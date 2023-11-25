#include "ComponentManager.hpp"

#include "ComponentList.hpp"
#include "ComponentLoader.hpp"
#include "ComponentSaver.hpp"
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

void ComponentManager::CopyEntity(const ComponentManager* other, Entity entityDest, Entity entitySrc)
{
    for (auto& it : m_componentLists)
    {
        it.second->CopyEntity(other->m_componentLists.at(it.first).get(), entityDest.ID(), entitySrc.ID());
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

void ComponentManager::Save(ComponentSaver& saver)
{
    for (auto& worldComponent : m_worldComponents)
    {
        worldComponent.second->SaveLoad(saver);
    }

    for (int entityID = 0; entityID < m_entityCount; entityID++)
    {
        int componentCount = 0;
        for (auto& componentList : m_componentLists)
        {
            componentCount += componentList.second->HasComponent(entityID);
        }
        saver << componentCount;
        for (auto& componentList : m_componentLists)
        {
            if (componentList.second->HasComponent(entityID))
            {
                uint componentID = componentList.first;
                saver << componentID;
                componentList.second->SaveLoad(entityID, saver);
            }
        }
    }
}

void ComponentManager::Load(ComponentLoader& loader)
{
    for (auto& worldComponent : m_worldComponents)
    {
        worldComponent.second->SaveLoad(loader);
        worldComponent.second->PostLoad();
    }

    for (int entityID = 0; entityID < m_entityCount; entityID++)
    {
        LoadEntityComponents(loader, entityID);
    }
}

void ComponentManager::LoadEntityComponents(ComponentLoader& loader, int entityID)
{
    int componentCount;
    loader << componentCount;
    for (int i = 0; i < componentCount; i++)
    {
        uint componentID;
        loader << componentID;
        HATCHER_ASSERT(m_componentLists.find(componentID) != m_componentLists.end());
        m_componentLists[componentID]->CreateComponent(entityID);
        m_componentLists[componentID]->SaveLoad(entityID, loader);
    }
}

} // namespace hatcher

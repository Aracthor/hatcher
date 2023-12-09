#include "EntityManager.hpp"

#include "ComponentLoader.hpp"
#include "ComponentManager.hpp"
#include "ComponentSaver.hpp"
#include "Entity.hpp"
#include "EntityDescriptor.hpp"
#include "EntityDescriptorCatalog.hpp"
#include "EntityEgg.hpp"
#include "EntityIDRegistry.hpp"

#include <algorithm> // std::find

namespace hatcher
{

void ResizeComponentManagerIfNecessary(int newSize, int& currentSize, ComponentManager* componentManager,
                                       ComponentManager* renderingComponentManager)
{
    if (newSize > currentSize)
    {
        const int entitiesAdded = newSize - currentSize;
        componentManager->AddEntities(entitiesAdded);
        if (renderingComponentManager)
            renderingComponentManager->AddEntities(entitiesAdded);
        currentSize = newSize;
    }
}

EntityManager::EntityManager(const EntityDescriptorCatalog* descriptorCatalog)
    : m_descriptorCatalog(descriptorCatalog)
{
    m_entityIDRegistry = make_unique<EntityIDRegistry>();

    m_componentManager = make_unique<ComponentManager>();
    m_renderingComponentManager = make_unique<ComponentManager>();

    m_temporaryComponentManager = make_unique<ComponentManager>();
    m_temporaryRenderingComponentManager = make_unique<ComponentManager>();
}

EntityManager::~EntityManager() = default;

void EntityManager::UpdateNewAndDeletedEntities()
{
    for (int i = 0; i < (int)m_entitiesToAdd.size(); i++)
    {
        const Entity temporaryEntity = Entity(i);
        const Entity newEntity = m_entitiesToAdd[i];
        ResizeComponentManagerIfNecessary(newEntity.ID() + 1, m_maxEntityCount, m_componentManager.get(),
                                          m_renderingComponentManager.get());
        m_componentManager->CopyEntity(m_temporaryComponentManager.get(), newEntity, temporaryEntity);
        if (m_renderingComponentManager)
            m_renderingComponentManager->CopyEntity(m_temporaryRenderingComponentManager.get(), newEntity,
                                                    temporaryEntity);

        m_temporaryComponentManager->RemoveEntity(temporaryEntity);
        if (m_temporaryRenderingComponentManager)
            m_temporaryRenderingComponentManager->RemoveEntity(temporaryEntity);
    }
    m_entitiesToAdd.clear();

    for (Entity entity : m_entitiesToDelete)
    {
        m_componentManager->RemoveEntity(entity);
        if (m_renderingComponentManager)
            m_renderingComponentManager->RemoveEntity(entity);
        m_entityIDRegistry->UnregisterEntityID(entity.ID());
    }
    m_entitiesToDelete.clear();
}

Entity EntityManager::CreateNewEntity()
{
    Entity entity = Entity(m_entityIDRegistry->GetNewID());
    m_entitiesToAdd.push_back(entity);
    ResizeComponentManagerIfNecessary(m_entitiesToAdd.size(), m_maxTemporaryEntityCount,
                                      m_temporaryComponentManager.get(), m_temporaryRenderingComponentManager.get());
    return entity;
}

EntityEgg EntityManager::CreateNewEntity(EntityDescriptorID id)
{
    const Entity newEntity = CreateNewEntity();
    const Entity::IDType temporaryID = m_entitiesToAdd.size() - 1;
    const IEntityDescriptor* descriptor = m_descriptorCatalog->GetDescriptor(id);
    HATCHER_ASSERT(descriptor != nullptr);
    ComponentLoader loader = ComponentLoader(descriptor->GetComponentData());
    m_temporaryComponentManager->LoadEntityComponents(loader, temporaryID);
    if (m_temporaryRenderingComponentManager)
    {
        ComponentLoader renderingLoader = ComponentLoader(descriptor->GetRenderingComponentData());
        m_temporaryRenderingComponentManager->LoadEntityComponents(renderingLoader, temporaryID);
    }

    return EntityEgg(newEntity, Entity(temporaryID), m_temporaryComponentManager.get(),
                     m_temporaryRenderingComponentManager.get());
}

void EntityManager::DeleteEntity(Entity entity)
{
    if (std::find(m_entitiesToDelete.begin(), m_entitiesToDelete.end(), entity) == m_entitiesToDelete.end())
        m_entitiesToDelete.push_back(entity);
}

void EntityManager::Save(ComponentSaver& saver)
{
    saver << m_maxEntityCount;
    m_componentManager->Save(saver);
    m_renderingComponentManager->Save(saver);
}

void EntityManager::Load(ComponentLoader& loader)
{
    loader << m_maxEntityCount;
    m_entityIDRegistry->ResetEntityCount(m_maxEntityCount);
    m_componentManager->ClearEntities();
    m_componentManager->AddEntities(m_maxEntityCount);
    m_componentManager->Load(loader);
    m_renderingComponentManager->ClearEntities();
    m_renderingComponentManager->AddEntities(m_maxEntityCount);
    m_renderingComponentManager->Load(loader);
}

} // namespace hatcher

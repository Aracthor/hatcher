#pragma once

#include <optional>
#include <unordered_map>

#include "Entity.hpp"
#include "basic_types.hpp"
#include "unique_ptr.hpp"

namespace hatcher
{
class DataLoader;
class DataSaver;
class IComponentList;
class IWorldComponent;

class ComponentManager
{
public:
    ComponentManager();
    ~ComponentManager();

    void AddEntities(int count);
    void CopyEntity(const ComponentManager* other, Entity entityDest, Entity entitySrc);
    void RemoveEntity(Entity entity);
    void ClearEntities();

    void Save(DataSaver& saver) const;
    void Load(DataLoader& loader);
    void LoadEntityComponents(DataLoader& loader, int entityID);

    int Count() const { return m_entityCount; }

    template <class Component>
    void AddComponentType();

    template <class Component>
    void AddWorldComponent(int64_t seed);

    template <class Component>
    const IComponentList* GetComponentList() const;
    template <class Component>
    IComponentList* GetComponentList();

    template <class Component>
    const IWorldComponent* GetWorldComponent() const;
    template <class Component>
    IWorldComponent* GetWorldComponent();

private:
    int m_entityCount = 0;
    std::unordered_map<uint, unique_ptr<IComponentList>> m_componentLists;
    std::unordered_map<uint, unique_ptr<IWorldComponent>> m_worldComponents;
};

} // namespace hatcher

#include "ComponentManager.ipp"

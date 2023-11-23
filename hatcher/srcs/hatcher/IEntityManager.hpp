#pragma once

#include "Entity.hpp"
#include "EntityEgg.hpp"

namespace hatcher
{
class ComponentLoader;
class ComponentSaver;
class EntityDescriptorID;
class EntityEgg;
class IEntityDescriptor;

class IEntityManager
{
public:
    virtual ~IEntityManager() = default;

    virtual EntityEgg CreateNewEntity(EntityDescriptorID id) = 0;
    virtual EntityEgg CloneEntity(Entity entity) = 0;
    virtual void DeleteEntity(Entity entity) = 0;

    virtual void Save(ComponentSaver& saver) = 0;
    virtual void Load(ComponentLoader& loader) = 0;
};

} // namespace hatcher

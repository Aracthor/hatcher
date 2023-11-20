#pragma once

#include "Entity.hpp"

namespace hatcher
{
class ComponentLoader;
class ComponentSaver;
class EntityEgg;
class IEntityDescriptor;

class IEntityManager
{
public:
    virtual ~IEntityManager() = default;

    virtual EntityEgg CreateNewEntity(const IEntityDescriptor* descriptor) = 0;
    virtual EntityEgg CloneEntity(Entity entity) = 0;
    virtual void DeleteEntity(Entity entity) = 0;

    virtual void Save(ComponentSaver& saver) = 0;
    virtual void Load(ComponentLoader& loader) = 0;
};

} // namespace hatcher

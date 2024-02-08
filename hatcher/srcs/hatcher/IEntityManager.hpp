#pragma once

#include "Entity.hpp"
#include "EntityEgg.hpp"

namespace hatcher
{
class DataLoader;
class DataSaver;
class EntityDescriptorID;
class EntityEgg;
class IEntityDescriptor;

class IEntityManager
{
public:
    virtual ~IEntityManager() = default;

    virtual EntityEgg CreateNewEntity(EntityDescriptorID id) = 0;
    virtual void DeleteEntity(Entity entity) = 0;

    virtual void Save(DataSaver& saver) const = 0;
    virtual void Load(DataLoader& loader) = 0;
};

} // namespace hatcher

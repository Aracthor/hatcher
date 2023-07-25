#pragma once

#include "Entity.hpp"

namespace hatcher
{
class ComponentLoader;
class ComponentSaver;

class IEntityManager
{
public:
    virtual ~IEntityManager() = default;

    virtual Entity CreateNewEntity() = 0;
    virtual void DeleteEntity(Entity entity) = 0;
    virtual bool IsEntityDeleted(Entity entity) const = 0;

    virtual void Save(ComponentSaver& saver) = 0;
    virtual void Load(ComponentLoader& loader) = 0;
};

} // namespace hatcher

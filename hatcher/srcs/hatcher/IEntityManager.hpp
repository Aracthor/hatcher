#pragma once

#include "Entity.hpp"

namespace hatcher
{
class ISaveLoader;

class IEntityManager
{
public:
    virtual ~IEntityManager() = default;

    virtual Entity CreateNewEntity() = 0;
    virtual void DeleteEntity(Entity entity) = 0;
    virtual bool IsEntityDeleted(Entity entity) const = 0;

    virtual void Save(ISaveLoader& saveLoader) = 0;
    virtual void Load(ISaveLoader& saveLoader) = 0;
};

} // namespace hatcher

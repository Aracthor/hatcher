#pragma once

#include "Entity.hpp"

namespace hatcher
{
class ComponentAccessor;
class IEntityManager;

class Updater
{
public:
    virtual ~Updater() {}

    virtual void Update(IEntityManager* entityManager, ComponentAccessor* componentAccessor) = 0;

    virtual void OnCreatedEntity(Entity entity, IEntityManager* entityManager, ComponentAccessor* componentAccessor) {}
    virtual void OnDeletedEntity(Entity entity, IEntityManager* entityManager, ComponentAccessor* componentAccessor) {}
};

class IUpdaterCreator
{
public:
    virtual Updater* Create() const = 0;
};

void RegisterUpdater(const IUpdaterCreator* creator);

template <class UpdaterClass>
class UpdaterRegisterer final : public IUpdaterCreator
{
public:
    UpdaterRegisterer() { RegisterUpdater(this); }

    Updater* Create() const override { return new UpdaterClass(); }
};

} // namespace hatcher

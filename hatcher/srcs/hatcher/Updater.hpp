#pragma once

namespace hatcher
{
class ComponentManager;
class Entity;
class IEntityManager;

class Updater
{
public:
    virtual ~Updater() {}

    virtual void Update(IEntityManager* entityManager, ComponentManager* componentManager) = 0;
    virtual void OnDeletedEntity(Entity entity, ComponentManager* componentManager) {}
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

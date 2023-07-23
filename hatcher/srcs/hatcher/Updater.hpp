#pragma once

namespace hatcher
{
class ComponentManager;

class Updater
{
public:
    virtual ~Updater() {}

    virtual void Update(ComponentManager* componentManager) = 0;
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

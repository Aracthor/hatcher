#pragma once

namespace hatcher
{
class ComponentAccessor;
class IApplication;
class ICommandManager;

class InterfaceUpdater
{
public:
    virtual ~InterfaceUpdater() = default;

    virtual void Update(IApplication* application, ICommandManager* commandManager,
                        const ComponentAccessor* componentAccessor, ComponentAccessor* renderComponentAccessor) = 0;
};

class IInterfaceUpdaterCreator
{
public:
    virtual InterfaceUpdater* Create() const = 0;
};

void RegisterInterfaceUpdater(const IInterfaceUpdaterCreator* creator);

template <class InterfaceUpdaterClass>
class InterfaceUpdaterRegisterer final : public IInterfaceUpdaterCreator
{
public:
    InterfaceUpdaterRegisterer() { RegisterInterfaceUpdater(this); }

    InterfaceUpdater* Create() const override { return new InterfaceUpdaterClass(); }
};

} // namespace hatcher

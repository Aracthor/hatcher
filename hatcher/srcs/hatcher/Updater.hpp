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

using CreateUpdaterFunction = Updater*();
template <class UpdaterClass>
int RegisterUpdater()
{
    int RegisterUpdater(CreateUpdaterFunction * createFunction);
    return RegisterUpdater([]() -> Updater* { return new UpdaterClass(); });
}

} // namespace hatcher

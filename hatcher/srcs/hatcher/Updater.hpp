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
int RegisterUpdater(const char* name)
{
    int RegisterUpdater(const char* name, CreateUpdaterFunction* createFunction);
    return RegisterUpdater(name, []() -> Updater* { return new UpdaterClass(); });
}

} // namespace hatcher

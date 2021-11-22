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

} // namespace hatcher

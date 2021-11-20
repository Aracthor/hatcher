#pragma once

namespace hatcher
{
class World;

class Updater
{
public:
    virtual ~Updater() {}

    virtual void Update(World& world) = 0;
};

} // namespace hatcher

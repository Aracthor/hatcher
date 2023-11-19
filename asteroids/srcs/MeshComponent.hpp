#pragma once

namespace hatcher
{
class ISaveLoader;
} // namespace hatcher

struct MeshComponent
{
    enum Identifier : unsigned int
    {
        Asteroid,
        Player,
        Shoot,
        COUNT,
    };
    unsigned int ID;
};

void operator<<(hatcher::ISaveLoader& saveLoader, MeshComponent& component);

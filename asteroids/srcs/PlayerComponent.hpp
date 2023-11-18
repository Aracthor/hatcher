#pragma once

namespace hatcher
{
class ISaveLoader;
} // namespace hatcher

struct PlayerComponent
{
    bool turningLeft;
    bool turningRight;
    bool accelerating;
};

void operator<<(hatcher::ISaveLoader& saveLoader, PlayerComponent& component);

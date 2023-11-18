#pragma once

namespace hatcher
{
class ISaveLoader;
} // namespace hatcher

using namespace hatcher;

struct PlayerComponent
{
    bool turningLeft;
    bool turningRight;
    bool accelerating;
};

void operator<<(ISaveLoader& saveLoader, PlayerComponent& component);

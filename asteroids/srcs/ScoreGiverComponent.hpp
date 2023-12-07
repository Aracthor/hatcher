#pragma once

namespace hatcher
{
class ISaveLoader;
} // namespace hatcher

struct ScoreGiverComponent
{
    int points;
};

void operator<<(hatcher::ISaveLoader& saveLoader, ScoreGiverComponent& component);

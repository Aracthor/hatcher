#pragma once

#include "hatcher/IWorldComponent.hpp"

struct Score final : public hatcher::IWorldComponent
{
    int points = 0;

    Score(int64_t seed) {}

    void Save(hatcher::DataSaver& saver) const override;
    void Load(hatcher::DataLoader& loader) override;
};

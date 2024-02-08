#pragma once

#include "hatcher/IWorldComponent.hpp"

struct Score final : public hatcher::IWorldComponent
{
    int points = 0;

    void Save(hatcher::DataSaver& saver) const override;
    void Load(hatcher::DataLoader& loader) override;
};

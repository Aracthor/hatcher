#pragma once

#include "hatcher/IWorldComponent.hpp"

struct Score final : public hatcher::IWorldComponent
{
    int points = 0;

    void SaveLoad(hatcher::ISaveLoader& saveLoader) override;
};

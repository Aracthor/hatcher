#pragma once

#include "hatcher/IWorldComponent.hpp"

struct Lives final : public hatcher::IWorldComponent
{
    int remaining = 3;

    void SaveLoad(hatcher::ISaveLoader& saveLoader) override;
};

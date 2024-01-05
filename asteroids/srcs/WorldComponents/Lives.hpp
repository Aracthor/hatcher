#pragma once

#include "hatcher/IWorldComponent.hpp"

struct Lives final : public hatcher::IWorldComponent
{
    int remaining = 3;

    void Save(hatcher::ComponentSaver& saver) const override;
    void Load(hatcher::ComponentLoader& loader) override;
};

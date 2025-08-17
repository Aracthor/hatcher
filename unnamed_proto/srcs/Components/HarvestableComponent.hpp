#pragma once

#include "hatcher/EntityDescriptorID.hpp"

using namespace hatcher;

struct HarvestableComponent
{
    EntityDescriptorID harvest;
    int amount = 5;
};

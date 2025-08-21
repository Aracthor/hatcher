#pragma once

#include <unordered_map>
#include <utility>

#include "hatcher/Maths/glm_pure.hpp"

#include "Components/ItemComponent.hpp"

namespace hatcher
{
class DataLoader;
class DataSaver;
} // namespace hatcher

struct ItemDisplayComponent
{
    using LocationKey = std::pair<ItemComponent::EType, int>;
    static size_t LocationHash(LocationKey key);

    using Locations = std::unordered_map<LocationKey, glm::mat4, decltype(&LocationHash)>;
    Locations locations = Locations(10, &LocationHash);
};

void operator<<(DataSaver& saver, const ItemDisplayComponent& component);
void operator>>(DataLoader& loader, ItemDisplayComponent& component);

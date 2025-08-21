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

private:
    static size_t LocationHash(LocationKey key);

public:
    std::unordered_map<LocationKey, glm::mat4, decltype(&LocationHash)> locations =
        std::unordered_map<LocationKey, glm::mat4, decltype(&LocationHash)>(10, &LocationHash);
};

void operator<<(DataSaver& saver, const ItemDisplayComponent& component);
void operator>>(DataLoader& loader, ItemDisplayComponent& component);

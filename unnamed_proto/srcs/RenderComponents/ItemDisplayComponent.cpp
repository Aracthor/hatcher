#include "ItemDisplayComponent.hpp"

#include "hatcher/DataLoader.hpp"
#include "hatcher/DataSaver.hpp"

using namespace hatcher;

template <>
struct std::hash<ItemComponent::EType>
{
    size_t operator()(ItemComponent::EType type) const noexcept { return std::hash<int>{}(static_cast<int>(type)); }
};

size_t ItemDisplayComponent::LocationHash(LocationKey key)
{
    size_t seed = std::hash<decltype(key.first)>{}(key.first);
    seed ^= std::hash<decltype(key.second)>{}(key.second) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    return seed;
}

void operator<<(DataSaver& saver, const ItemDisplayComponent& component)
{
    saver << component.locations;
}

void operator>>(DataLoader& loader, ItemDisplayComponent& component)
{
    loader >> component.locations;
}

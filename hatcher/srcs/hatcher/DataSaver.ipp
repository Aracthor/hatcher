#include "ClassKey.hpp"

namespace hatcher
{

template <typename T>
void DataSaver::operator<<(const T& value)
{
    static_assert(std::is_trivially_copyable_v<T>);
    SaveData(&value, sizeof(value));
}

template <typename T>
void DataSaver::operator<<(const std::vector<T>& vector)
{
    *this << static_cast<int>(vector.size());
    for (const auto& value : vector)
        *this << value;
}

template <class Key, class T, class Hash>
void DataSaver::operator<<(const std::unordered_map<Key, T, Hash>& map)
{
    int size = map.size();
    *this << size;
    for (auto it : map)
    {
        *this << it.first;
        *this << it.second;
    }
}

template <class Component>
void DataSaver::SaveComponent(Component& component)
{
    uint key = ClassKey<Component>();
    *this << key;
    *this << component;
}

} // namespace hatcher

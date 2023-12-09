#include "ISaveLoader.hpp"

#include <string>

namespace hatcher
{
template <typename T>
void ISaveLoader::operator<<(T& value)
{
    static_assert(std::is_trivially_copyable_v<T>);
    SaveLoadData(&value, sizeof(value));
}

template <typename T>
void ISaveLoader::operator<<(std::vector<T>& vector)
{
    int size = vector.size();
    *this << size;
    vector.resize(size);
    for (auto& value : vector)
    {
        *this << value;
    }
}

template <class Key, class T, class Hash>
void ISaveLoader::operator<<(std::unordered_map<Key, T, Hash>& map)
{
    if (!IsSaving())
        map = {};

    int size = map.size();
    *this << size;
    for (int i = 0; i < size; i++)
    {
        if (IsSaving())
        {
            auto it = map.begin();
            std::advance(it, i);
            Key key = it->first;
            T value = it->second;
            *this << key;
            *this << value;
        }
        else
        {
            Key key;
            T value;
            *this << key;
            *this << value;
            map.insert({key, value});
        }
    }
}

} // namespace hatcher

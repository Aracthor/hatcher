#include "ISaveLoader.hpp"

#include "EntityDescriptorID.hpp"
#include "hatcher/Maths/RandomGenerator.hpp"

#include <string>

namespace hatcher
{

template <typename T>
void ISaveLoader::operator<<(std::optional<T>& optional)
{
    bool hasValue = optional.has_value();
    *this << hasValue;
    if (hasValue)
    {
        T value = *optional;
        *this << value;
        optional = value;
    }
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

template <glm::length_t L, typename T>
void ISaveLoader::operator<<(glm::vec<L, T>& vector)
{
    for (int i = 0; i < L; i++)
    {
        *this << vector[i];
    }
}

template <glm::length_t L, typename T>
void ISaveLoader::operator<<(Box<L, T>& box)
{
    *this << box.Min();
    *this << box.Max();
}

void ISaveLoader::SaveLoadData(void* data, int size)
{
    ubyte* bytes = reinterpret_cast<ubyte*>(data);
    while (size-- > 0)
    {
        *this << *bytes;
        bytes++;
    }
}

} // namespace hatcher

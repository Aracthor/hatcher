#pragma once

#include "basic_types.hpp"

#include <string>
#include <unordered_map>
#include <vector>

namespace hatcher
{

class ComponentSaver final
{
public:
    const std::vector<ubyte>& Result() const;

    template <typename T>
    inline void operator<<(const T& value);

    template <typename T>
    void operator<<(const std::vector<T>& vector);

    template <class Key, class T, class Hash>
    void operator<<(const std::unordered_map<Key, T, Hash>& map);

    void operator<<(const std::string& string);

    template <class Component>
    void SaveComponent(Component& component);

private:
    void SaveData(const void* value, int size);

    std::vector<ubyte> m_data;
};

} // namespace hatcher

#include "ComponentSaver.ipp"

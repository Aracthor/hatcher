#pragma once

#include "hatcher/basic_types.hpp"

#include <string>
#include <unordered_map>
#include <vector>

namespace hatcher
{

class DataLoader final
{
public:
    DataLoader(const std::vector<ubyte>& data);

    template <typename T>
    inline void operator>>(T& value);

    template <typename T>
    void operator>>(std::vector<T>& vector);

    template <class Key, class T, class Hash>
    void operator>>(std::unordered_map<Key, T, Hash>& map);

    void operator>>(std::string& string);

private:
    void LoadData(void* value, int size);

    const std::vector<ubyte> m_data;
    int m_currentIndex = 0;
};

} // namespace hatcher

#include "DataLoader.ipp"

#pragma once

#include "hatcher/basic_types.hpp"

#include <unordered_map>
#include <vector>

namespace std
{
typedef basic_string<char> string;
}

namespace hatcher
{
class ISaveLoader
{
public:
    template <typename T>
    inline void operator<<(T& value);

    void operator<<(std::string& string);

    template <typename T>
    void operator<<(std::vector<T>& vector);

    template <class Key, class T, class Hash>
    void operator<<(std::unordered_map<Key, T, Hash>& map);

private:
    virtual void SaveLoadData(void* value, int size) = 0;

    virtual bool IsSaving() const = 0; // To use as less as possible...
};

} // namespace hatcher

#include "ISaveLoader.ipp"

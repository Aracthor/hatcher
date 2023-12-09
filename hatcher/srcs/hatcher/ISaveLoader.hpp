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

    virtual void operator<<(bool& value) = 0;
    virtual void operator<<(char& value) = 0;
    virtual void operator<<(ubyte& value) = 0;
    virtual void operator<<(int& value) = 0;
    virtual void operator<<(uint& value) = 0;
    virtual void operator<<(float& value) = 0;

    void operator<<(std::string& string);

    template <typename T>
    void operator<<(std::vector<T>& vector);

    template <class Key, class T, class Hash>
    void operator<<(std::unordered_map<Key, T, Hash>& map);

private:
    inline void SaveLoadData(void* value, int size);

    virtual bool IsSaving() const = 0; // To use as less as possible...
};

} // namespace hatcher

#include "ISaveLoader.ipp"

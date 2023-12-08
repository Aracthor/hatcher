#pragma once

#include "EntityDescriptorID.hpp"
#include "hatcher/Maths/Box.hpp"
#include "hatcher/Maths/RandomGenerator.hpp"
#include "hatcher/Maths/glm_pure.hpp"
#include "hatcher/basic_types.hpp"

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace hatcher
{

class ISaveLoader
{
public:
    virtual void operator<<(bool& value) = 0;
    virtual void operator<<(char& value) = 0;
    virtual void operator<<(ubyte& value) = 0;
    virtual void operator<<(int& value) = 0;
    virtual void operator<<(uint& value) = 0;
    virtual void operator<<(float& value) = 0;

    inline void operator<<(EntityDescriptorID& id);
    inline void operator<<(RandomGenerator& randomGenerator);
    inline void operator<<(std::string& string);

    template <typename T>
    void operator<<(std::optional<T>& optional);

    template <typename T>
    void operator<<(std::vector<T>& vector);

    template <class Key, class T, class Hash>
    void operator<<(std::unordered_map<Key, T, Hash>& map);

    template <glm::length_t L, typename T>
    void operator<<(glm::vec<L, T>& vector);

    template <glm::length_t L, typename T>
    void operator<<(Box<L, T>& box);

private:
    inline void SaveLoadData(void* value, int size);

    virtual bool IsSaving() const = 0; // To use as less as possible...
};

} // namespace hatcher

#include "ISaveLoader.ipp"

#pragma once

#include "hatcher/Maths/Box.hpp"
#include "hatcher/glm_pure.hpp"

#include <optional>
#include <vector>

namespace hatcher
{

class ISaveLoader
{
public:
    virtual void operator<<(char value) = 0;
    virtual void operator<<(bool& value) = 0;
    virtual void operator<<(int& value) = 0;
    virtual void operator<<(uint& value) = 0;
    virtual void operator<<(float& value) = 0;

    template <typename T>
    void operator<<(std::optional<T>& optional);

    template <typename T>
    void operator<<(std::vector<T>& vector);

    template <glm::length_t L, typename T>
    void operator<<(glm::vec<L, T>& vector);

    template <glm::length_t L, typename T>
    void operator<<(Box<L, T>& box);
};

} // namespace hatcher

#include "ISaveLoader.ipp"

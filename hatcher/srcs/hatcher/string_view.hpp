#pragma once

#include <ostream>

#include "assert.hpp"

namespace hatcher
{

struct string_view
{
    const char* data;
    const int size;

    constexpr char operator[](int index) const
    {
        HATCHER_ASSERT(index < size);
        return data[index];
    }
};

std::ostream& operator<<(std::ostream& stream, const string_view& stringView);

} // namespace hatcher

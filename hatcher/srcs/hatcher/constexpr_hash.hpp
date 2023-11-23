#pragma once

constexpr int constexpr_strlen(const char* str)
{
    return *str == '\0' ? 0 : constexpr_strlen(str + 1) + 1;
}

template <int N>
constexpr unsigned int constexpr_hash(const char* str);

#include "constexpr_hash.ipp"
#pragma once

template <typename T, size_t N = sizeof(T)>
constexpr unsigned int constexpr_hash(const T str);

#include "constexpr_hash.ipp"
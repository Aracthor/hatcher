#pragma once

template <typename T, int N = sizeof(T)>
constexpr unsigned int constexpr_hash(const T str);

#include "constexpr_hash.ipp"
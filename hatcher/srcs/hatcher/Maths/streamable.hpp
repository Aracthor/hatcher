#include <ostream>

#include "Vect.hpp"

template <std::size_t L, typename T>
std::ostream& operator<<(std::ostream& stream, hatcher::Vect<L, T> vect);

#include "streamable.ipp"

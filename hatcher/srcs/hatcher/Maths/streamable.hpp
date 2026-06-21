#include <ostream>

#include "Mat.hpp"
#include "Vect.hpp"

template <std::size_t L, typename T>
std::ostream& operator<<(std::ostream& stream, hatcher::Vect<L, T> vect);

template <std::size_t L, typename T>
std::ostream& operator<<(std::ostream& stream, const hatcher::Mat<L, T>& mat);

#include "streamable.ipp"

#include <cmath>
#include <limits>

namespace hatcher
{
template <std::size_t L, typename T>
constexpr T dot(Vect<L, T> u, Vect<L, T> v)
{
    static_assert(std::numeric_limits<T>::is_iec559, "'dot' is only possible for floating-point Vect.");
    T result = T(0);
    for (std::size_t i = 0; i < L; i++)
        result += u[i] * v[i];
    return result;
}
} // namespace hatcher

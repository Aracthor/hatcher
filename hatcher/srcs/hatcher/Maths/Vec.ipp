#include <cmath>
#include <limits>

namespace hatcher
{
template <std::size_t L, typename T>
constexpr T Dot(Vec<L, T> u, Vec<L, T> v)
{
    static_assert(std::numeric_limits<T>::is_iec559, "'Dot' is only possible for floating-point Vec.");
    T result = T(0);
    for (std::size_t i = 0; i < L; i++)
        result += u[i] * v[i];
    return result;
}
} // namespace hatcher

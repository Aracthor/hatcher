namespace hatcher
{

template <std::size_t L, std::size_t VL, typename T>
constexpr Vect<VL, T> operator*(const Mat<L, T>& u, const Vect<VL, T>& v)
{
    static_assert(L >= VL);
    Vect<VL, T> result;
    for (std::size_t y = 0; y < VL; y++)
    {
        result[y] = T(0);
        for (std::size_t x = 0; x < L; x++)
        {
            T rv = u[x][y];
            if (x < VL)
                rv *= v[x];
            result[y] += rv;
        }
    }
    return result;
}

template <std::size_t L, typename T>
constexpr Mat<L, T> operator*(const Mat<L, T>& u, const Mat<L, T>& v)
{
    Mat<L, T> result = Mat<L, T>::Zero();
    for (std::size_t y = 0; y < L; y++)
    {
        for (std::size_t x = 0; x < L; x++)
        {
            for (std::size_t i = 0; i < L; i++)
                result[x][y] += u[i][y] * v[x][i];
        }
    }
    return result;
}

template <std::size_t L, typename T>
constexpr Mat<L, T>& operator*=(Mat<L, T>& u, const Mat<L, T>& v)
{
    u = u * v;
    return u;
}

} // namespace hatcher

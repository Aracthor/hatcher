namespace hatcher
{

template <typename T>
constexpr Mat<3, T>::Mat(Column u0, Column u1, Column u2)
    : columns{u0, u1, u2}
{
}

template <typename T>
constexpr T Mat<3, T>::determinant() const
{
    return columns[0][0] * columns[1][1] * columns[2][2] + columns[0][1] * columns[1][2] * columns[2][0] +
           columns[0][2] * columns[1][0] * columns[2][1] - columns[0][2] * columns[1][1] * columns[2][0] -
           columns[0][1] * columns[1][0] * columns[2][2] - columns[0][0] * columns[1][2] * columns[2][1];
}

template <typename T>
constexpr typename Mat<3, T>::Column Mat<3, T>::operator[](int index) const
{
    return columns[index];
}

template <typename T>
constexpr typename Mat<3, T>::Column& Mat<3, T>::operator[](int index)
{
    return columns[index];
}

template <typename T>
constexpr bool Mat<3, T>::operator==(const Self& other) const
{
    for (int i = 0; i < 3; i++)
    {
        if (columns[i] != other.columns[i])
            return false;
    }
    return true;
}

template <typename T>
constexpr bool Mat<3, T>::operator!=(const Self& other) const
{
    return !operator==(other);
}

template <typename T>
constexpr Mat<3, T> Mat<3, T>::zero()
{
    return {
        {T(0), T(0), T(0)},
        {T(0), T(0), T(0)},
        {T(0), T(0), T(0)},
    };
}

template <typename T>
constexpr Mat<3, T> Mat<3, T>::identity()
{
    return {
        {T(1), T(0), T(0)},
        {T(0), T(1), T(0)},
        {T(0), T(0), T(1)},
    };
}

} // namespace hatcher

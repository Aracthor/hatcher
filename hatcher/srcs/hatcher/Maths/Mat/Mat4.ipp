namespace hatcher
{

template <typename T>
Mat<4, T>::Mat(Column u0, Column u1, Column u2, Column u3)
    : columns{u0, u1, u2, u3}
{
}

template <typename T>
typename Mat<4, T>::Column Mat<4, T>::operator[](int index) const
{
    return columns[index];
}

template <typename T>
typename Mat<4, T>::Column& Mat<4, T>::operator[](int index)
{
    return columns[index];
}

template <typename T>
bool Mat<4, T>::operator==(const Self& other) const
{
    for (int i = 0; i < 4; i++)
    {
        if (columns[i] != other.columns[i])
            return false;
    }
    return true;
}

template <typename T>
bool Mat<4, T>::operator!=(const Self& other) const
{
    return !operator==(other);
}

template <typename T>
Mat<4, T> Mat<4, T>::zero()
{
    return {
        {T(0), T(0), T(0), T(0)},
        {T(0), T(0), T(0), T(0)},
        {T(0), T(0), T(0), T(0)},
        {T(0), T(0), T(0), T(0)},
    };
}

template <typename T>
Mat<4, T> Mat<4, T>::identity()
{
    return {
        {T(1), T(0), T(0), T(0)},
        {T(0), T(1), T(0), T(0)},
        {T(0), T(0), T(1), T(0)},
        {T(0), T(0), T(0), T(1)},
    };
}

template <typename T>
Mat<4, T> Mat<4, T>::orthographic(T left, T right, T bottom, T top)
{
    return {
        {T(2) / (right - left), T(0), T(0), T(0)},
        {T(0), T(2) / (top - bottom), T(0), T(0)},
        {T(0), T(0), T(-1), T(0)},
        {-(right + left) / (right - left), -(top + bottom) / (top - bottom), T(0), T(1)},
    };
}

} // namespace hatcher

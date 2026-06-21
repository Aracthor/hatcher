namespace hatcher
{

template <typename T>
constexpr Mat<4, T>::Mat(Column u0, Column u1, Column u2, Column u3)
    : columns{u0, u1, u2, u3}
{
}

template <typename T>
constexpr T Mat<4, T>::determinant() const
{
    const T det1 =
        Mat<3, T>{
            {columns[1][1], columns[1][2], columns[1][3]},
            {columns[2][1], columns[2][2], columns[2][3]},
            {columns[3][1], columns[3][2], columns[3][3]},
        }
            .determinant() *
        columns[0][0];
    const T det2 =
        Mat<3, T>{
            {columns[0][1], columns[0][2], columns[0][3]},
            {columns[2][1], columns[2][2], columns[2][3]},
            {columns[3][1], columns[3][2], columns[3][3]},
        }
            .determinant() *
        columns[1][0];
    const T det3 =
        Mat<3, T>{
            {columns[0][1], columns[0][2], columns[0][3]},
            {columns[1][1], columns[1][2], columns[1][3]},
            {columns[3][1], columns[3][2], columns[3][3]},
        }
            .determinant() *
        columns[2][0];
    const T det4 =
        Mat<3, T>{
            {columns[0][1], columns[0][2], columns[0][3]},
            {columns[1][1], columns[1][2], columns[1][3]},
            {columns[2][1], columns[2][2], columns[2][3]},
        }
            .determinant() *
        columns[3][0];

    return det1 - det2 + det3 - det4;
}

template <typename T>
constexpr typename Mat<4, T>::Column Mat<4, T>::operator[](int index) const
{
    return columns[index];
}

template <typename T>
constexpr typename Mat<4, T>::Column& Mat<4, T>::operator[](int index)
{
    return columns[index];
}

template <typename T>
constexpr bool Mat<4, T>::operator==(const Self& other) const
{
    for (int i = 0; i < 4; i++)
    {
        if (columns[i] != other.columns[i])
            return false;
    }
    return true;
}

template <typename T>
constexpr bool Mat<4, T>::operator!=(const Self& other) const
{
    return !operator==(other);
}

template <typename T>
constexpr Mat<4, T> Mat<4, T>::zero()
{
    return {
        {T(0), T(0), T(0), T(0)},
        {T(0), T(0), T(0), T(0)},
        {T(0), T(0), T(0), T(0)},
        {T(0), T(0), T(0), T(0)},
    };
}

template <typename T>
constexpr Mat<4, T> Mat<4, T>::identity()
{
    return {
        {T(1), T(0), T(0), T(0)},
        {T(0), T(1), T(0), T(0)},
        {T(0), T(0), T(1), T(0)},
        {T(0), T(0), T(0), T(1)},
    };
}

template <typename T>
constexpr Mat<4, T> Mat<4, T>::orthographic(T left, T right, T bottom, T top)
{
    return {
        {T(2) / (right - left), T(0), T(0), T(0)},
        {T(0), T(2) / (top - bottom), T(0), T(0)},
        {T(0), T(0), T(-1), T(0)},
        {-(right + left) / (right - left), -(top + bottom) / (top - bottom), T(0), T(1)},
    };
}

template <typename T>
constexpr Mat<4, T> Mat<4, T>::lookAt(Vect<3, T> position, Vect<3, T> target, Vect<3, T> up)
{
    const Vect<3, T> zaxis = (target - position).normalized();
    const Vect<3, T> xaxis = cross(zaxis, up).normalized();
    const Vect<3, T> yaxis = cross(xaxis, zaxis);
    return {
        {xaxis.x, yaxis.x, -zaxis.x, T(0)},
        {xaxis.y, yaxis.y, -zaxis.y, T(0)},
        {xaxis.z, yaxis.z, -zaxis.z, T(0)},
        {-dot(xaxis, position), -dot(yaxis, position), dot(zaxis, position), T(1)},
    };
}

} // namespace hatcher

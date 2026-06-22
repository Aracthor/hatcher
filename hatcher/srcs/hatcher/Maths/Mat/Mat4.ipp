namespace hatcher
{

template <typename T>
constexpr Mat<4, T>::Mat(Column u0, Column u1, Column u2, Column u3)
    : columns{u0, u1, u2, u3}
{
}
template <typename T>
constexpr Mat<4, T>::Mat(const Mat<3, T>& mat3)
    : columns{
          Vect<4, T>(mat3[0], 0.f),
          Vect<4, T>(mat3[1], 0.f),
          Vect<4, T>(mat3[2], 0.f),
          Vect<4, T>(0.f, 0.f, 0.f, 1.f),
      }
{
}

template <typename T>
constexpr T Mat<4, T>::Determinant() const
{
    const T det1 =
        Mat<3, T>{
            {columns[1][1], columns[1][2], columns[1][3]},
            {columns[2][1], columns[2][2], columns[2][3]},
            {columns[3][1], columns[3][2], columns[3][3]},
        }
            .Determinant() *
        columns[0][0];
    const T det2 =
        Mat<3, T>{
            {columns[0][1], columns[0][2], columns[0][3]},
            {columns[2][1], columns[2][2], columns[2][3]},
            {columns[3][1], columns[3][2], columns[3][3]},
        }
            .Determinant() *
        columns[1][0];
    const T det3 =
        Mat<3, T>{
            {columns[0][1], columns[0][2], columns[0][3]},
            {columns[1][1], columns[1][2], columns[1][3]},
            {columns[3][1], columns[3][2], columns[3][3]},
        }
            .Determinant() *
        columns[2][0];
    const T det4 =
        Mat<3, T>{
            {columns[0][1], columns[0][2], columns[0][3]},
            {columns[1][1], columns[1][2], columns[1][3]},
            {columns[2][1], columns[2][2], columns[2][3]},
        }
            .Determinant() *
        columns[3][0];

    return det1 - det2 + det3 - det4;
}

template <typename T>
constexpr Mat<4, T> Mat<4, T>::Inverse() const
{
    const T oDeterminant = T(1) / Determinant();
    T sign = T(1);
    Self result = Self::Zero();
    for (std::size_t x = 0; x < 4; x++)
    {
        for (std::size_t y = 0; y < 4; y++)
        {
            Mat<3, T> subMatrix = Mat<3, T>::Zero();
            for (std::size_t subX = 0; subX < 3; subX++)
            {
                for (std::size_t subY = 0; subY < 3; subY++)
                {
                    const int xToGet = subX >= x ? subX + 1 : subX;
                    const int yToGet = subY >= y ? subY + 1 : subY;
                    subMatrix[subX][subY] = columns[xToGet][yToGet];
                }
            }
            result[y][x] = oDeterminant * subMatrix.Determinant() * sign;
            sign *= T(-1);
        }
        sign *= T(-1);
    }
    return result;
}

template <typename T>
constexpr const T* Mat<4, T>::Data() const
{
    static_assert(sizeof(*this) == sizeof(T) * 4 * 4);
    return reinterpret_cast<const T*>(this);
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
constexpr Mat<4, T> Mat<4, T>::Zero()
{
    return {
        {T(0), T(0), T(0), T(0)},
        {T(0), T(0), T(0), T(0)},
        {T(0), T(0), T(0), T(0)},
        {T(0), T(0), T(0), T(0)},
    };
}

template <typename T>
constexpr Mat<4, T> Mat<4, T>::Identity()
{
    return {
        {T(1), T(0), T(0), T(0)},
        {T(0), T(1), T(0), T(0)},
        {T(0), T(0), T(1), T(0)},
        {T(0), T(0), T(0), T(1)},
    };
}

template <typename T>
constexpr Mat<4, T> Mat<4, T>::Translation(Vect<3, T> translation)
{
    return {
        {T(1), T(0), T(0), T(0)},
        {T(0), T(1), T(0), T(0)},
        {T(0), T(0), T(1), T(0)},
        {translation, T(1)},
    };
}

template <typename T>
constexpr Mat<4, T> Mat<4, T>::Scale(T scale)
{
    return Scale(Vect<3, T>(scale, scale, scale));
}

template <typename T>
constexpr Mat<4, T> Mat<4, T>::Scale(Vect<3, T> scale)
{
    return {
        {scale.x, T(0), T(0), T(0)},
        {T(0), scale.y, T(0), T(0)},
        {T(0), T(0), scale.z, T(0)},
        {T(0), T(0), T(0), T(1)},
    };
}

template <typename T>
Mat<4, T> Mat<4, T>::RotationAroundX(T angle)
{
    return Mat4f(Mat3f::RotationAroundX(angle));
}

template <typename T>
Mat<4, T> Mat<4, T>::RotationAroundY(T angle)
{
    return Mat4f(Mat3f::RotationAroundY(angle));
}

template <typename T>
Mat<4, T> Mat<4, T>::RotationAroundZ(T angle)
{
    return Mat4f(Mat3f::RotationAroundZ(angle));
}

template <typename T>
constexpr Mat<4, T> Mat<4, T>::Orthographic(T left, T right, T bottom, T top)
{
    return {
        {T(2) / (right - left), T(0), T(0), T(0)},
        {T(0), T(2) / (top - bottom), T(0), T(0)},
        {T(0), T(0), T(-1), T(0)},
        {-(right + left) / (right - left), -(top + bottom) / (top - bottom), T(0), T(1)},
    };
}

template <typename T>
constexpr Mat<4, T> Mat<4, T>::Orthographic(T left, T right, T bottom, T top, T zNear, T zFar)
{
    return {
        {T(2) / (right - left), T(0), T(0), T(0)},
        {T(0), T(2) / (top - bottom), T(0), T(0)},
        {T(0), T(0), -T(2) / (zFar - zNear), T(0)},
        {-(right + left) / (right - left), -(top + bottom) / (top - bottom), -(zFar + zNear) / (zFar - zNear), T(1)},
    };
}

template <typename T>
constexpr Mat<4, T> Mat<4, T>::LookAt(Vect<3, T> position, Vect<3, T> target, Vect<3, T> up)
{
    const Vect<3, T> zaxis = (target - position).Normalized();
    const Vect<3, T> xaxis = Cross(zaxis, up).Normalized();
    const Vect<3, T> yaxis = Cross(xaxis, zaxis);
    return {
        {xaxis.x, yaxis.x, -zaxis.x, T(0)},
        {xaxis.y, yaxis.y, -zaxis.y, T(0)},
        {xaxis.z, yaxis.z, -zaxis.z, T(0)},
        {-Dot(xaxis, position), -Dot(yaxis, position), Dot(zaxis, position), T(1)},
    };
}

template <typename T>
constexpr Vect<3, T> Mat<4, T>::Unproject(Vect<3, T> winCoords, const Self& modelView, const Self& projection,
                                          Vect<4, T> viewport)
{
    const Self inverse = (projection * modelView).Inverse();

    Vect<4, T> temp(winCoords, T(1));
    temp.x = (temp.x - viewport.x) / viewport.z;
    temp.y = (temp.y - viewport.y) / viewport.w;
    temp = temp * T(2) - Vect<4, T>(T(1), T(1), T(1), T(1));

    Vect<4, T> result = inverse * temp;
    result /= result.w;

    return result.xyz();
}

} // namespace hatcher

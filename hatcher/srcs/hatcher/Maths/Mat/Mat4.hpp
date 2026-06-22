#pragma once

namespace hatcher
{
template <typename T>
struct Mat<4, T>
{
    using Self = Mat<4, T>;
    using Column = Vect<4, T>;

    constexpr Mat() = default;
    constexpr Mat(const Self& other) = default;
    constexpr Mat(Self&& other) = default;
    constexpr Mat(Column u0, Column u1, Column u2, Column u3);
    constexpr Mat(const Mat<3, T>& mat3);

    Column columns[4];

    constexpr T Determinant() const;
    constexpr Self Inverse() const;

    constexpr const T* Data() const;

    constexpr Self& operator=(const Self& other) = default;
    constexpr Self& operator=(Self&& other) = default;

    constexpr Column operator[](int index) const;
    constexpr Column& operator[](int index);

    constexpr bool operator==(const Self& other) const;
    constexpr bool operator!=(const Self& other) const;

    static constexpr Self Zero();
    static constexpr Self Identity();

    static constexpr Self Translation(Vect<3, T> translation);
    static constexpr Self Scale(T scale);
    static constexpr Self Scale(Vect<3, T> scale);

    static Self RotationAroundX(T angle);
    static Self RotationAroundY(T angle);
    static Self RotationAroundZ(T angle);

    static constexpr Self Orthographic(T left, T right, T bottom, T top);
    static constexpr Self Orthographic(T left, T right, T bottom, T top, T zNear, T zFar);
    static constexpr Self LookAt(Vect<3, T> position, Vect<3, T> target, Vect<3, T> up);

    static constexpr Vect<3, T> Unproject(Vect<3, T> winCoords, const Self& modelView, const Self& projection,
                                          Vect<4, T> viewport);
};

using Mat4f = Mat<4, float>;

} // namespace hatcher

#include "Mat4.ipp"

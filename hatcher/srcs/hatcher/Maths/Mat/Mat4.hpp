#pragma once

#include "hatcher/Maths/Vect.hpp"

namespace hatcher
{
template <typename T>
struct Mat<4, T>
{
    using Self = Mat<4, T>;
    using Column = Vect<4, T>;

    constexpr Mat() = default;
    constexpr Mat(const Mat& other) = default;
    constexpr Mat(Mat&& other) = default;
    constexpr Mat(Column u0, Column u1, Column u2, Column u3);

    Column columns[4];

    constexpr T determinant() const;

    constexpr Column operator[](int index) const;
    constexpr Column& operator[](int index);

    constexpr bool operator==(const Self& other) const;
    constexpr bool operator!=(const Self& other) const;

    static constexpr Self zero();
    static constexpr Self identity();
    static constexpr Self orthographic(T left, T right, T bottom, T top);
    static constexpr Self lookAt(Vect<3, T> position, Vect<3, T> target, Vect<3, T> up);
};

using Mat4f = Mat<4, float>;

} // namespace hatcher

#include "Mat4.ipp"

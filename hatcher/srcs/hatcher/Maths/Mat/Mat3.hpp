#pragma once

namespace hatcher
{
template <typename T>
struct Mat<3, T>
{
    using Self = Mat<3, T>;
    using Column = Vect<3, T>;

    constexpr Mat() = default;
    constexpr Mat(const Mat& other) = default;
    constexpr Mat(Mat&& other) = default;
    constexpr Mat(Column u0, Column u1, Column u2);

    Column columns[3];

    constexpr T Determinant() const;

    constexpr Self& operator=(const Self& other) = default;
    constexpr Self& operator=(Self&& other) = default;

    constexpr Column operator[](int index) const;
    constexpr Column& operator[](int index);

    constexpr bool operator==(const Self& other) const;
    constexpr bool operator!=(const Self& other) const;

    static constexpr Self Zero();
    static constexpr Self Identity();

    // C++26 and constexpr cosinus are still so far...
    static Self RotationAroundX(T angle);
    static Self RotationAroundY(T angle);
    static Self RotationAroundZ(T angle);
};

using Mat3f = Mat<3, float>;

} // namespace hatcher

#include "Mat3.ipp"

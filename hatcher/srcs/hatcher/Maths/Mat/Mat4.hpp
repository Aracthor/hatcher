#pragma once

namespace hatcher
{
template <typename T>
struct Mat<4, T>
{
    using Self = Mat<4, T>;
    using Column = Vec<4, T>;

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

    static constexpr Self Translation(Vec<3, T> translation);
    static constexpr Self Scale(T scale);
    static constexpr Self Scale(Vec<3, T> scale);

    static Self RotationAroundX(T angle);
    static Self RotationAroundY(T angle);
    static Self RotationAroundZ(T angle);

    static constexpr Self Orthographic(T left, T right, T bottom, T top);
    static constexpr Self Orthographic(T left, T right, T bottom, T top, T zNear, T zFar);
    static constexpr Self LookAt(Vec<3, T> position, Vec<3, T> target, Vec<3, T> up);

    static constexpr Vec<3, T> Unproject(Vec<3, T> winCoords, const Self& modelView, const Self& projection,
                                         Vec<4, T> viewport);
};

using Mat4f = Mat<4, float>;

} // namespace hatcher

#include "Mat4.ipp"

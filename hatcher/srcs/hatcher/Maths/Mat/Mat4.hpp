#pragma once

#include "hatcher/Maths/Vect.hpp"

namespace hatcher
{
template <typename T>
struct Mat<4, T>
{
    using Self = Mat<4, T>;
    using Column = Vect<4, T>;

    Mat() = default;
    Mat(const Mat& other) = default;
    Mat(Mat&& other) = default;
    Mat(Column u0, Column u1, Column u2, Column u3);

    Column columns[4];

    Column operator[](int index) const;
    Column& operator[](int index);

    bool operator==(const Self& other) const;
    bool operator!=(const Self& other) const;

    static Self zero();
    static Self identity();
    static Self orthographic(T left, T right, T bottom, T top);
};

using Mat4f = Mat<4, float>;

} // namespace hatcher

#include "Mat4.ipp"

#include "hatcher/Maths/Mat.hpp"
#include "hatcher/Maths/streamable.hpp"

#include "UnitTester.hpp"

using namespace hatcher;

REGISTER_TEST(MatIdentity)
{
    {
        Mat3f identity = Mat3f::zero();
        identity[0][0] = 1.f;
        identity[1][1] = 1.f;
        identity[2][2] = 1.f;
        TEST_EQUALS(identity, Mat3f::identity());
    }

    {
        Mat4f identity = Mat4f::zero();
        identity[0][0] = 1.f;
        identity[1][1] = 1.f;
        identity[2][2] = 1.f;
        identity[3][3] = 1.f;
        TEST_EQUALS(identity, Mat4f::identity());
    }
}

REGISTER_TEST(MatDeterminant)
{
    {
        Mat3f u{
            {2, -3, 1},
            {2, 0, -1},
            {1, 4, 5},
        };
        TEST_EQUALS(u.determinant(), 49.f);
        Mat3f v{
            {1, 3, 2},
            {-3, -1, -3},
            {2, 3, 1},
        };
        TEST_EQUALS(v.determinant(), -15.f);
    }

    {
        Mat4f u{
            {1.f, 4.f, -4.f, 3.f},
            {2.f, -1.f, 2.f, 2.f},
            {-2.f, 4.f, 10.f, 0.f},
            {0.f, 42.f, -1.f, 3.f},
        };
        TEST_EQUALS(u.determinant(), 2436.f);
    }
}

REGISTER_TEST(MatProjection)
{
    Mat4f ortho = Mat4f::zero();
    ortho[0][0] = 1.f / 400.f;
    ortho[1][1] = 1.f / 300.f;
    ortho[2][2] = -1.f;
    ortho[3][0] = -1.f;
    ortho[3][1] = -1.f;
    ortho[3][3] = 1.f;
    TEST_EQUALS(ortho, Mat4f::orthographic(0.f, 800.f, 0.f, 600.f));
}

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

REGISTER_TEST(MatProduct)
{
    Mat4f u = Mat4f{
        {1.f, 4.f, -4.f, 3.f},
        {2.f, 0.f, 2.f, 2.f},
        {-2.f, 4.f, 10.f, 0.f},
        {0.f, 42.f, -1.f, 3.f},
    };
    Mat4f v = Mat4f{
        {4.f, 1.f, 0.f, 2.f},
        {6.f, 5.f, -1.f, 4.f},
        {2.f, 6.f, -3.f, 2.f},
        {-2.f, 0.f, 4.f, -1.f},
    };

    TEST_EQUALS(u * Vect3f(42.f, 4.f, -42.f), Vect3f(134.f, 42.f, -581.f));
    TEST_EQUALS(u * Vect4f(42.f, 4.f, -42.f, 1.0), Vect4f(134.f, 42.f, -581.f, 137.f));

    Mat4f expected{
        {6.f, 100.f, -16.f, 20.f},
        {18.f, 188.f, -28.f, 40.f},
        {20.f, 80.f, -28.f, 24.f},
        {-10.f, -34.f, 49.f, -9.f},
    };
    TEST_EQUALS(u * v, expected);
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

REGISTER_TEST(MatRotation)
{
    const float angle = 30.f * M_PI / 180.f;
    {
        Mat4f matrixAroundX(Mat3f::rotationAroundX(angle));
        Mat4f expected{
            {1.f, 0.f, 0.f, 0.f},
            {0.f, std::cos(angle), std::sin(angle), 0.f},
            {0.f, -std::sin(angle), std::cos(angle), 0.f},
            {0.f, 0.f, 0.f, 1.f},
        };
        TEST_EQUALS(matrixAroundX, expected);
    }

    {
        Mat4f matrixAroundY(Mat3f::rotationAroundY(angle));
        Mat4f expected{
            {std::cos(angle), 0.f, -std::sin(angle), 0.f},
            {0.f, 1.f, 0.f, 0.f},
            {std::sin(angle), 0.f, std::cos(angle), 0.f},
            {0.f, 0.f, 0.f, 1.f},
        };
        TEST_EQUALS(matrixAroundY, expected);
    }

    {
        Mat4f matrixAroundZ(Mat3f::rotationAroundZ(angle));
        Mat4f expected{
            {std::cos(angle), std::sin(angle), 0.f, 0.f},
            {-std::sin(angle), std::cos(angle), 0.f, 0.f},
            {0.f, 0.f, 1.f, 0.f},
            {0.f, 0.f, 0.f, 1.f},
        };
        TEST_EQUALS(matrixAroundZ, expected);
    }
}

REGISTER_TEST(MatTransformation)
{
    const float angle = M_PI / 2.f;
    Mat4f translation = Mat4f::translation(Vect3f(2.f, -1.f, 3.f));
    Mat4f rotation = Mat3f::rotationAroundZ(M_PI / 2.f);
    Mat4f scale = Mat4f::scale(Vect3f(2.f, 1.f, 1.f));
    Mat4f transformation = translation * rotation * scale;
    Mat4f expected{
        {std::cos(angle) * 2.f, std::sin(angle) * 2.f, 0.f, 0.f},
        {-std::sin(angle), std::cos(angle), 0.f, 0.f},
        {0.f, 0.f, 1.f, 0.f},
        {2.f, -1.f, 3.f, 1.f},
    };
    TEST_EQUALS(transformation, expected);
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

REGISTER_TEST(MatView)
{
    Mat4f lookAt = Mat4f::zero();
    lookAt[0][2] = 1.f;
    lookAt[1][1] = 1.f;
    lookAt[2][0] = -1.f;
    lookAt[3][2] = -1.f;
    lookAt[3][3] = 1.f;
    TEST_EQUALS(lookAt, Mat4f::lookAt({1.f, 0.f, 0.f}, {0.f, 0.f, 0.f}, {0.f, 1.f, 0.f}));
}

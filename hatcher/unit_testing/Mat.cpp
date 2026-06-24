#include "hatcher/Maths/Mat.hpp"
#include "hatcher/Maths/angles.hpp"
#include "hatcher/Maths/streamable.hpp"

#include "UnitTester.hpp"

using namespace hatcher;

REGISTER_TEST(MatIdentity)
{
    {
        Mat3f identity = Mat3f::Zero();
        identity[0][0] = 1.f;
        identity[1][1] = 1.f;
        identity[2][2] = 1.f;
        TEST_EQUALS(identity, Mat3f::Identity());
    }

    {
        Mat4f identity = Mat4f::Zero();
        identity[0][0] = 1.f;
        identity[1][1] = 1.f;
        identity[2][2] = 1.f;
        identity[3][3] = 1.f;
        TEST_EQUALS(identity, Mat4f::Identity());
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

    TEST_EQUALS(u * Vec3f(42.f, 4.f, -42.f), Vec3f(134.f, 42.f, -581.f));
    TEST_EQUALS(u * Vec4f(42.f, 4.f, -42.f, 1.0), Vec4f(134.f, 42.f, -581.f, 137.f));

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
        TEST_EQUALS(u.Determinant(), 49.f);
        Mat3f v{
            {1, 3, 2},
            {-3, -1, -3},
            {2, 3, 1},
        };
        TEST_EQUALS(v.Determinant(), -15.f);
    }

    {
        Mat4f u{
            {1.f, 4.f, -4.f, 3.f},
            {2.f, -1.f, 2.f, 2.f},
            {-2.f, 4.f, 10.f, 0.f},
            {0.f, 42.f, -1.f, 3.f},
        };
        TEST_EQUALS(u.Determinant(), 2436.f);
    }
}

REGISTER_TEST(MatInverse)
{
    auto MatAlmostEquals = [](const Mat4f& a, const Mat4f& b)
    {
        const float epsilon = 0.0001f;
        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                const float valueA = a[x][y];
                const float valueB = b[x][y];
                if (std::abs(valueA - valueB) > epsilon)
                    return false;
            }
        }
        return true;
    };

    Mat4f matrix = Mat4f{
        {1.0f, 4.0f, -4.2f, 3.0f},
        {2.0f, -1.0f, 2.2f, 2.1f},
        {-2.1f, 4.3f, 10.0f, 0.0f},
        {0.0f, 42.0f, -1.0f, 3.2f},
    };
    Mat4f expected{
        {-0.37738f, 0.430349f, -0.246039f, 0.071377f},
        {-0.0329337f, 0.00398875f, -0.0118839f, 0.0282577f},
        {-0.0650883f, 0.0886582f, 0.0534419f, 0.00283836f},
        {0.411915f, -0.0246466f, 0.172677f, -0.0574956f},
    };
    TEST_EQUALS_FUNC(matrix.Inverse(), expected, MatAlmostEquals);
}

REGISTER_TEST(MatRotation)
{
    const float angle = DegToRad(30.f);
    {
        Mat4f matrixAroundX(Mat3f::RotationAroundX(angle));
        Mat4f expected{
            {1.f, 0.f, 0.f, 0.f},
            {0.f, std::cos(angle), std::sin(angle), 0.f},
            {0.f, -std::sin(angle), std::cos(angle), 0.f},
            {0.f, 0.f, 0.f, 1.f},
        };
        TEST_EQUALS(matrixAroundX, expected);
    }

    {
        Mat4f matrixAroundY(Mat3f::RotationAroundY(angle));
        Mat4f expected{
            {std::cos(angle), 0.f, -std::sin(angle), 0.f},
            {0.f, 1.f, 0.f, 0.f},
            {std::sin(angle), 0.f, std::cos(angle), 0.f},
            {0.f, 0.f, 0.f, 1.f},
        };
        TEST_EQUALS(matrixAroundY, expected);
    }

    {
        Mat4f matrixAroundZ(Mat3f::RotationAroundZ(angle));
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
    const float angle = DegToRad(90.f);
    Mat4f translation = Mat4f::Translation(Vec3f(2.f, -1.f, 3.f));
    Mat4f rotation = Mat3f::RotationAroundZ(M_PI / 2.f);
    Mat4f scale = Mat4f::Scale(Vec3f(2.f, 1.f, 1.f));
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
    Mat4f ortho = Mat4f::Zero();
    ortho[0][0] = 1.f / 400.f;
    ortho[1][1] = 1.f / 300.f;
    ortho[2][2] = -1.f;
    ortho[3][0] = -1.f;
    ortho[3][1] = -1.f;
    ortho[3][3] = 1.f;
    TEST_EQUALS(ortho, Mat4f::Orthographic(0.f, 800.f, 0.f, 600.f));

    Mat4f orthoWithZ = Mat4f::Zero();
    orthoWithZ[0][0] = 1.f / 400.f;
    orthoWithZ[1][1] = 1.f / 300.f;
    orthoWithZ[2][2] = -2.f / 999.9f;
    orthoWithZ[3][0] = -1.f;
    orthoWithZ[3][1] = -1.f;
    orthoWithZ[3][2] = -1000.1f / 999.9f;
    orthoWithZ[3][3] = 1.f;
    TEST_EQUALS(orthoWithZ, Mat4f::Orthographic(0.f, 800.f, 0.f, 600.f, 0.1f, 1000.f));
}

REGISTER_TEST(MatView)
{
    Mat4f lookAt = Mat4f::Zero();
    lookAt[0][2] = 1.f;
    lookAt[1][1] = 1.f;
    lookAt[2][0] = -1.f;
    lookAt[3][2] = -1.f;
    lookAt[3][3] = 1.f;
    TEST_EQUALS(lookAt, Mat4f::LookAt({1.f, 0.f, 0.f}, {0.f, 0.f, 0.f}, {0.f, 1.f, 0.f}));
}

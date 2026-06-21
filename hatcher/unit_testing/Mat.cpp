#include "hatcher/Maths/Mat.hpp"
#include "hatcher/Maths/streamable.hpp"

#include "UnitTester.hpp"

using namespace hatcher;

REGISTER_TEST(MatIdentity)
{
    Mat4f identity = Mat4f::zero();
    identity[0][0] = 1.f;
    identity[1][1] = 1.f;
    identity[2][2] = 1.f;
    identity[3][3] = 1.f;
    TEST_EQUALS(identity, Mat4f::identity());
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

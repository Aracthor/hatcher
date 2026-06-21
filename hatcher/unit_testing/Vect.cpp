#include "hatcher/Maths/Vect.hpp"
#include "hatcher/Maths/streamable.hpp"

#include "UnitTester.hpp"

using namespace hatcher;

REGISTER_TEST(VectAddSub)
{
    {
        Vect2f u(1.f, 2.f);
        Vect2f v(-4.f, 3.f);
        TEST_EQUALS(u + v, Vect2f(-3.f, 5.f));
        TEST_EQUALS(u - v, Vect2f(5.f, -1.f));
    }

    {
        Vect3f u(1.f, 2.f, 3.f);
        Vect3f v(-4.f, 3.f, 0.f);
        TEST_EQUALS(u + v, Vect3f(-3.f, 5.f, 3.f));
        TEST_EQUALS(u - v, Vect3f(5.f, -1.f, 3.f));
    }

    {
        Vect4f u(1.f, 2.f, 3.f, -0.5f);
        Vect4f v(-4.f, 3.f, 0.f, 3.f);
        TEST_EQUALS(u + v, Vect4f(-3.f, 5.f, 3.f, 2.5f));
        TEST_EQUALS(u - v, Vect4f(5.f, -1.f, 3.f, -3.5f));
    }
}

REGISTER_TEST(VectMulDiv)
{
    {
        Vect2f u(1.f, 3.f);
        TEST_EQUALS(u * 3.f, Vect2f(3.f, 9.f));
        TEST_EQUALS(u / 3.f, Vect2f(1.f / 3.f, 1.f));
    }

    {
        Vect3f u(1.f, 3.f, -2.5f);
        TEST_EQUALS(u * 3.f, Vect3f(3.f, 9.f, -7.5f));
        TEST_EQUALS(u / 3.f, Vect3f(1.f / 3.f, 1.f, -2.5f / 3.f));
    }

    {
        Vect4f u(1.f, 3.f, -2.5f, -6.f);
        TEST_EQUALS(u * 3.f, Vect4f(3.f, 9.f, -7.5f, -18.f));
        TEST_EQUALS(u / 3.f, Vect4f(1.f / 3.f, 1.f, -2.5f / 3.f, -2.f));
    }
}

REGISTER_TEST(VectLength)
{
    {
        Vect2f u(1.f, -1.f);
        TEST_EQUALS(u.Length(), std::sqrt(2.f));
    }

    {
        Vect3f u(1.f, -2.f, 2.f);
        TEST_EQUALS(u.Length(), 3.f);
    }

    {
        Vect4f u(1.f, -2.f, 2.f, 4.f);
        TEST_EQUALS(u.Length(), 5.f);
    }
}

REGISTER_TEST(dotProduct)
{
    {
        Vect2f u(1.f, -2.f);
        Vect2f v(4.f, 3.f);
        TEST_EQUALS(Dot(u, v), -2.f);
    }

    {
        Vect3f u(1.f, -2.f, 0.5f);
        Vect3f v(4.f, 3.f, 1.f);
        TEST_EQUALS(Dot(u, v), -1.5f);
    }

    {
        Vect4f u(1.f, -2.f, 0.5f, 0.f);
        Vect4f v(4.f, 3.f, 1.f, 3.f);
        TEST_EQUALS(Dot(u, v), -1.5f);
    }
}

REGISTER_TEST(crossProduct)
{
    {
        Vect3f u(1.f, 0.f, 0.f);
        Vect3f v(0.f, 1.f, 0.f);
        TEST_EQUALS(Cross(u, v), Vect3f(0.f, 0.f, 1.f));
    }

    {
        Vect3f u(3.f, -3.f, 1.f);
        Vect3f v(4.f, 9.f, 2.f);
        TEST_EQUALS(Cross(u, v), Vect3f(-15.f, -2.f, 39.f));
    }
}

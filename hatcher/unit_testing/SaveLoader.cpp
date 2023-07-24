#include <iomanip>
#include <iostream>
#include <limits>

#include "hatcher/ComponentLoader.hpp"
#include "hatcher/ComponentSaver.hpp"
#include "hatcher/Maths/Box.hpp"
#include "hatcher/glm_pure.hpp"

using namespace hatcher;

template <typename T>
int testEquals(const T& a, const T& b)
{
    if (a != b)
        std::cerr << std::setprecision(std::numeric_limits<float>::digits) << "unit test fail: " << a << " != " << b
                  << std::endl;
    return a != b;
}

template <glm::length_t L, typename T>
std::ostream& operator<<(std::ostream& stream, const glm::vec<L, T>& vec)
{
    stream << '[';
    for (glm::length_t i = 0; i < L; i++)
    {
        if (i > 0)
            stream << ',';
        stream << vec[i];
    }
    stream << ']';
    return stream;
}

template <glm::length_t L, typename T>
std::ostream& operator<<(std::ostream& stream, const Box<L, T>& box)
{
    stream << '{';
    stream << box.Min();
    stream << ';';
    stream << box.Max();
    stream << '}';
    return stream;
}

int testInt()
{
    int input[] = {
        3,
        42,
        std::numeric_limits<int>::min(),
        std::numeric_limits<int>::max(),
    };
    constexpr int testCount = std::size(input);
    int output[testCount];

    ComponentSaver saver;
    for (int i = 0; i < testCount; i++)
        saver << input[i];

    ComponentLoader loader(saver.Result());
    for (int i = 0; i < testCount; i++)
        loader << output[i];

    int fails = 0;
    for (int i = 0; i < testCount; i++)
        fails += testEquals(output[i], input[i]);

    return fails;
}

int testFloat()
{
    float input[] = {
        0.f,
        42.f,
        8.3f,
        -78.f,
        std::numeric_limits<float>::lowest(),
        std::numeric_limits<float>::min(),
        -std::numeric_limits<float>::min(),
        std::numeric_limits<float>::max(),
    };
    constexpr int testCount = std::size(input);
    float output[testCount];

    ComponentSaver saver;
    for (int i = 0; i < testCount; i++)
        saver << input[i];

    ComponentLoader loader(saver.Result());
    for (int i = 0; i < testCount; i++)
        loader << output[i];

    int fails = 0;
    for (int i = 0; i < testCount; i++)
        fails += testEquals(output[i], input[i]);

    return fails;
}

int testVector()
{
    glm::vec3 input[] = {
        {0.f, 0.f, 0.f},
        {2.f, -42.f, 42.f},
        {2.1f, -78.5f, 1.f},
    };
    constexpr int testCount = std::size(input);
    glm::vec3 output[testCount];

    ComponentSaver saver;
    ISaveLoader& abstractSaver = saver;
    for (int i = 0; i < testCount; i++)
        abstractSaver << input[i];

    ComponentLoader loader(saver.Result());
    ISaveLoader& abstractLoader = loader;
    for (int i = 0; i < testCount; i++)
        abstractLoader << output[i];

    int fails = 0;
    for (int i = 0; i < testCount; i++)
        fails += testEquals(output[i], input[i]);

    return fails;
}

int testBox()
{
    Box3f input[] = {
        {},
        {{0.f, 0.f, 0.f}, {0.f, 0.f, 0.f}},
        {{2.f, -78.5f, 1.f}, {2.1f, -42.f, 2.3f}},
    };
    constexpr int testCount = std::size(input);
    Box3f output[testCount];

    ComponentSaver saver;
    ISaveLoader& abstractSaver = saver;
    for (int i = 0; i < testCount; i++)
        abstractSaver << input[i];

    ComponentLoader loader(saver.Result());
    ISaveLoader& abstractLoader = loader;
    for (int i = 0; i < testCount; i++)
        abstractLoader << output[i];

    int fails = 0;
    for (int i = 0; i < testCount; i++)
        fails += testEquals(output[i], input[i]);

    return fails;
}

int main()
{
    int fails = 0;

    fails += testInt();
    fails += testFloat();
    fails += testVector();
    fails += testBox();

    return fails;
}

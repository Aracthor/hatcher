#include <iomanip>
#include <iostream>
#include <limits>

#include "hatcher/ComponentLoader.hpp"
#include "hatcher/ComponentSaver.hpp"

using namespace hatcher;

template <typename T>
int testEquals(const T& a, const T& b)
{
    if (a != b)
        std::cerr << std::setprecision(std::numeric_limits<float>::digits) << "unit test fail: " << a << " != " << b
                  << std::endl;
    return a != b;
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

int main()
{
    int fails = 0;

    fails += testInt();
    fails += testFloat();

    return fails;
}

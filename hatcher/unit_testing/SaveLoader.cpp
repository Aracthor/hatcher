#include <limits>
#include <ostream>

#include "hatcher/DataLoader.hpp"
#include "hatcher/DataSaver.hpp"
#include "hatcher/Maths/Box.hpp"
#include "hatcher/Maths/RandomGenerator.hpp"
#include "hatcher/Maths/Vect.hpp"

using namespace hatcher;

template <std::size_t L, typename T>
std::ostream& operator<<(std::ostream& stream, Vect<L, T> vec)
{
    stream << '[';
    for (std::size_t i = 0; i < L; i++)
    {
        if (i > 0)
            stream << ',';
        stream << vec[i];
    }
    stream << ']';
    return stream;
}

template <std::size_t L, typename T>
std::ostream& operator<<(std::ostream& stream, const Box<L, T>& box)
{
    stream << '{';
    stream << box.Min();
    stream << ';';
    stream << box.Max();
    stream << '}';
    return stream;
}

#include "UnitTester.hpp"

REGISTER_TEST(SaveLoader_Int)
{
    int input[] = {
        3,
        42,
        std::numeric_limits<int>::min(),
        std::numeric_limits<int>::max(),
    };
    constexpr int testCount = std::size(input);
    int output[testCount];

    DataSaver saver;
    for (int i = 0; i < testCount; i++)
        saver << input[i];

    DataLoader loader(saver.Result());
    for (int i = 0; i < testCount; i++)
        loader >> output[i];

    for (int i = 0; i < testCount; i++)
        TEST_EQUALS(output[i], input[i]);
}

REGISTER_TEST(SaveLoader_Float)
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

    DataSaver saver;
    for (int i = 0; i < testCount; i++)
        saver << input[i];

    DataLoader loader(saver.Result());
    for (int i = 0; i < testCount; i++)
        loader >> output[i];

    for (int i = 0; i < testCount; i++)
        TEST_EQUALS(output[i], input[i]);
}

REGISTER_TEST(SaveLoader_Vector)
{
    Vect3f input[] = {
        {0.f, 0.f, 0.f},
        {2.f, -42.f, 42.f},
        {2.1f, -78.5f, 1.f},
    };
    constexpr int testCount = std::size(input);
    Vect3f output[testCount];

    DataSaver saver;
    for (int i = 0; i < testCount; i++)
        saver << input[i];

    DataLoader loader(saver.Result());
    for (int i = 0; i < testCount; i++)
        loader >> output[i];

    for (int i = 0; i < testCount; i++)
        TEST_EQUALS(output[i], input[i]);
}

REGISTER_TEST(SaveLoader_Box)
{
    Box3f input[] = {
        {},
        {{0.f, 0.f, 0.f}, {0.f, 0.f, 0.f}},
        {{2.f, -78.5f, 1.f}, {2.1f, -42.f, 2.3f}},
    };
    constexpr int testCount = std::size(input);
    Box3f output[testCount];

    DataSaver saver;
    for (int i = 0; i < testCount; i++)
        saver << input[i];

    DataLoader loader(saver.Result());
    for (int i = 0; i < testCount; i++)
        loader >> output[i];

    for (int i = 0; i < testCount; i++)
        TEST_EQUALS(output[i], input[i]);
}

REGISTER_TEST(SaveLoader_RandomGenerator)
{
    RandomGenerator randomGenerator(4242);

    DataSaver saver;
    saver << randomGenerator;

    float input[] = {
        randomGenerator.RandomFloat(0.f, 1.f),
        randomGenerator.RandomFloat(0.f, 1.f),
        randomGenerator.RandomFloat(0.f, 1.f),
    };

    DataLoader loader(saver.Result());
    loader >> randomGenerator;

    float output[] = {
        randomGenerator.RandomFloat(0.f, 1.f),
        randomGenerator.RandomFloat(0.f, 1.f),
        randomGenerator.RandomFloat(0.f, 1.f),
    };

    for (int i = 0; i < (int)std::size(input); i++)
        TEST_EQUALS(output[i], input[i]);
}

REGISTER_TEST(SaveLoader_String)
{
    std::string input[] = {
        "Hello World!",
        "",
        "Ohi\nworld!",
    };
    constexpr int testCount = std::size(input);
    std::string output[testCount];

    DataSaver saver;
    for (int i = 0; i < testCount; i++)
        saver << input[i];

    DataLoader loader(saver.Result());
    for (int i = 0; i < testCount; i++)
        loader >> output[i];

    for (int i = 0; i < testCount; i++)
        TEST_EQUALS(output[i], input[i]);
}

REGISTER_TEST(SaveLoader_Combined)
{
    Vect2f inputVec = {2.5f, -1.5f};
    uint inputCount1 = 2u;
    int inputCount2 = 42;
    bool inputBool = false;
    Box3f inputBox{{0.f, 0.f, 0.f}, {0.f, 0.f, 0.f}};
    std::string inputString = "Hello World!";

    DataSaver saver;
    saver << inputVec;
    saver << inputCount1;
    saver << inputCount2;
    saver << inputBool;
    saver << inputBox;
    saver << inputString;

    Vect2f outputVec;
    uint outputCount1;
    int outputCount2;
    Box3f outputBox;
    bool outputBool;
    std::string outputString;

    DataLoader loader(saver.Result());
    loader >> outputVec;
    loader >> outputCount1;
    loader >> outputCount2;
    loader >> outputBool;
    loader >> outputBox;
    loader >> outputString;

    TEST_EQUALS(outputVec, Vect2f(2.5f, -1.5f));
    TEST_EQUALS(outputCount1, 2u);
    TEST_EQUALS(outputCount2, 42);
    TEST_EQUALS(outputBox, Box3f(Vect3f(0.f, 0.f, 0.f), Vect3f(0.f, 0.f, 0.f)));
    TEST_EQUALS(outputBool, false);
    TEST_EQUALS(outputString, std::string("Hello World!"));
}

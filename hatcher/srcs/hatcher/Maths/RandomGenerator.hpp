#pragma once

#include <cstdint>

namespace hatcher
{

// Wichmann–Hill algorithm, https://en.wikipedia.org/wiki/Wichmann%E2%80%93Hill
class RandomGenerator
{
public:
    RandomGenerator(int64_t seed);

    float RandomFloat(float min, float max);

private:
    float RandomBetween0And1();

    int32_t m_s1;
    int32_t m_s2;
    int32_t m_s3;
};

} // namespace hatcher

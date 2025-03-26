#include "RandomGenerator.hpp"

#include "hatcher/assert.hpp"

namespace hatcher
{

RandomGenerator::RandomGenerator(int64_t seed)
{
    if (seed == 0)
        seed = 4242; // A 0 would stay a 0 forever
    m_s1 = static_cast<uint32_t>(seed) % 30269;
    m_s2 = static_cast<uint32_t>(seed >> 16) % 30307;
    m_s3 = static_cast<uint32_t>(seed >> 32) % 30323;

    // Generate some numbers to be sure whatever the seed is, every internal number shall be randomized.
    RandomBetween0And1();
    RandomBetween0And1();
    RandomBetween0And1();
}

bool RandomGenerator::RandomBool()
{
    return (RandomBetween0And1() < 0.5f);
}

int RandomGenerator::RandomInt(int min, int max)
{
    HATCHER_ASSERT(min <= max);
    const float rand = RandomBetween0And1();
    return min + rand * (max - min + 1);
}

float RandomGenerator::RandomFloat(float min, float max)
{
    HATCHER_ASSERT(min <= max);
    const float rand = RandomBetween0And1();
    return min + rand * (max - min);
}

float RandomGenerator::RandomAngle()
{
    return RandomFloat(-M_PI, M_PI);
}

glm::vec2 RandomGenerator::RandomDirection()
{
    const float angle = RandomAngle();
    return {cos(angle), sin(angle)};
}

float RandomGenerator::RandomBetween0And1()
{
    m_s1 = 171 * m_s1 % 30269;
    m_s2 = 172 * m_s2 % 30307;
    m_s3 = 170 * m_s3 % 30323;
    const float result = float(m_s1) / 30269.f + float(m_s2) / 30307.f + float(m_s3) / 30323.f;
    return result - int(result);
}

} // namespace hatcher

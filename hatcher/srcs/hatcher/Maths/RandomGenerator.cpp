#include "RandomGenerator.hpp"

#include "hatcher/assert.hpp"

namespace hatcher
{

RandomGenerator::RandomGenerator(int64_t seed)
{
    m_s1 = static_cast<int32_t>(seed);
    m_s2 = static_cast<int32_t>(seed >> 8);
    m_s3 = static_cast<int32_t>(seed >> 16);
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

float RandomGenerator::RandomBetween0And1()
{
    m_s1 = 171 * m_s1 % 30269;
    m_s2 = 172 * m_s2 % 30307;
    m_s3 = 170 * m_s3 % 30323;
    const float result = float(m_s1) / 30269.f + float(m_s2) / 30307.f + float(m_s3) / 30323.f;
    return result - int(result);
}

} // namespace hatcher

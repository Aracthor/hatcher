#include "RandomDirector.hpp"

#include "hatcher/ComponentRegisterer.hpp"
#include "hatcher/DataLoader.hpp"
#include "hatcher/DataSaver.hpp"

using namespace hatcher;

RandomDirector::RandomDirector(int64_t seed)
    : m_randomGenerator(seed)
{
}

bool RandomDirector::RandomBool()
{
    return m_randomGenerator.RandomBool();
}

int RandomDirector::RandomInt(int min, int max)
{
    return m_randomGenerator.RandomInt(min, max);
}

float RandomDirector::RandomFloat(float min, float max)
{
    return m_randomGenerator.RandomFloat(min, max);
}

float RandomDirector::RandomAngle()
{
    return m_randomGenerator.RandomFloat(-M_PI, M_PI);
}

glm::vec2 RandomDirector::RandomDirection(float speedMin, float speedMax)
{
    const float angle = RandomAngle();
    return glm::vec2(glm::cos(angle), glm::sin(angle)) * m_randomGenerator.RandomFloat(speedMin, speedMax);
}

void RandomDirector::Save(DataSaver& saver) const
{
    saver << m_randomGenerator;
}

void RandomDirector::Load(DataLoader& loader)
{
    loader >> m_randomGenerator;
}

namespace
{
WorldComponentTypeRegisterer<RandomDirector, EComponentList::Gameplay> registerer;
} // namespace

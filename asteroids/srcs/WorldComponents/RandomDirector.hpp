#pragma once

#include "hatcher/IWorldComponent.hpp"
#include "hatcher/Maths/RandomGenerator.hpp"
#include "hatcher/Maths/glm_pure.hpp"

class RandomDirector final : public hatcher::IWorldComponent
{
public:
    RandomDirector(int64_t seed);

    bool RandomBool();
    int RandomInt(int min, int max);
    float RandomFloat(float min, float max);
    float RandomAngle();
    glm::vec2 RandomDirection(float speedMin, float speedMax);

private:
    void Save(hatcher::DataSaver& saver) const override;
    void Load(hatcher::DataLoader& loader) override;

    hatcher::RandomGenerator m_randomGenerator;
};

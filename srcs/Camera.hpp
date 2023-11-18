#pragma once

#include "hatcher/IWorldComponent.hpp"
#include "hatcher/Maths/glm_pure.hpp"

using namespace hatcher;

namespace hatcher
{
class IFrameRenderer;
}

struct Camera final : public IWorldComponent
{
    glm::vec3 target = {0.f, 0.f, 0.f};
    glm::vec2 angles = {M_PI / 4.f, M_PI / 4.f};

    float pixelSize = 0.01f;

    glm::vec2 MouseCoordsToWorldCoords(int x, int y, const IFrameRenderer& frameRenderer) const;

    glm::vec3 Position() const;
    glm::vec3 Target() const;
    glm::vec3 Up() const;

    void SaveLoad(ISaveLoader& saveLoader) override;
};

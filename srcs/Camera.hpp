#pragma once

#include "hatcher/IWorldComponent.hpp"
#include "hatcher/glm_pure.hpp"

using namespace hatcher;

namespace hatcher
{
class IFrameRenderer;
}

struct Camera final : public IWorldComponent
{
    glm::vec3 position = glm::vec3(-90.f, -90.f, 100.f);
    glm::vec3 target = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 up = glm::vec3(sqrtf(2.f) / 2.f, sqrtf(2.f) / 2.f, 0.f);

    float pixelSize = 0.01f;

    glm::vec2 MouseCoordsToWorldCoords(int x, int y, const IFrameRenderer& frameRenderer) const;
};

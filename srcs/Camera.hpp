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
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;

    float pixelSize = 0.01f;

    Camera();

    glm::vec2 MouseCoordsToWorldCoords(int x, int y, const IFrameRenderer& frameRenderer) const;

    void SaveLoad(ISaveLoader& saveLoader) override;
};

#pragma once

#include <optional>

#include "hatcher/IWorldComponent.hpp"
#include "hatcher/Maths/glm_pure.hpp"

using namespace hatcher;

namespace hatcher
{
class IFrameRenderer;
}

struct Blueprint final : public IWorldComponent
{
    glm::vec2 position{};
    bool active{};

    Blueprint(int64_t seed) {}

    void Save(DataSaver& saver) const override;
    void Load(DataLoader& loader) override;
};

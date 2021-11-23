#pragma once

#include <memory>

#include "hatcher/Updater.hpp"

namespace hatcher
{
class Mesh;
class MeshBuilder;
class World;
} // namespace hatcher

class SquareDisplayUpdater final : public hatcher::Updater
{
public:
    SquareDisplayUpdater(const std::unique_ptr<hatcher::MeshBuilder>& meshBuilder);
    ~SquareDisplayUpdater();

    void Update(hatcher::ComponentManager* componentManager) override;

private:
    std::unique_ptr<hatcher::Mesh> m_mesh;
};

#pragma once

#include <memory>

namespace hatcher
{
class IFrameRenderer;
class Mesh;
class MeshBuilder;
} // namespace hatcher

class GridDisplay
{
public:
    GridDisplay(const std::unique_ptr<hatcher::MeshBuilder>& meshBuilder);
    ~GridDisplay();

    bool Enabled() const { return m_enabled; }

    void SetEnabled(bool enabled) { m_enabled = enabled; }

    void DrawGrid(hatcher::IFrameRenderer& frameRenderer, float parPositionX,
                  float parPositionY) const;

private:
    int m_gridSize = 150;
    bool m_enabled = false;
    std::unique_ptr<hatcher::Mesh> m_gridMesh;
};

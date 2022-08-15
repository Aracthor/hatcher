#pragma once

#include <memory>

namespace hatcher
{
class IFrameRenderer;
class IRendering;
class Mesh;
} // namespace hatcher

using namespace hatcher;

class GridDisplay
{
public:
    GridDisplay(const IRendering* rendering);
    ~GridDisplay();

    bool Enabled() const { return m_enabled; }

    void SetEnabled(bool enabled) { m_enabled = enabled; }

    void DrawGrid(IFrameRenderer& frameRenderer, float parPositionX, float parPositionY) const;

private:
    int m_gridSize = 150;
    bool m_enabled = false;
    std::unique_ptr<Mesh> m_gridMesh;
};

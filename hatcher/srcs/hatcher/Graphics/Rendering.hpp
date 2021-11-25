#pragma once

#include <memory>

namespace hatcher
{
class MeshBuilder;
class Window;
class World;

class Rendering
{
public:
    Rendering(const char* name, int windowWidth, int windowHeight);
    ~Rendering();

    void RenderWorld(World* parWorld);

    const std::unique_ptr<MeshBuilder>& GetMeshBuilder() { return m_meshBuilder; }

private:
    std::unique_ptr<MeshBuilder> m_meshBuilder;
    std::unique_ptr<Window> m_window;
};

} // namespace hatcher

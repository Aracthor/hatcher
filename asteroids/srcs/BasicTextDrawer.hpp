#pragma once

#include "hatcher/Maths/glm_pure.hpp"
#include "hatcher/unique_ptr.hpp"

#include <vector>

namespace hatcher
{
class IFrameRenderer;
class Material;
class MaterialFactory;
class Mesh;
class Texture;
} // namespace hatcher

class BasicTextDrawer
{
public:
    BasicTextDrawer(hatcher::MaterialFactory* materialFactory);
    ~BasicTextDrawer();

    void AddTextToDraw(const char* text, glm::vec2 position, glm::vec2 size);
    void Draw(hatcher::IFrameRenderer& frameRenderer);

private:
    hatcher::unique_ptr<hatcher::Texture> m_texture;
    hatcher::unique_ptr<hatcher::Material> m_material;
    hatcher::unique_ptr<hatcher::Mesh> m_mesh;

    std::vector<float> m_positionBuffer;
    std::vector<float> m_textureCoordsBuffer;
};

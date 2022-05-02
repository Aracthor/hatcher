#pragma once

#include "hatcher/Graphics/Core/gl_types.hpp"

namespace hatcher
{

class Texture
{
public:
    Texture();
    ~Texture();

private:
    GLuint m_textureID;
};

} // namespace hatcher

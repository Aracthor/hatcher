#include "Texture.hpp"

#include "hatcher/Graphics/Core/gl.hpp"

namespace hatcher
{

Texture::Texture()
{
    // clang-format off
    GLubyte pixels[9 * 4] = {
        18,  140, 171, 255,
        143, 143, 143, 255,
        255, 255, 255, 255,

        255, 255, 0,   255,
        0,   255, 255, 255,
        255, 0,   255, 255,

        255, 0,   0,   255,
        0,   255, 0,   255,
        0,   0,   255, 255,
    };
    // clang-format on

    GL_CHECK(glGenTextures(1, &m_textureID));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_textureID));
    GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 3, 3, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels));

    // Set the filtering mode.
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
}

Texture::~Texture()
{
    GL_CHECK(glDeleteTextures(1, &m_textureID));
}

} // namespace hatcher
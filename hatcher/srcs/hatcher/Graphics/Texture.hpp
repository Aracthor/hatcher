#pragma once

#include "hatcher/Graphics/Core/gl_types.hpp"

namespace hatcher
{

class Texture
{
public:
    Texture();
    ~Texture();

    void Bind() const;
    void Unbind() const;

private:
    GLuint m_textureID;
};

} // namespace hatcher

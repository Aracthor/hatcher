#pragma once

#include "hatcher/Graphics/Core/gl_types.hpp"

namespace hatcher
{

class Texture
{
public:
    Texture(int width, int height, const GLubyte* bytes);
    ~Texture();

    static Texture* CreateFromFile(const char* fileName);

    void Bind() const;
    void Unbind() const;

private:
    GLuint m_textureID;
};

} // namespace hatcher

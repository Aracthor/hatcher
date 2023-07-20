#include "Texture.hpp"

#include <iostream>

#include <SDL2/SDL_surface.h>

#include "hatcher/Graphics/Core/gl.hpp"

namespace hatcher
{

Texture::Texture(const char* fileName)
{
    SDL_Surface* surface = SDL_LoadBMP(fileName);
    if (surface == nullptr)
    {
        std::cerr << "Cannot load texture file '" << fileName << "': " << SDL_GetError() << std::endl;
        std::terminate();
    }

    // Must convert to RGBA : WebGL doesn't support BGR or BGRA formats...
    SDL_PixelFormat* pixelFormat = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
    SDL_Surface* convertedSurface = SDL_ConvertSurface(surface, pixelFormat, 0);
    SDL_FreeSurface(surface);
    SDL_FreeFormat(pixelFormat);
    if (convertedSurface == nullptr)
    {
        std::cerr << "Error converting surface: " << SDL_GetError() << std::endl;
        std::terminate();
    }

    const GLubyte* bytes = static_cast<const GLubyte*>(convertedSurface->pixels);
    const int width = convertedSurface->w;
    const int height = convertedSurface->h;
    const GLenum format = GL_RGBA;

    GL_CHECK(glGenTextures(1, &m_textureID));
    Bind();
    GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, bytes));

    // Set the filtering mode.
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    Unbind();

    SDL_FreeSurface(convertedSurface);
}

Texture::~Texture()
{
    GL_CHECK(glDeleteTextures(1, &m_textureID));
}

void Texture::Bind() const
{
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_textureID));
}

void Texture::Unbind() const
{
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
}

} // namespace hatcher

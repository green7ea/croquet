#include "texture.hpp"

#include <SDL.h>

#ifdef _WIN32
#include <GL/glext.h>
#endif

Texture::Texture(const std::string &filename)
{
    SDL_Surface *surface = SDL_LoadBMP(filename.c_str());
    GLenum texture_format = GL_RGB;
    GLint nColors;

    if (!surface)
    {
        fprintf(stderr, "Couldn't load surface for image %s.\n",
                filename.c_str());
    }

    if (((surface->w & (surface->w - 1)) != 0)
        || ((surface->h & (surface->h - 1)) != 0))
    {
        fprintf(stderr, "Image is %s not a power of 2.\n", filename.c_str());
    }

    nColors = surface->format->BytesPerPixel;
    switch (nColors)
    {
    case 4:
        if (surface->format->Rmask == 0x000000ff)
        {
            texture_format = GL_RGBA;
        }
        else
        {
            texture_format = GL_BGRA;
        }
        break;
    case 3:
        if (surface->format->Rmask == 0x000000ff)
        {
            texture_format = GL_RGB;
        }
        else
        {
            texture_format = GL_BGR;
        }
        break;
    default:
        fprintf(stderr, "Couldn't recognize image format for %s.\n",
                filename.c_str());
        break;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, nColors, surface->w, surface->h, 0,
                 texture_format, GL_UNSIGNED_BYTE, surface->pixels);

    if (surface)
    {
        SDL_FreeSurface(surface);
    }
}

Texture::~Texture()
{
}

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, textureID);
}

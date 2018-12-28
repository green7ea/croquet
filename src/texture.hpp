#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/gl.h>
#include <string>

class Texture
{
public:
    Texture(const std::string &filename);
    ~Texture();

    void bind();

    GLuint textureID;
};

#endif // TEXTURE_H

#ifndef SURFACE_H
#define SURFACE_H

#include "node.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "vector.hpp"

class Surface : public Node
{
public:
    Surface(Node *parent);
    virtual ~Surface();
    virtual void draw();

    static const float width;
    static const float height;

private:
    // Shader shader;
    Vector normal;
    Texture texture;
};

#endif // SURFACE_H

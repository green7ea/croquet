#ifndef MALLET_H
#define MALLET_H

#include "node.hpp"
#include "texture.hpp"

#include <GL/glu.h>

class Mallet : public Node
{
public:
    Mallet(Node *parent);
    virtual ~Mallet();

    virtual void draw();

    static const float cylinder_radius;
    static const float cylinder_length;
    static const float handle_radius;
    static const float handle_length;
    static const float mass;

private:
    GLUquadric *quad;

    Texture tex;
};

#endif // MALLET_H

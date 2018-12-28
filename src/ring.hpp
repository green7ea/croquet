#ifndef RING_H
#define RING_H

#include "node.hpp"
#include "texture.hpp"

#include <GL/glu.h>

class Ball;

class Ring : public Node
{
public:
    Ring(Node *parent);
    virtual ~Ring();

    virtual void draw();
    virtual void collision(Ball &ball, float delta_t);

    static const float radius;
    static const float height;
    static const float width;

private:
    GLUquadric *quad;

    Texture tex;
};

#endif // RING_H

#ifndef PEG_H
#define PEG_H

#include "node.hpp"
#include "texture.hpp"

#include <GL/glu.h>

class Peg : public Node
{
public:
    Peg(Node *parent);
    virtual ~Peg();

    virtual void draw();
    virtual void collision(Ball &ball, float delta_t);

    static const float radius;
    static const float height;

private:
    GLUquadric *quad;

    Texture tex;
};

#endif // PEG_H

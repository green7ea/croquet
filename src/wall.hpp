#ifndef WALL_H
#define WALL_H

#include "node.hpp"
#include "texture.hpp"

class Wall : public Node
{
public:
    Wall(Node *parent, const Vector &v1, const Vector &v2);
    virtual ~Wall();

    virtual void update(float deltat);
    virtual void draw();
    virtual void collision(Ball &ball, float delta_t);

    static const float height;

private:
    Texture tex;
    float stretch;

    Vector pos1;
    Vector pos2;

    Vector normal;
};

#endif // WALL_H

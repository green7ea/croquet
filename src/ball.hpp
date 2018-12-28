#ifndef BALL_H
#define BALL_H

#include "node.hpp"
#include "texture.hpp"

#include <GL/glu.h>
#include <functional>

class Ball : public Node
{
public:
    Ball(Node *parent, bool blue);
    virtual ~Ball();

    virtual void update(float deltat);
    virtual void draw();
    virtual void collision(Ball &ball, float delta_t);
    virtual void setAbsPos(const Vector &pos);
    virtual void setRelPos(const Vector &pos);

    Vector getPreviousPosition() const;
    bool isBlue() const;

    static const float radius;
    static const float mass;

    Vector velocity;
    float spin_speed;
    float spin;

private:
    GLUquadric *quad;

    Texture blueTex;
    Texture redTex;

    std::function<Vector(const Vector &)> accFunc;
    std::function<Vector(const Vector &)> velFunc;

    bool blue;
    Vector previousPosition;
};

#endif // BALL_H

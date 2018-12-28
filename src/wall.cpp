#include "wall.hpp"

#include "audio.hpp"
#include "ball.hpp"
#include "collision.hpp"

#include <GL/gl.h>

const float Wall::height = 0.5f; // TODO put the value from TP

Wall::Wall(Node *parent, const Vector &pos1, const Vector &pos2)
    : Node(parent), tex("media/wall.bmp"), pos1(pos1), pos2(pos2)
{
    normal = (pos2 - pos1).cross(Vector(0.f, 0.f, height));
    normal /= normal.length();

    stretch = (pos2 - pos1).length() / 1.f;
}

Wall::~Wall()
{
}

void Wall::update(float deltat)
{
    Node::update(deltat);
}

void Wall::draw()
{
    Node::draw();

    tex.bind();

    glPushMatrix();
    glMultMatrixf(cachedTransform.data);

    glBegin(GL_TRIANGLE_STRIP);
    {
        glTexCoord2f(0.f, 1.f);
        glVertex3f(pos1.x, pos1.y, 0.f);
        glTexCoord2f(stretch, 1.f);
        glVertex3f(pos2.x, pos2.y, 0.f);
        glTexCoord2f(0.f, 0.f);
        glVertex3f(pos1.x, pos1.y, height);
        glTexCoord2f(stretch, 0.f);
        glVertex3f(pos2.x, pos2.y, height);
    }
    glEnd();
    glPopMatrix();
}

void Wall::collision(Ball &ball, float delta_t)
{
    const float coeff_restitution = 0.5f;
    Vector ball_pos = ball.getRelPos();
    const Vector edge_pos = ball_pos - (normal * Ball::radius);
    bool passed = (pos2 - pos1).cross(pos2 - edge_pos).z < 0.f;

    if (passed)
    {
        const Vector v1 = edge_pos - pos1;
        const Vector v2 = pos2 - pos1;
        const float v2_length = v2.length();

        const Vector dist = (v1.dot(v2) / v2_length) * (v2 / v2_length);
        const Vector correction = v1 - dist;
        ball_pos -= correction;
        ball_pos.z = Ball::radius;
        ball.setRelPos(ball_pos);

        Audio &audio = Audio::getAudio();
        audio.playSound(1, 200.f, getAmplitude(ball_pos, ball.velocity));

        BallObstacle(ball_pos, ball.velocity, normal, coeff_restitution);
    }
    else
    {
        Node::collision(ball, delta_t);
    }
}

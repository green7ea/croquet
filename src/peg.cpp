#include "peg.hpp"

#include "audio.hpp"
#include "ball.hpp"
#include "collision.hpp"
#include "game.hpp"

#include <GL/gl.h>
#include <cmath>

const float Peg::radius = 0.02f * 0.5f;
const float Peg::height = 0.5f;

Peg::Peg(Node *parent) : Node(parent), tex("media/wood.bmp")
{
    quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    gluQuadricNormals(quad, GL_TRUE);
}

Peg::~Peg()
{
    if (quad)
    {
        gluDeleteQuadric(quad);
        quad = NULL;
    }
}

void Peg::draw()
{
    const int segments = 10;

    Node::draw();

    glPushMatrix();

    glMultMatrixf(cachedTransform.data);
    gluCylinder(quad, radius, radius, height, segments, 10);
    gluDisk(quad, 0, radius, segments, 1);
    glTranslatef(0.f, 0.f, height);
    gluDisk(quad, 0, radius, segments, 1);

    glPopMatrix();
    glPushMatrix();
    Game &game = Game::getGame();
    glMultMatrixf(cachedTransform.data);
    glMultMatrixf(game.groundProjection.data);
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.f, 0.f, 0.f);

    gluCylinder(quad, radius, radius, height, segments, 10);
    gluDisk(quad, 0, radius, segments, 1);
    glTranslatef(0.f, 0.f, height);
    gluDisk(quad, 0, radius, segments, 1);

    glColor3f(1.f, 1.f, 1.f);
    glEnable(GL_TEXTURE_2D);
    glPopMatrix();
}

void Peg::collision(Ball &ball, float delta_t)
{
    const float coeff_restitution = 0.5f;
    const float min_distance = Ball::radius + Peg::radius;
    Vector ball_pos = ball.getRelPos();
    const Vector displacement = ball_pos - pos;
    const float distance = displacement.length();

    if (distance <= min_distance)
    {
        ball_pos = pos + min_distance * (displacement / distance);
        ball.setRelPos(ball_pos);
        Vector normal = -1.f * displacement;
        normal.z = 0.f;
        normal /= normal.length();

        // Play sound
        Audio &audio = Audio::getAudio();
        audio.playSound(1, 200.f, getAmplitude(ball_pos, ball.velocity));

        BallObstacle(ball_pos, ball.velocity, normal, coeff_restitution);
    }
}

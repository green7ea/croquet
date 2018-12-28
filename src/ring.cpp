#include "ring.hpp"

#include "audio.hpp"
#include "ball.hpp"
#include "collision.hpp"
#include "game.hpp"

#include <cstdio>

const float Ring::radius = 0.015f / 2.f; // in metres
const float Ring::height = 0.3f;         // in metres
const float Ring::width = 0.1f;          // in metres

Ring::Ring(Node *parent) : Node(parent), tex("media/wood.bmp")
{
    quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    gluQuadricNormals(quad, GL_TRUE);
}

Ring::~Ring()
{
    if (quad)
    {
        gluDeleteQuadric(quad);
        quad = NULL;
    }
}

void Ring::draw()
{
    Node::draw();

    if (!quad)
    {
        fprintf(stderr, "Invalid quadric in ring.cpp\n");
        return;
    }

    tex.bind();

    glPushMatrix();
    glMultMatrixf(cachedTransform.data);

    // Left
    glTranslatef(-width * 0.5f, 0.f, 0.f);
    gluCylinder(quad, radius, radius, height, 10, 10);

    // Right
    glTranslatef(width, 0.f, 0.f);
    gluCylinder(quad, radius, radius, height, 10, 10);

    // Top
    glTranslatef(-width, 0.f, height);
    glRotatef(90.f, 0.f, 1.f, 0.f);
    gluCylinder(quad, radius, radius, width, 10, 10);

    glPopMatrix();
    glPushMatrix();
    Game &game = Game::getGame();
    glMultMatrixf(cachedTransform.data);
    glMultMatrixf(game.groundProjection.data);
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.f, 0.f, 0.f);

    // Left
    glTranslatef(-width * 0.5f, 0.f, 0.f);
    gluCylinder(quad, radius, radius, height, 10, 10);

    // Right
    glTranslatef(width, 0.f, 0.f);
    gluCylinder(quad, radius, radius, height, 10, 10);

    // Top
    glTranslatef(-width, 0.f, height);
    glRotatef(90.f, 0.f, 1.f, 0.f);
    gluCylinder(quad, radius, radius, width, 10, 10);

    glColor3f(1.f, 1.f, 1.f);
    glEnable(GL_TEXTURE_2D);
    glPopMatrix();
}

static void collHelper(const Vector &cylinder_pos, Ball &ball)
{
    const float coeff_restitution = 0.8f;
    const float min_distance = Ball::radius + Ring::radius;
    Vector ball_pos = ball.getAbsPos();
    Vector displacement = ball_pos - cylinder_pos;
    displacement.z = 0.f;
    const float distance = displacement.length();

    if (distance <= min_distance)
    {
        Vector new_ball_pos
            = cylinder_pos + min_distance * (displacement / distance);
        new_ball_pos.z = ball_pos.z;
        ball_pos = new_ball_pos;
        ball.setAbsPos(ball_pos);
        Vector normal = -1.f * displacement;
        normal.z = 0.f;
        normal /= normal.length();
        BallObstacle(ball_pos, ball.velocity, normal, coeff_restitution);

        Audio &audio = Audio::getAudio();
        audio.playSound(10, 200.f, getAmplitude(ball_pos, ball.velocity));
    }
}

void Ring::collision(Ball &ball, float delta_t)
{
    const Vector ball_pos1 = ball.getRelPos() - this->pos;
    const Vector ball_pos2 = ball.getPreviousPosition() - this->pos;

    if (((ball_pos1.y >= 0.f && ball_pos2.y <= 0.f)
         || (ball_pos1.y <= 0.f && ball_pos2.y >= 0.f))
        && ball_pos1.x <= width && ball_pos1.x >= -width)
    {
        Game &game = Game::getGame();
        game.signalGoal(ball, this);
    }

    Vector cylinderPos = getAbsPos() + Vector(-width * 0.5f, 0.f, 0.f);
    collHelper(cylinderPos, ball);

    cylinderPos = getAbsPos() + Vector(width * 0.5f, 0.f, 0.f);
    collHelper(cylinderPos, ball);
}

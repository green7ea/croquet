#include "ball.hpp"

#include "collision.hpp"
#include "game.hpp"
#include "rk4.hpp"

#include <GL/glu.h>
#include <cstdio>

const float Ball::radius = 0.076f / 2.f; // in metres
const float Ball::mass = 0.25489256f;    // in kg

Ball::Ball(Node *parent, bool blue)
    : Node(parent)
    , spin_speed(0.f)
    , spin(0.f)
    , blueTex("media/blue.bmp")
    , redTex("media/red.bmp")
    , blue(blue)
{
    quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    gluQuadricNormals(quad, GL_TRUE);

    accFunc = [](const Vector &vel) {
        const float coeff_roulement = 0.065f;
        const Vector normal = Ball::mass * Vector(0.f, 0.f, -9.8f);

        if (vel.length2() == 0) return Vector();

        const Vector fric_dir = -1.f * vel / vel.length();
        const Vector Fr = coeff_roulement * normal.length() * fric_dir;
        const Vector F = Fr;

        return F / Ball::mass;
    };

    velFunc = [&](const Vector &) { return velocity; };
}

Ball::~Ball()
{
    if (quad)
    {
        gluDeleteQuadric(quad);
        quad = NULL;
    }
}

static bool diffSigns(float a, float b)
{
    return (a >= 0.f && b < 0.f) || (a < 0.f && b >= 0.f);
}

void Ball::update(float deltat)
{
    Node::update(deltat);
    previousPosition = pos;

    if (!visible) return;

    const Vector old_vel = velocity;
    velocity = rk4(accFunc, velocity, deltat);

    if (diffSigns(velocity.x, old_vel.x) || diffSigns(velocity.y, old_vel.y)
        || diffSigns(velocity.z, old_vel.z))
    {
        velocity = Vector();
    }

    pos = rk4(velFunc, pos, deltat);

    const float distance = (pos - previousPosition).length();
    const float circ = 2.f * 3.1415f * radius;
    const float angle = (distance / circ);

    const float length = velocity.length();

    if (length > 0.f)
    {
        const Vector dir = velocity / length;
        const Vector up = Vector(0.f, 0.f, 1.f);
        const Vector left = up.cross(dir);
        Quaternion change(angle, left.x, left.y, left.z);
        change.normalize();
        rot = change * rot;
    }

    cachedUpdate = true;
}

void Ball::draw()
{
    Node::draw();

    if (!quad)
    {
        fprintf(stderr, "Invalid quadric in ball.cpp\n");
        return;
    }

    if (visible)
    {
        if (blue)
        {
            blueTex.bind();
        }
        else
        {
            redTex.bind();
        }

        glPushMatrix();
        glMultMatrixf(cachedTransform.data);
        gluSphere(quad, radius, 100.f, 100.f);

        glPopMatrix();
        glPushMatrix();

        Game &game = Game::getGame();

        // Nous changons la rotation pour la rotation unitaire et gardons la
        // même position pour que l'ombre ne tourne pas avec la balle.
        Vector temp_pos = this->pos;
        const Quaternion rot = Quaternion();
        temp_pos.z = 0.f;
        temp_pos.x += Ball::radius;
        temp_pos.y -= Ball::radius;
        const Matrix ballMatrix(rot, temp_pos);

        glMultMatrixf(ballMatrix.data);
        glMultMatrixf(game.groundProjection.data);
        glDisable(GL_TEXTURE_2D);
        glColor3f(0.f, 0.f, 0.f);
        gluSphere(quad, radius, 100.f, 100.f);

        glColor3f(1.f, 1.f, 1.f);
        glEnable(GL_TEXTURE_2D);
        glPopMatrix();
    }
}

void Ball::collision(Ball &ball, float)
{
    if (&ball == this) return;
    if (!visible || !ball.visible) return;

    const Vector displacement = ball.pos - this->pos;
    const float distance = displacement.length();
    const float min_distance = Ball::radius * 2.f;

    if (distance <= min_distance)
    {
        BallBall(*this, ball);
    }
}

void Ball::setAbsPos(const Vector &pos)
{
    Node::setAbsPos(pos);
    previousPosition = this->pos;
}

void Ball::setRelPos(const Vector &pos)
{
    Node::setRelPos(pos);
    previousPosition = this->pos;
}

Vector Ball::getPreviousPosition() const
{
    return previousPosition;
}

bool Ball::isBlue() const
{
    return blue;
}

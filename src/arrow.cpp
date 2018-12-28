#include "arrow.hpp"

#include <GL/gl.h>

Arrow::Arrow(Node *parent) : Node(parent), theta(0.f), blue(true)
{
}

Arrow::~Arrow()
{
}

void Arrow::update(float deltat)
{
    Node::update(deltat);

    theta += 90.f * deltat;

    if (theta > 360.f)
    {
        theta -= 360.f;
    }
}

void Arrow::draw()
{
    Node::draw();

    const float blue = this->blue ? 1.f : 0.f;
    const float red = this->blue ? 0.f : 1.f;

    glDisable(GL_TEXTURE_2D);
    glColor3f(red, red, blue);

    glPushMatrix();
    glMultMatrixf(cachedTransform.data);
    glRotatef(90.f, 0.f, 1.f, 0.f);
    glRotatef(theta, 1.f, 0.f, 0.f);
    glScalef(0.1f, 0.05f, 0.1f);
    glBegin(GL_TRIANGLE_STRIP);
    glVertex3f(-3.f, 1.f, 0.f);
    glVertex3f(-3.f, -1.f, 0.f);
    glVertex3f(-1.f, 1.f, 0.f);
    glVertex3f(-1.f, -1.f, 0.f);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex3f(-1.f, -3.f, 0.f);
    glVertex3f(0.f, 0.f, 0.f);
    glVertex3f(-1.f, 3.f, 0.f);
    glEnd();
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
    glColor3f(1.f, 1.f, 1.f);
}

void Arrow::setBlue(bool blue)
{
    this->blue = blue;
}

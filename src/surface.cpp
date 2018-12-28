#include "surface.hpp"

#include "matrix.hpp"

#include <GL/gl.h>

const float Surface::width = 14.f;   // in metres
const float Surface::height = 17.5f; // in metres

Surface::Surface(Node *parent) : Node(parent), texture("media/grass.bmp")
{
    pos = Vector(1.f, 0.f, 0.f);
}

Surface::~Surface()
{
}

void Surface::draw()
{
    Node::draw();
    // shader.activate();

    texture.bind();

    glPushMatrix();
    glMultMatrixf(cachedTransform.data);
    const float w = width * 0.5f;
    const float h = height * 0.5f;
    const float stretch = 5.f;

    glBegin(GL_TRIANGLE_STRIP);
    {
        glTexCoord2f(0.f, stretch);
        glVertex3f(-w, -h, 0.f);
        glTexCoord2f(stretch, stretch);
        glVertex3f(w, -h, 0.f);
        glTexCoord2f(0.f, 0.f);
        glVertex3f(-w, h, 0.f);
        glTexCoord2f(stretch, 0.f);
        glVertex3f(w, h, 0.f);
    }
    glEnd();
    glPopMatrix();
}

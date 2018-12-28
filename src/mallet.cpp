#include "mallet.hpp"

#include <cstdio>

const float Mallet::cylinder_radius = 0.076f / 2.f; // in m
const float Mallet::cylinder_length = 0.23f;        // in m
const float Mallet::handle_radius = 0.01f;          // in m
const float Mallet::handle_length = 1.f;            // in m
const float Mallet::mass = 1.25;                    // in kg

Mallet::Mallet(Node *parent) : Node(parent), tex("media/wood.bmp")
{
    quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    gluQuadricNormals(quad, GL_TRUE);
}

Mallet::~Mallet()
{
    if (quad)
    {
        gluDeleteQuadric(quad);
        quad = NULL;
    }
}

void Mallet::draw()
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
    gluCylinder(quad, handle_radius, handle_radius, handle_length, 10, 10);
    glRotatef(90.f, 0.f, 1.f, 0.f);
    glTranslatef(0.f, 0.f, -cylinder_length * 0.5f);
    gluCylinder(quad, cylinder_radius, cylinder_radius, cylinder_length, 10,
                10);
    gluDisk(quad, 0, cylinder_radius, 10, 10);
    glTranslatef(0.f, 0.f, cylinder_length);
    gluDisk(quad, 0, cylinder_radius, 10, 10);
    glPopMatrix();
}

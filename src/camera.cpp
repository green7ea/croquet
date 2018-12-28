#include "camera.hpp"

#include <GL/glu.h>

Camera::Camera(const Vector &position, const Vector &lookAt)
{
    setLookAt(position, lookAt);
}

Camera::Camera(const Camera &cam)
{
    this->pos = cam.pos;
    this->dir = cam.dir;
}

Camera::~Camera()
{
}

void Camera::draw()
{
    const Vector lookAt = pos + dir;
    gluLookAt(pos.x, pos.y, pos.z, lookAt.x, lookAt.y, lookAt.z, 0.f, 0.f, 1.f);
}

void Camera::setLookAt(const Vector &position, const Vector &lookAt)
{
    pos = position;
    dir = lookAt - position;
}

void Camera::keyMove(const Vector &mov)
{
    const Vector forward = dir / dir.length();
    const Vector up(0.f, 0.f, 1.f);
    const Vector left = up.cross(forward);

    pos += 0.1f * mov.x * forward;
    pos += 0.1f * mov.y * left;
    pos += 0.1f * mov.z * up;
}

void Camera::mouseMove(float xRot, float yRot)
{
    float theta = atan2(dir.y, dir.x);
    theta += 0.01f * xRot;
    dir.x = cos(theta);
    dir.y = sin(theta);

    const float length = dir.length();
    float gamma = asin(dir.z / length);
    gamma += 0.01f * yRot;
    dir.z = sin(gamma) * length;
}

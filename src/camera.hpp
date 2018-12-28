#ifndef CAMERA_H
#define CAMERA_H

#include "matrix.hpp"
#include "vector.hpp"

#include <cmath>

class Camera
{
public:
    Camera(const Vector &position = Vector(),
           const Vector &lookAt = Vector(1.f, 0.f, 0.f));
    Camera(const Camera *cam);
    virtual ~Camera();

    virtual void draw();
    virtual void setLookAt(const Vector &position, const Vector &lookAt);
    virtual void keyMove(const Vector &mov);
    virtual void mouseMove(float xRot, float yRot);

    Vector pos;
    Vector dir;
};

#endif // CAMERA_H

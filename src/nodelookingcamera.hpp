#ifndef NODELOOKINGCAMERA_H
#define NODELOOKINGCAMERA_H

#include "camera.hpp"
#include "node.hpp"

class NodeLookingCamera : public Camera
{
public:
    NodeLookingCamera(Node *node, const Camera *cam);
    virtual ~NodeLookingCamera();

    virtual void draw();
    virtual void keyMove(const Vector &mov);
    virtual void mouseMove(float xRot, float yRot);

protected:
    Node *node;
};

#endif // NODELOOKINGCAMERA_H

#ifndef NODECAMERA_H
#define NODECAMERA_H

#include "camera.hpp"
#include "node.hpp"

#include <memory>

class NodeCamera : public Camera
{
public:
    NodeCamera(const std::shared_ptr<Node> &node,
               float height,
               float distance,
               float theta = 0.f,
               const Vector &offset = Vector());
    virtual ~NodeCamera();

    virtual void keyMove(const Vector &mov);
    virtual void mouseMove(float xRot, float yRot);

protected:
    std::shared_ptr<Node> node;

    Vector offset;
    float height;
    float distance;
    float theta;
};

#endif // NODECAMERA_H

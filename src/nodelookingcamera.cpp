#include "nodelookingcamera.hpp"

NodeLookingCamera::NodeLookingCamera(Node *node, const Camera &cam) : node(node)
{
    this->pos = cam.pos;
    this->dir = cam.dir;
}

NodeLookingCamera::~NodeLookingCamera()
{
}

void NodeLookingCamera::draw()
{
    dir = node->getAbsPos() - pos;
    dir /= dir.length();

    Camera::draw();
}

void NodeLookingCamera::keyMove(const Vector &mov)
{
}

void NodeLookingCamera::mouseMove(float xRot, float yRot)
{
}

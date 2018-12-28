#include "nodecamera.hpp"

#include <GL/glu.h>

NodeCamera::NodeCamera(const std::shared_ptr<Node> &node,
                       float height,
                       float distance,
                       float theta,
                       const Vector &offset)
    : node(node)
    , offset(offset)
    , height(height)
    , distance(distance)
    , theta(theta)
{
    const Vector eye(distance * cos(theta), distance * sin(theta), height);
    pos = node->getAbsPos() + eye;
    dir = node->getAbsPos() - pos + offset;
}

NodeCamera::~NodeCamera()
{
}

void NodeCamera::keyMove(const Vector &mov)
{
}

void NodeCamera::mouseMove(float xRot, float yRot)
{
    const float two_pi = 2.f * 3.1415f;
    theta += 0.001f * xRot;

    if (theta > two_pi)
    {
        theta -= two_pi;
    }
    else if (theta < 0.f)
    {
        theta += two_pi;
    }

    const Vector eye(distance * cos(theta), distance * sin(theta), height);
    pos = node->getAbsPos() + eye;
    dir = node->getAbsPos() - pos + offset;
}

#include "node.hpp"

#include <GL/gl.h>

Node::Node(Node *parent) : parent(parent), cachedUpdate(true)
{
}

Node::~Node()
{
}

void Node::addChild(std::shared_ptr<Node> child)
{
    children.push_back(child);
}

void Node::remove(const std::shared_ptr<Node> &node)
{
    for (auto &iter : children)
    {
        iter->remove(node);
    }
    children.remove(node);
}

void Node::update(float deltat)
{
    for (auto &iter : children)
    {
        iter->update(deltat);
    }
}

void Node::draw()
{
    if (cachedUpdate)
    {
        cachedTransform = Matrix(this->rot, this->pos);
        cachedUpdate = false;
    }

    glPushMatrix();
    glMultMatrixf(cachedTransform.data);
    for (auto &iter : children)
    {
        iter->draw();
    }
    glPopMatrix();
}

void Node::collision(Ball &ball, float delta_t)
{
    for (auto &iter : children)
    {
        iter->collision(ball, delta_t);
    }
}

void Node::setAbsPos(const Vector &pos)
{
    this->pos = pos - parent->getAbsPos();
    this->cachedUpdate = true;
}

void Node::setRelPos(const Vector &pos)
{
    this->pos = pos;
    this->cachedUpdate = true;
}

Vector Node::getAbsPos() const
{
    Vector real_pos = pos;

    if (parent)
    {
        real_pos += parent->getOrientation() * parent->getAbsPos();
    }
    return real_pos;
}

Vector Node::getRelPos() const
{
    return pos;
}

void Node::setOrientation(const Quaternion &quat)
{
    rot = quat;
    cachedUpdate = true;
}

Quaternion Node::getOrientation() const
{
    return rot;
}

bool Node::isVisible() const
{
    return visible;
}

void Node::setVisible(bool vis)
{
    visible = vis;
}

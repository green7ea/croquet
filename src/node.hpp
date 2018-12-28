#ifndef NODE_H
#define NODE_H

#include "matrix.hpp"
#include "quaternion.hpp"
#include "vector.hpp"

#include <list>
#include <memory>

class Ball;

class Node
{
public:
    Node(Node *parent);
    virtual ~Node();
    virtual void addChild(std::shared_ptr<Node> child);
    virtual void remove(const std::shared_ptr<Node> &node);
    virtual void update(float deltat);
    virtual void draw();
    virtual void collision(Ball &ball, float delta_t);
    virtual void setAbsPos(const Vector &pos);
    virtual void setRelPos(const Vector &pos);
    virtual Vector getAbsPos() const;
    virtual Vector getRelPos() const;
    virtual void setOrientation(const Quaternion &quat);
    virtual Quaternion getOrientation() const;
    virtual bool isVisible() const;
    virtual void setVisible(bool vis);

protected:
    Node *parent;
    std::list<std::shared_ptr<Node>> children;

    Vector pos;
    Quaternion rot;

    Matrix cachedTransform;
    bool cachedUpdate;
    bool visible;
};

#endif // NODE_H

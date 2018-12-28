#ifndef ARROW_H
#define ARROW_H

#include "node.hpp"

class Arrow : public Node
{
public:
    Arrow(Node *parent);
    virtual ~Arrow();

    virtual void update(float deltat);
    virtual void draw();

    void setBlue(bool blue);

private:
    float theta;
    bool blue;
};

#endif // ARROW_H

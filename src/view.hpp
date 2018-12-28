#ifndef VIEW_H
#define VIEW_H

#include "matrix.hpp"

class View
{
public:
    void setMatrix(const Matrix &mat);

    Matrix getMatrix() const;
    Matrix getInverse() const;

private:
    Matrix matrix;
    Matrix inverse;
};

#endif // VIEW_H

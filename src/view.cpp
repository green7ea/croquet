#include "view.hpp"

void View::setMatrix(const Matrix &mat)
{
    if (mat == matrix) return;

    matrix = mat;
    inverse = mat.inverse();
}

Matrix View::getMatrix() const
{
    return matrix;
}

Matrix View::getInverse() const
{
    return inverse;
}

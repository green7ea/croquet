#ifndef RK4_H
#define RK4_H

#include "vector.hpp"

#include <functional>

Vector rk4(std::function<Vector(Vector)> func, const Vector &q0, float deltat);

#endif // RK4_H

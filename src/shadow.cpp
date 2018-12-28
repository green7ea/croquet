#include "shadow.hpp"

#include "vector.hpp"

Vector project(const Vector &point, const Vector &dir)
{
    static const Vector v(0.f, 0.f, point.z);

    Vector x = point.z * point.z / v.dot(dir);
    x.z = 0.01f;
    return x;
}

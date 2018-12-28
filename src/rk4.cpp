#include "rk4.hpp"

#include "vector.hpp"

#include <cstdio>

static bool diffSigns(float a, float b)
{
    return (a >= 0.f && b < 0.f) || (a < 0.f && b >= 0.f);
}

Vector rk4(std::function<Vector(Vector)> func, const Vector &q0, float deltat)
{
    const Vector k1 = func(q0) * deltat;
    const Vector k2 = func(q0 + k1 * 0.5f) * deltat;
    const Vector k3 = func(q0 + k2 * 0.5f) * deltat;
    const Vector k4 = func(q0 + k3) * deltat;

    // detect unstable conditions

    if (diffSigns(k1.x, k2.x) || diffSigns(k1.y, k2.y) || diffSigns(k1.z, k2.z)
        || diffSigns(k2.x, k3.x) || diffSigns(k2.y, k3.y)
        || diffSigns(k2.z, k3.z) || diffSigns(k3.x, k4.x)
        || diffSigns(k3.y, k4.y) || diffSigns(k3.z, k4.z))
    {
        return Vector();
    }

    return q0 + (k1 + 2.f * k2 + 2.f * k3 + k4) / 6.f;
}

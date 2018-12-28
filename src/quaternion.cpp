#include "quaternion.hpp"

#include "matrix.hpp"
#include "vector.hpp"

#include <cmath>

Quaternion::Quaternion(float w, float x, float y, float z)
    : w(w), x(x), y(y), z(z)
{
}

Quaternion::Quaternion(float angle, const Vector &vec)
    : w(cos(angle / 2))
    , x(vec.x * sin(angle / 2))
    , y(vec.y * sin(angle / 2))
    , z(vec.z * sin(angle / 2))
{
    normalize();
}

Quaternion::Quaternion(const Matrix &mat)
    : w(sqrt(1.f + mat.data[0] + mat.data[5] + mat.data[10]) / 2.f)
{
    x = (mat.data[9] - mat.data[6]) / (4.f * w);
    y = (mat.data[2] - mat.data[8]) / (4.f * w);
    z = (mat.data[4] - mat.data[1]) / (4.f * w);
    normalize();
}

Quaternion Quaternion::add(const Quaternion &quat) const
{
    return Quaternion(this->w + quat.w, this->x + quat.x, this->y + quat.y,
                      this->z + quat.z);
}

Quaternion Quaternion::subtract(const Quaternion &quat) const
{
    return Quaternion(this->w - quat.w, this->x - quat.x, this->y - quat.y,
                      this->z - quat.z);
}

Quaternion Quaternion::multiply(const Quaternion &quat) const
{
    const float w = this->w;
    const float x = this->x;
    const float y = this->y;
    const float z = this->z;
    const float W = quat.w;
    const float X = quat.x;
    const float Y = quat.y;
    const float Z = quat.z;

    return Quaternion(
        w * W - x * X - y * Y - z * Z, w * X + x * W + y * Z - z * Y,
        w * Y - x * Z + y * W + z * X, w * Z + x * Y - y * X + z * W);
}

Vector Quaternion::multiply(const Vector &vec) const
{
    // nVidia SDK implementation
    Vector uv, uuv;
    Vector qvec(x, y, z);
    uv = qvec.cross(vec);
    uuv = qvec.cross(uv);
    uv *= (2.f * w);
    uuv *= 2.f;

    return vec + uv + uuv;
}

float Quaternion::norm() const
{
    return sqrt(w * w + x * x + y * y + z * z);
}

Quaternion &Quaternion::normalize()
{
    const float norm = this->norm();

    if (norm > 0)
    {
        w /= norm;
        x /= norm;
        y /= norm;
        z /= norm;
    }

    return *this;
}

Quaternion Quaternion::operator+(const Quaternion &quat) const
{
    return this->add(quat);
}

Quaternion Quaternion::operator-(const Quaternion &quat) const
{
    return this->subtract(quat);
}

Quaternion Quaternion::operator*(const Quaternion &quat) const
{
    return this->multiply(quat);
}

Vector Quaternion::operator*(const Vector &vec) const
{
    return this->multiply(vec);
}

Quaternion &Quaternion::operator=(const Quaternion &quat)
{
    this->w = quat.w;
    this->x = quat.x;
    this->y = quat.y;
    this->z = quat.z;

    return *this;
}

bool Quaternion::operator==(const Quaternion &quat) const
{
    return this->w == quat.w && this->x == quat.x && this->y == quat.y
        && this->z == quat.z;
}

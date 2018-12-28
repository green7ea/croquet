#include "vector.hpp"

#include <cmath>

Vector::Vector(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
{
}

Vector Vector::add(const Vector &vec) const
{
    return Vector(this->x + vec.x, this->y + vec.y, this->z + vec.z, 1.f);
}

Vector Vector::multiply(float value) const
{
    return Vector(value * this->x, value * this->y, value * this->z, 1.f);
}

Vector Vector::cross(const Vector &vec) const
{
    return Vector(y * vec.z - vec.y * z, vec.x * z - x * vec.z,
                  x * vec.y - vec.x * y);
}

float Vector::dot(const Vector &vec) const
{
    return vec.x * this->x + vec.y * this->y + vec.z * this->z;
}

float Vector::length() const
{
    return sqrt(x * x + y * y + z * z);
}

float Vector::length2() const
{
    return x * x + y * y + z * z;
}

Vector Vector::operator+(const Vector &vec) const
{
    return this->add(vec);
}

void Vector::operator+=(const Vector &vec)
{
    this->x += vec.x;
    this->y += vec.y;
    this->z += vec.z;
}

Vector Vector::operator-(const Vector &vec) const
{
    return this->add(vec.multiply(-1.f));
}

void Vector::operator-=(const Vector &vec)
{
    this->x -= vec.x;
    this->y -= vec.y;
    this->z -= vec.z;
}

Vector Vector::operator*(float value) const
{
    return this->multiply(value);
}

void Vector::operator*=(float value)
{
    this->x *= value;
    this->y *= value;
    this->z *= value;
}

Vector Vector::operator/(float value) const
{
    return this->multiply(1.f / value);
}

void Vector::operator/=(float value)
{
    const float newVal = 1.f / value;
    this->x *= newVal;
    this->y *= newVal;
    this->z *= newVal;
}

float &Vector::operator[](unsigned int i)
{
    switch (i)
    {
    case 0:
        return this->x;
    case 1:
        return this->y;
    case 2:
        return this->z;
    case 3:
        return this->w;
    default:
        throw("Tried to access an invalid vector "
              "parameter with operator[].");
    }
}

Vector &Vector::operator=(const Vector &vec)
{
    this->x = vec.x;
    this->y = vec.y;
    this->z = vec.z;
    this->w = vec.w;

    return *this;
}

bool Vector::operator==(const Vector &vec) const
{
    return vec.x == vec.x && vec.y == vec.y && vec.z == vec.z && vec.w == vec.w;
}

Vector operator*(float value, const Vector &vec)
{
    return vec.multiply(value);
}

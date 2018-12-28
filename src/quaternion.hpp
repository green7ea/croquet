#ifndef QUATERNION_H
#define QUATERNION_H

class Vector;
class Matrix;

class Quaternion
{
public:
    Quaternion(float w = 1.f, float x = 0.f, float y = 0.f, float z = 0.f);
    Quaternion(float angle, const Vector &vec);
    Quaternion(const Matrix &mat);

    Quaternion add(const Quaternion &quat) const;
    Quaternion subtract(const Quaternion &quat) const;
    Quaternion multiply(const Quaternion &quat) const;
    Vector multiply(const Vector &vec) const;
    float norm() const;
    Quaternion &normalize();

    Quaternion operator+(const Quaternion &quat) const;
    Quaternion operator-(const Quaternion &quat) const;
    Quaternion operator*(const Quaternion &quat) const;
    Vector operator*(const Vector &vec) const;
    Quaternion &operator=(const Quaternion &quat);
    bool operator==(const Quaternion &quat) const;

    float w;
    float x;
    float y;
    float z;
};

#endif // QUATERNION_H

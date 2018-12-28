#ifndef MATRIX_H
#define MATRIX_H

#include <string>

class Vector;
class Quaternion;

class Matrix
{
public:
    Matrix();
    Matrix(const float *data);
    Matrix(const Matrix &n);
    Matrix(const Quaternion &quat);
    Matrix(const Vector &vec);
    Matrix(const Quaternion &quat, const Vector &vec);
    Matrix(const Vector &base_x, const Vector &base_y, const Vector &base_z);

    Matrix multiply(const Matrix &n) const;
    Vector multiply(const Vector &vec) const;
    Matrix scale(float x, float y, float z) const;
    Matrix rotate(float angle, float x, float y, float z) const;
    Matrix translate(float x, float y, float z) const;
    Matrix inverse() const;
    std::string format() const;

    Matrix operator*(const Matrix &n) const;
    Vector operator*(const Vector &vec) const;
    Matrix &operator=(const Matrix &n);
    bool operator==(const Matrix &mat) const;

    static Matrix identity();
    static Matrix rotation(float angle, float x, float y, float z);
    static Matrix translation(float x, float y, float z);
    static Matrix perspective(float left,
                              float right,
                              float top,
                              float bottom,
                              float near,
                              float far);
    static Matrix ortho(float left,
                        float right,
                        float top,
                        float bottom,
                        float near,
                        float far);

    float data[16];
};

#endif // MATRIX_H

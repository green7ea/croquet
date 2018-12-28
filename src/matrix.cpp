#include "matrix.hpp"

#include "quaternion.hpp"
#include "vector.hpp"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

static void stupidFunctionBecauseC11IsntImplementedYet(Matrix *mat,
                                                       const Quaternion &quat,
                                                       const Vector &vec)
{
    mat->data[0] = 1.f - 2.f * quat.y * quat.y - 2.f * quat.z * quat.z;
    mat->data[1] = 2.f * quat.x * quat.y - 2.f * quat.z * quat.w;
    mat->data[2] = 2.f * quat.x * quat.z + 2.f * quat.y * quat.w;
    mat->data[3] = 0.f;
    mat->data[4] = 2.f * quat.x * quat.y + 2.f * quat.z * quat.w;
    mat->data[5] = 1.f - 2.f * quat.x * quat.x - 2.f * quat.z * quat.z;
    mat->data[6] = 2.f * quat.y * quat.z - 2.f * quat.x * quat.w;
    mat->data[7] = 0.f;
    mat->data[8] = 2.f * quat.x * quat.z - 2.f * quat.y * quat.w;
    mat->data[9] = 2.f * quat.y * quat.z + 2.f * quat.x * quat.w;
    mat->data[10] = 1.f - 2.f * quat.x * quat.x - 2.f * quat.y * quat.y;
    mat->data[11] = 0.f;
    mat->data[12] = vec.x;
    mat->data[13] = vec.y;
    mat->data[14] = vec.z;
    mat->data[15] = 1.f;
}

Matrix::Matrix()
{
    this->data[0] = this->data[5] = this->data[10] = this->data[15] = 1.f;
    this->data[1] = this->data[2] = this->data[3] = 0.f;
    this->data[4] = this->data[6] = this->data[7] = 0.f;
    this->data[8] = this->data[9] = this->data[11] = 0.f;
    this->data[12] = this->data[13] = this->data[14] = 0.f;
}

Matrix::Matrix(const float *n)
{
    memcpy(data, n, sizeof data);
}

Matrix::Matrix(const Matrix &n)
{
    operator=(n);
}

Matrix::Matrix(const Quaternion &quat)
{
    stupidFunctionBecauseC11IsntImplementedYet(this, quat, Vector());
}

Matrix::Matrix(const Vector &vec)
{
    stupidFunctionBecauseC11IsntImplementedYet(this, Quaternion(), vec);
}

Matrix::Matrix(const Quaternion &quat, const Vector &vec)
{
    stupidFunctionBecauseC11IsntImplementedYet(this, quat, vec);
}

Matrix::Matrix(const Vector &base_x, const Vector &base_y, const Vector &base_z)
{
    this->data[0] = base_x.x;
    this->data[1] = base_x.y;
    this->data[2] = base_x.z;
    this->data[3] = 0.f;

    this->data[4] = base_y.x;
    this->data[5] = base_y.y;
    this->data[6] = base_y.z;
    this->data[7] = 0.f;

    this->data[8] = base_z.x;
    this->data[9] = base_z.y;
    this->data[10] = base_z.z;
    this->data[11] = 0.f;

    this->data[12] = 0.f;
    this->data[13] = 0.f;
    this->data[14] = 0.f;
    this->data[15] = 1.f;
}

Matrix Matrix::multiply(const Matrix &n) const
{
    Matrix res;
    const float *row, *column;
    div_t d;

    for (int i = 0; i < 16; ++i)
    {
        res.data[i] = 0;
        d = div(i, 4);
        row = n.data + d.quot * 4;
        column = data + d.rem;
        for (int j = 0; j < 4; ++j)
        {
            res.data[i] += row[j] * column[j * 4];
        }
    }

    return res;
}

Vector Matrix::multiply(const Vector &vec) const
{
    return Vector(this->data[0] * vec.x + this->data[4] * vec.y
                      + this->data[8] * vec.z + this->data[12] * vec.w,
                  this->data[1] * vec.x + this->data[5] * vec.y
                      + this->data[9] * vec.z + this->data[13] * vec.w,
                  this->data[2] * vec.x + this->data[6] * vec.y
                      + this->data[10] * vec.z + this->data[14] * vec.w);
}

Matrix Matrix::scale(float x, float y, float z) const
{
    return multiply(Matrix::scale(x, y, z));
}

Matrix Matrix::rotate(float angle, float x, float y, float z) const
{
    return multiply(Matrix::rotation(angle, x, y, z));
}

Matrix Matrix::translate(float x, float y, float z) const
{
    return multiply(Matrix::translation(x, y, z));
}

Matrix Matrix::inverse() const
{
    // TODO complete this.
    return Matrix();
}

std::string Matrix::format() const
{
    char output[512];
    snprintf(output, 512,
             "\n"
             "( %.2f %.2f %.2f %.2f\n"
             "  %.2f %.2f %.2f %.2f\n"
             "  %.2f %.2f %.2f %.2f\n"
             "  %.2f %.2f %.2f %.2f )\n",
             data[0], data[1], data[2], data[3], data[4], data[5], data[6],
             data[7], data[8], data[9], data[10], data[11], data[12], data[13],
             data[14], data[15]);

    return std::string(output);
}

Matrix Matrix::operator*(const Matrix &n) const
{
    return multiply(n);
}

Vector Matrix::operator*(const Vector &vec) const
{
    return this->multiply(vec);
}

Matrix &Matrix::operator=(const Matrix &n)
{
    memcpy(data, n.data, sizeof data);
    return *this;
}

bool Matrix::operator==(const Matrix &mat) const
{
    for (unsigned int i = 0; i < 16; ++i)
    {
        if (this->data[i] != mat.data[i]) return false;
    }

    return true;
}

Matrix Matrix::identity()
{
    const float temp[] = {1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f,
                          0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f};
    return Matrix(temp);
}

Matrix Matrix::rotation(float angle, float x, float y, float z)
{
    const float s = sin(angle);
    const float c = cos(angle);

    const float temp[] = {x * x * (1 - c) + c,
                          y * x * (1 - c) + z * s,
                          x * z * (1 - c) - y * s,
                          0.f,
                          x * y * (1 - c) - z * s,
                          y * y * (1 - c) + c,
                          y * z * (1 - c) + x * s,
                          0.f,
                          x * z * (1 - c) + y * s,
                          y * z * (1 - c) - x * s,
                          z * z * (1 - c) + c,
                          0.f,
                          0.f,
                          0.f,
                          0.f,
                          1.f};
    return Matrix(temp);
}

Matrix Matrix::translation(float x, float y, float z)
{
    const float temp[] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, x, y, z, 1};
    return Matrix(temp);
}

Matrix Matrix::perspective(
    float left, float right, float top, float bottom, float near, float far)
{
    const float proj[] = {2.f * near / (right - left),
                          0.f,
                          (right + left) / (right - left),
                          0.f,
                          0.f,
                          2.f * near / (top - bottom),
                          (top + bottom) / (top - bottom),
                          0.f,
                          0.f,
                          0.f,
                          -(far + near) / (far - near),
                          -2.f * far * near / (far - near),
                          0.f,
                          0.f,
                          -1.f,
                          0.f};

    return Matrix(proj);
}

Matrix Matrix::ortho(
    float left, float right, float top, float bottom, float near, float far)
{
    const float ortho[] = {2.f / (right - left),
                           0.f,
                           0.f,
                           -(right + left) / (right - left),
                           0.f,
                           2.f / (top - bottom),
                           0.f,
                           -(top + bottom) / (top - bottom),
                           0.f,
                           0.f,
                           -2.f / (far - near),
                           (far + near) / (far - near),
                           0.f,
                           0.f,
                           0.f,
                           1.f};

    return Matrix(ortho);
}

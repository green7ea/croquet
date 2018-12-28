#ifndef VECTOR_H
#define VECTOR_H

class Vector
{
public:
    Vector(float x = 0.f, float y = 0.f, float z = 0.f, float w = 1.f);

    Vector add(const Vector &vec) const;
    Vector multiply(float value) const;
    Vector cross(const Vector &vec) const;
    float dot(const Vector &vec) const;
    float length() const;
    float length2() const;

    Vector operator+(const Vector &vec) const;
    void operator+=(const Vector &vec);
    Vector operator-(const Vector &vec) const;
    void operator-=(const Vector &vec);
    Vector operator*(float value) const;
    void operator*=(float value);
    Vector operator/(float value) const;
    void operator/=(float value);
    float &operator[](unsigned int i);
    Vector &operator=(const Vector &vec);
    bool operator==(const Vector &vec) const;

    float x;
    float y;
    float z;
    float w;
};

Vector operator*(float value, const Vector &vec);

#endif // VECTOR_H

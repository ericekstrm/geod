#pragma once

#include <ostream>

#include "OBJ_Loader.h"

class Vector2
{
public:
    Vector2();
    Vector2(Vector2 const&);
    Vector2(float x, float y);
    Vector2 operator=(Vector2 const&);

    Vector2 operator+(Vector2 const&) const;
    Vector2 operator+=(Vector2 const&);
    Vector2 operator-(Vector2 const&) const;
    Vector2 operator-=(Vector2 const&);

    Vector2 operator*(float) const;
    Vector2 operator*=(float);
    Vector2 operator/(float) const;
    Vector2 operator/=(float);

    // dot product
    float operator*(Vector2 const&);

    bool operator==(Vector2 const&) const;
    bool operator!=(Vector2 const&) const;
    bool operator< (Vector2 const&) const;

    float length() const;
    void normalize();

    float x;
    float y;
};

std::ostream& operator<<(std::ostream& os, Vector2 const& rhs);

class Vector3
{
public:
    Vector3();
    Vector3(Vector3 const&);
    Vector3(float x, float y, float z);
    ~Vector3();
    Vector3 operator=(Vector3 const&);

    Vector3(objl::Vector3 const&);
    Vector3& operator=(objl::Vector3 const&);

    Vector3 operator+(Vector3 const&) const;
    Vector3 operator+=(Vector3 const&);
    Vector3 operator-(Vector3 const&) const;
    Vector3 operator-=(Vector3 const&);

    Vector3 operator*(float) const;
    Vector3 operator*=(float);
    Vector3 operator/(float) const;
    Vector3 operator/=(float);
    Vector3 cross(Vector3 const& rhs) const;

    // dot product
    float operator*(Vector3 const&) const;

    bool operator==(Vector3 const&) const;
    bool operator!=(Vector3 const&) const;

    float length() const;
    Vector3 normalize();

    float x;
    float y;
    float z;
};

std::ostream& operator<<(std::ostream& os, Vector3 const& rhs);

class Vector4
{
public:
    Vector4();
    Vector4(Vector4 const&);
    Vector4(std::initializer_list<float> list);
    ~Vector4();
    Vector4 operator=(Vector4 const&);

    Vector4 operator+(Vector4 const&) const;
    Vector4 operator+=(Vector4 const&);
    Vector4 operator-(Vector4 const&) const;
    Vector4 operator-=(Vector4 const&);

    Vector4 operator*(float) const;
    Vector4 operator*=(float);
    Vector4 operator/(float) const;
    Vector4 operator/=(float);

    float& operator[](int);
    float operator[](int) const;

    // dot product
    float operator*(Vector4 const&);

    bool operator==(Vector4 const&) const;
    bool operator!=(Vector4 const&) const;

    float length() const;
    void normalize();

private:
    Vector4(float tmp[4]);

    float x[4];
};

typedef Vector2 vec2;
typedef Vector3 vec3;
typedef Vector4 vec4;

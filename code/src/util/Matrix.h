#pragma once

#include <type_traits>
#include "Vector.h"

#include <vector>

#define mat4 Matrix4

class Matrix4
{
public:
    Matrix4();
    Matrix4(Matrix4 const&);
    Matrix4(std::initializer_list<float> list);
    ~Matrix4();
    Matrix4 operator=(Matrix4 const&);

    Matrix4 operator+(Matrix4 const&) const;
    Matrix4 operator*(float) const;
    Matrix4 operator*=(float);
    Matrix4 operator/(float) const;
    Matrix4 operator/=(float);

    Matrix4 operator*(Matrix4 const&) const;

    vec3 operator*(vec3 const&) const;
    Vector4 operator*(Vector4 const&) const;

    bool operator==(Matrix4 const&) const;
    bool operator!=(Matrix4 const&) const;

    Matrix4 inverse() const;
    float determinant() const;
    Matrix4 transpose() const;
    Matrix4 remove_translation();

    float m[4][4];

private:
    Matrix4(float tmp[4][4]);
};

std::ostream& operator<<(std::ostream & os, Matrix4 const & rhs);

Matrix4 frustum_projection_matrix(float near, float far, float right, float left, float top, float bottom);
Matrix4 fov_projection_matrix(float fovy, float aspect, float near, float far);
Matrix4 ortho_projection_matrix(float left, float right, float bottom, float top, float near, float far);

Matrix4 look_at(vec3 const& position, vec3 const& look_at, vec3 const& up_vector);

Matrix4 rotation_matrix(float angle, float x, float y, float z);
Matrix4 rotation_matrix(float angle, vec3 const& r);
Matrix4 rotation_matrix(float x, float y, float z);
Matrix4 rotation_matrix(vec3 const& a, vec3 const& b);
Matrix4 translation_matrix(float x, float y, float z);
Matrix4 translation_matrix(vec3 const& pos);
Matrix4 scale_matrix(float x, float y, float z);


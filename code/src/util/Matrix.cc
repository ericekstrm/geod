#include "Matrix.h"
#include "Vector.h"

#include <cmath>
#include <iostream>

#include "Matrix.h"

Matrix4::Matrix4()
    : Matrix4 {
    1,0,0,0,
    0,1,0,0,
    0,0,1,0,
    0,0,0,1}
{}

Matrix4::Matrix4(Matrix4 const & rhs)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m[i][j] = rhs.m[i][j];
        }
    }
}

Matrix4::Matrix4(std::initializer_list<float> list)
{
    if (list.size() != 16)
    {
        throw std::logic_error("dimension missmatch in matrix constructor.");
    }
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m[i][j] = *(list.begin() + i * 4 + j);
        }
    }
}

Matrix4::~Matrix4()
{
}

Matrix4 Matrix4::operator=(Matrix4 const & rhs)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m[i][j] = rhs.m[i][j];
        }
    }
    return *this;
}

Matrix4 Matrix4::operator+(Matrix4 const& rhs) const
{
    Matrix4 tmp {};

    tmp.m[0][0] = m[0][0] + rhs.m[0][0];
    tmp.m[1][0] = m[1][0] + rhs.m[1][0];
    tmp.m[2][0] = m[2][0] + rhs.m[2][0];
    tmp.m[3][0] = m[3][0] + rhs.m[3][0];

    tmp.m[0][1] = m[0][1] + rhs.m[0][1];
    tmp.m[1][1] = m[1][1] + rhs.m[1][1];
    tmp.m[2][1] = m[2][1] + rhs.m[2][1];
    tmp.m[3][1] = m[3][1] + rhs.m[3][1];

    tmp.m[0][2] = m[0][2] + rhs.m[0][2];
    tmp.m[1][2] = m[1][2] + rhs.m[1][2];
    tmp.m[2][2] = m[2][2] + rhs.m[2][2];
    tmp.m[3][2] = m[3][2] + rhs.m[3][2];

    tmp.m[0][3] = m[0][3] + rhs.m[0][3];
    tmp.m[1][3] = m[1][3] + rhs.m[1][3];
    tmp.m[2][3] = m[2][3] + rhs.m[2][3];
    tmp.m[3][3] = m[3][3] + rhs.m[3][3];

    return tmp;
}

Matrix4 Matrix4::operator*(float f) const
{
    float tmp[4][4];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            tmp[i][j] = m[i][j] * f;
        }
    }
    return Matrix4 {tmp};
}

Matrix4 Matrix4::operator*=(float f)
{
    *this = operator*(f);
    return *this;
}

Matrix4 Matrix4::operator/(float f) const
{
    return operator*(1 / f);
}

Matrix4 Matrix4::operator/=(float f)
{
    *this = operator/(f);
    return *this;
}

Matrix4 Matrix4::operator*(Matrix4 const & rhs) const
{
    Matrix4 tmp {};
    tmp.m[0][0] = m[0][0] * rhs.m[0][0] + m[0][1] * rhs.m[1][0] + m[0][2] * rhs.m[2][0] + m[0][3] * rhs.m[3][0];
    tmp.m[1][0] = m[1][0] * rhs.m[0][0] + m[1][1] * rhs.m[1][0] + m[1][2] * rhs.m[2][0] + m[1][3] * rhs.m[3][0];
    tmp.m[2][0] = m[2][0] * rhs.m[0][0] + m[2][1] * rhs.m[1][0] + m[2][2] * rhs.m[2][0] + m[2][3] * rhs.m[3][0];
    tmp.m[3][0] = m[3][0] * rhs.m[0][0] + m[3][1] * rhs.m[1][0] + m[3][2] * rhs.m[2][0] + m[3][3] * rhs.m[3][0];

    tmp.m[0][1] = m[0][0] * rhs.m[0][1] + m[0][1] * rhs.m[1][1] + m[0][2] * rhs.m[2][1] + m[0][3] * rhs.m[3][1];
    tmp.m[1][1] = m[1][0] * rhs.m[0][1] + m[1][1] * rhs.m[1][1] + m[1][2] * rhs.m[2][1] + m[1][3] * rhs.m[3][1];
    tmp.m[2][1] = m[2][0] * rhs.m[0][1] + m[2][1] * rhs.m[1][1] + m[2][2] * rhs.m[2][1] + m[2][3] * rhs.m[3][1];
    tmp.m[3][1] = m[3][0] * rhs.m[0][1] + m[3][1] * rhs.m[1][1] + m[3][2] * rhs.m[2][1] + m[3][3] * rhs.m[3][1];
    
    tmp.m[0][2] = m[0][0] * rhs.m[0][2] + m[0][1] * rhs.m[1][2] + m[0][2] * rhs.m[2][2] + m[0][3] * rhs.m[3][2];
    tmp.m[1][2] = m[1][0] * rhs.m[0][2] + m[1][1] * rhs.m[1][2] + m[1][2] * rhs.m[2][2] + m[1][3] * rhs.m[3][2];
    tmp.m[2][2] = m[2][0] * rhs.m[0][2] + m[2][1] * rhs.m[1][2] + m[2][2] * rhs.m[2][2] + m[2][3] * rhs.m[3][2];
    tmp.m[3][2] = m[3][0] * rhs.m[0][2] + m[3][1] * rhs.m[1][2] + m[3][2] * rhs.m[2][2] + m[3][3] * rhs.m[3][2];

    tmp.m[0][3] = m[0][0] * rhs.m[0][3] + m[0][1] * rhs.m[1][3] + m[0][2] * rhs.m[2][3] + m[0][3] * rhs.m[3][3];
    tmp.m[1][3] = m[1][0] * rhs.m[0][3] + m[1][1] * rhs.m[1][3] + m[1][2] * rhs.m[2][3] + m[1][3] * rhs.m[3][3];
    tmp.m[2][3] = m[2][0] * rhs.m[0][3] + m[2][1] * rhs.m[1][3] + m[2][2] * rhs.m[2][3] + m[2][3] * rhs.m[3][3];
    tmp.m[3][3] = m[3][0] * rhs.m[0][3] + m[3][1] * rhs.m[1][3] + m[3][2] * rhs.m[2][3] + m[3][3] * rhs.m[3][3];
    
    return tmp;
}

vec3 Matrix4::operator*(vec3 const & rhs) const
{
    float x = m[0][0] * rhs.x + m[0][1] * rhs.y + m[0][2] * rhs.z + m[0][3] * 1;
    float y = m[1][0] * rhs.x + m[1][1] * rhs.y + m[1][2] * rhs.z + m[1][3] * 1;
    float z = m[2][0] * rhs.x + m[2][1] * rhs.y + m[2][2] * rhs.z + m[2][3] * 1;
    float w = m[3][0] * rhs.x + m[3][1] * rhs.y + m[3][2] * rhs.z + m[3][3] * 1;
    return vec3 {x / w, y / w, z / w};
}

Vector4 Matrix4::operator*(Vector4 const& rhs) const
{
    float x = m[0][0] * rhs[0] + m[0][1] * rhs[1] + m[0][2] * rhs[2] + m[0][3] * rhs[3];
    float y = m[1][0] * rhs[0] + m[1][1] * rhs[1] + m[1][2] * rhs[2] + m[1][3] * rhs[3];
    float z = m[2][0] * rhs[0] + m[2][1] * rhs[1] + m[2][2] * rhs[2] + m[2][3] * rhs[3];
    float w = m[3][0] * rhs[0] + m[3][1] * rhs[1] + m[3][2] * rhs[2] + m[3][3] * rhs[3];
    return Vector4 {x, y, z, w};
}

bool Matrix4::operator==(Matrix4 const & rhs) const
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (m[i][j] != rhs.m[i][j])
            {
                return false;
            }

        }
    }
    return true;
}

bool Matrix4::operator!=(Matrix4 const & rhs) const
{
    return !(*this == rhs);
}

//TODO: inverse

// TODO
float Matrix4::determinant() const
{
    return 0.0f;
}

Matrix4 Matrix4::transpose() const
{
    float tmp[4][4];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            tmp[j][i] = m[i][j];
        }
    }
    return Matrix4{tmp};
}

Matrix4 Matrix4::remove_translation()
{
    m[0][3] = 0;
    m[1][3] = 0;
    m[2][3] = 0;
    return *this;
}

Matrix4::Matrix4(float tmp[4][4])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m[i][j] = tmp[i][j];
        }
    }
}

std::ostream& operator<<(std::ostream & os, Matrix4 const & rhs)
{
    os << "[";
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            os << rhs.m[i][j] << ", ";
        }
        os << "\n";
    }
    os << "]";
    return os;
}


Matrix4 frustum_projection_matrix(float near, float far, float right, float left, float top, float bottom)
{
    Matrix4 frustum {};

    frustum.m[0][0] = (2.0f * near) / (right - left);
    frustum.m[0][2] = (right + left) / (right - left);
    frustum.m[1][1] = (2.0f * near) / (top - bottom);
    frustum.m[1][2] = (top + bottom) / (top - bottom);
    frustum.m[2][2] = -(far + near) / (far - near);
    frustum.m[2][3] = -(2.0f * far * near) / (far - near);
    frustum.m[3][2] = -1;
    frustum.m[3][3] = 0;

    return frustum;
}

Matrix4 fov_projection_matrix(float fovy, float aspect, float near, float far)
{
    float const rad = fovy * 3.1415f / 180.0f;
    float const tanHalfFovy = tan(rad / 2.0f);

    Matrix4 result {};
    result.m[0][0] = 1 / (aspect * tanHalfFovy);
    result.m[1][1] = 1 / (tanHalfFovy);
    result.m[2][2] = -(far + near) / (far - near);
    result.m[3][2] = -1;
    result.m[2][3] = -(2 * far * near) / (far - near);
    return result;
}

Matrix4 ortho_projection_matrix(float left, float right, float bottom, float top, float near, float far)
{
    Matrix4 ortho {};

    float tx = -(right + left) / (right - left);
    float ty = -(top + bottom) / (top - bottom);
    float tz = -(far + near) / (far - near);

    ortho.m[0][0] = 2.0f / (right - left);
    ortho.m[1][1] = 2.0f / (top - bottom);
    ortho.m[2][2] = -2.0f / (far - near);
    ortho.m[0][3] = tx;
    ortho.m[1][3] = ty;
    ortho.m[2][3] = tz;

    return ortho;
}

Matrix4 look_at(vec3 const& position, vec3 const& look_at, vec3 const& up_vector)
{
    vec3 n {position - look_at};
    n.normalize();
    vec3 u {up_vector.cross(n)};
    u.normalize();
    vec3 v {n.cross(u)};

    Matrix4 rotation
    {
        u.x, u.y, u.z, 0,
        v.x, v.y, v.z, 0,
        n.x, n.y, n.z, 0,
        0, 0, 0, 1
    };
    
    Matrix4 translation {translation_matrix(-position.x, -position.y, -position.z)};

    return (rotation * translation);
}

//angle of rotation is in degrees
Matrix4 rotation_matrix(float angle, float x, float y, float z)
{
    Matrix4 matrix {};

    float c = (float) cos(angle * 3.1415 / 180);
    float s = (float) sin(angle * 3.1415 / 180);
    vec3 vec {x, y, z};
    if (vec.length() != 1)
    {
        vec.normalize();
        x = vec.x;
        y = vec.y;
        z = vec.z;
    }

    matrix.m[0][0] = x * x * (1 - c) + c;
    matrix.m[1][0] = y * x * (1 - c) + z * s;
    matrix.m[2][0] = x * z * (1 - c) - y * s;
    matrix.m[0][1] = x * y * (1 - c) - z * s;
    matrix.m[1][1] = y * y * (1 - c) + c;
    matrix.m[2][1] = y * z * (1 - c) + x * s;
    matrix.m[0][2] = x * z * (1 - c) + y * s;
    matrix.m[1][2] = y * z * (1 - c) - x * s;
    matrix.m[2][2] = z * z * (1 - c) + c;
    matrix.m[3][3] = 1;

    return matrix;
}

Matrix4 rotation_matrix(float angle, vec3 const& r)
{
    return rotation_matrix(angle, r.x, r.y, r.z);
}

Matrix4 rotation_matrix(float x, float y, float z)
{
    return rotation_matrix(x, 1, 0, 0) *
           rotation_matrix(y, 0, 1, 0) *
           rotation_matrix(z, 0, 0, 1);
}

/**
 *  Gives a rotation matrix that transforms unit vector a into unit vector b.
 * 
 */
Matrix4 rotation_matrix(vec3 const& a, vec3 const& b)
{
    if (a == b)
    {
        return mat4{};
    }
    if (a == b * -1)
    {
        return rotation_matrix(180, 0, 1, 0);
    }

    vec3 v {a.cross(b)};
    float s {v.length()};
    float c {a * b};
    mat4 Vx {
        0,   -v.z, v.y, 0,
        v.z,  0, - v.x, 0,
        -v.y, v.x, 0,   0,
        0,    0,   0,   1
    };

    return mat4{} + Vx + Vx * Vx * (1 - c) / (s * s);
}

Matrix4 translation_matrix(float x, float y, float z)
{
    Matrix4 matrix {};

    matrix.m[0][0] = 1;
    matrix.m[1][1] = 1;
    matrix.m[2][2] = 1;
    matrix.m[3][3] = 1;
    matrix.m[0][3] = x;
    matrix.m[1][3] = y;
    matrix.m[2][3] = z;

    return matrix;
}

Matrix4 translation_matrix(vec3 const& pos)
{
    return translation_matrix(pos.x, pos.y, pos.z);
}

Matrix4 scale_matrix(float x, float y, float z)
{
    return Matrix4 {
        x, 0.0f, 0.0f, 0.0f,
        0.0f, y, 0.0f, 0.0f,
        0.0f, 0.0f, z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f};
}

#include "Vector.h"

#include <stdexcept>
#include <cmath>
#include <stdexcept>

Vector2::Vector2()
    : x {0}, y{0}
{
}

Vector2::Vector2(Vector2 const & rhs)
    : x {rhs.x}, y{rhs.y}
{
}

Vector2::Vector2(float x, float y)
    : x {x}, y {y}
{
}

float Vector2::length() const
{
    return sqrt(x * x + y * y);
}

void Vector2::normalize()
{
    if (length() != 0)
    {
        operator/=(length());
    }
}

Vector2 Vector2::operator=(Vector2 const & rhs)
{
    Vector2 tmp {rhs};
    *this = tmp;
    return *this;
}

Vector2 Vector2::operator+(Vector2 const & rhs) const
{
    return Vector2{x + rhs.x, y + rhs.y};
}

Vector2 Vector2::operator+=(Vector2 const & rhs)
{
    *this = operator+(rhs);
    return *this;
}

Vector2 Vector2::operator-(Vector2 const & rhs) const
{
    return Vector2{x - rhs.x, y - rhs.y};
}

Vector2 Vector2::operator-=(Vector2 const & rhs)
{
    *this = operator-(rhs);
    return *this;
}

Vector2 Vector2::operator*(float f) const
{
    return Vector2 {x * f, y * f};
}

Vector2 Vector2::operator*=(float f)
{
    *this = operator*(f);
    return *this;
}

Vector2 Vector2::operator/(float f) const
{
    return operator*(1.0f / f);
}

Vector2 Vector2::operator/=(float f)
{
    *this = operator/(f);
    return *this;
}

float Vector2::operator*(Vector2 const & rhs)
{
    return x * rhs.x + y * rhs.y;
}

bool Vector2::operator==(Vector2 const & rhs) const
{
    return x == rhs.x && y == rhs.y;
}

bool Vector2::operator!=(Vector2 const & rhs) const
{
    return !(*this == rhs);
}

bool Vector2::operator<(Vector2 const& r) const
{
     return (x < r.x || (x == r.x && y < r.y));
}

std::ostream& operator<<(std::ostream& os, Vector2 const& rhs)
{
    os << "[" << rhs.x << ", " << rhs.y << "]";
    return os;
}

// =================
// ===| Vector3 |===
// =================

Vector3::Vector3()
{
    x = 0;
    y = 0;
    z = 0;
}

Vector3::Vector3(Vector3 const & rhs)
{
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
}

Vector3::Vector3(float x, float y, float z)
    : x {x}, y {y}, z {z}
{
}

float Vector3::length() const
{
    return sqrt(x * x + y * y + z * z);
}


Vector3 Vector3::normalize()
{
    if (length() != 0)
    {
        operator/=(length());
    }
    return *this;
}

Vector3::~Vector3()
{
}

Vector3 Vector3::operator=(Vector3 const & rhs)
{
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    
    return *this;
}

Vector3::Vector3(objl::Vector3 const& rhs)
{
    *this = rhs;
}

vec3& Vector3::operator=(objl::Vector3 const& rhs)
{
	x = rhs.X;
	y = rhs.Y;
	z = rhs.Z;

	return *this;
}

Vector3 Vector3::operator+(Vector3 const & rhs) const
{
    Vector3 tmp {};
    
    tmp.x = x + rhs.x;
    tmp.y = y + rhs.y;
    tmp.z = z + rhs.z;
    
    return tmp;
}

Vector3 Vector3::operator+=(Vector3 const & rhs)
{
    *this = operator+(rhs);
    return *this;
}

Vector3 Vector3::operator-(Vector3 const & rhs) const
{
    Vector3 tmp {};
    
    tmp.x = x - rhs.x;
    tmp.y = y - rhs.y;
    tmp.z = z - rhs.z;
    
    return tmp;
}

Vector3 Vector3::operator-=(Vector3 const & rhs)
{
    *this = operator-(rhs);
    return *this;
}

Vector3 Vector3::operator*(float f) const
{
    Vector3 tmp {};
    
    tmp.x = x * f;
    tmp.y = y * f;
    tmp.z = z * f;
    
    return tmp;
}

Vector3 Vector3::operator*=(float f)
{
    *this = operator*(f);
    return *this;
}

Vector3 Vector3::operator/(float f) const
{
    return operator*(1.0f / f);
}

Vector3 Vector3::operator/=(float f)
{
    *this = operator/(f);
    return *this;
}

float Vector3::operator*(Vector3 const & rhs) const
{
    float sum {0};

    sum += x * rhs.x;
    sum += y * rhs.y;
    sum += z * rhs.z;
    
    return sum;
}

bool Vector3::operator==(Vector3 const & rhs) const
{
    return x == rhs.x && y == rhs.y && z == rhs.z;
}

bool Vector3::operator!=(Vector3 const & rhs) const
{
    return !(*this == rhs);
}

Vector3 Vector3::cross(Vector3 const & rhs) const
{
    float x = this->y * rhs.z - this->z * rhs.y;
    float y = this->z * rhs.x - this->x * rhs.z;
    float z = this->x * rhs.y - this->y * rhs.x;
    return Vector3 {x, y, z};
}

std::ostream& operator<<(std::ostream& os, vec3 const& rhs)
{
    os << "[" << rhs.x << ", " << rhs.y << ", " << rhs.z << "]";
    return os;
}

//=================
//===| Vector4 |===
//=================

Vector4::Vector4()
{
    for (int i = 0; i < 4; i++)
    {
        x[i] = 0;
    }
}

Vector4::Vector4(Vector4 const & rhs)
{
    for (int i = 0; i < 4; i++)
    {
        x[i] = rhs.x[i];
    }
}

Vector4::Vector4(std::initializer_list<float> list)
{
    if (list.size() != 4)
    {
        throw std::domain_error("Dimension missmatch in constructor of Vector.");
    }

    for (int i = 0; i < 4; i++)
    {
        x[i] = *(list.begin() + i);
    }
}

float Vector4::length() const
{
    float sum {0};

    for (int i = 0; i < 4; i++)
    {
        sum += x[i] * x[i];
    }
    return sqrt(sum);
}


void Vector4::normalize()
{
    operator/=(length());
}


Vector4::Vector4(float tmp[4])
{
    for (int i = 0; i < 4; i++)
    {
        x[i] = tmp[i];
    }
}

Vector4::~Vector4()
{
}

Vector4 Vector4::operator=(Vector4 const & rhs)
{
    for (int i = 0; i < 4; i++)
    {
        x[i] = rhs.x[i];
    }
    return *this;
}

Vector4 Vector4::operator+(Vector4 const & rhs) const
{
    float tmp[4];
    for (int i = 0; i < 4; i++)
    {
        tmp[i] = x[i] + rhs.x[i];
    }
    return Vector4(tmp);
}

Vector4 Vector4::operator+=(Vector4 const & rhs)
{
    *this = operator+(rhs);
    return *this;
}

Vector4 Vector4::operator-(Vector4 const & rhs) const
{
    float tmp[4];
    for (int i = 0; i < 4; i++)
    {
        tmp[i] = x[i] - rhs.x[i];
    }
    return Vector4(tmp);
}

Vector4 Vector4::operator-=(Vector4 const & rhs)
{
    *this = operator-(rhs);
    return *this;
}

Vector4 Vector4::operator*(float f) const
{
    float tmp[4];
    for (int i = 0; i < 4; i++)
    {
        tmp[i] = x[i] * f;
    }
    return Vector4 {tmp};
}

Vector4 Vector4::operator*=(float f)
{
    *this = operator*(f);
    return *this;
}

Vector4 Vector4::operator/(float f) const
{
    return operator*(1.0f / f);
}

Vector4 Vector4::operator/=(float f)
{
    *this = operator/(f);
    return *this;
}

float& Vector4::operator[](int index)
{
    if (index >= 4)
    {
        throw std::logic_error("index out of bounds in vector.");
    }
    return x[index];
}

float Vector4::operator[](int index) const
{
    if (index >= 4)
    {
        throw std::logic_error("index out of bounds in vector.");
    }
    return x[index];
}

float Vector4::operator*(Vector4 const & rhs)
{
    float sum {0};

    for (int i = 0; i < 4; i++)
    {
        sum += x[i] * rhs.x[i];
    }
    return sum;
}

bool Vector4::operator==(Vector4 const & rhs) const
{
    for (int i = 0; i < 4; i++)
    {
        if (x[i] != rhs.x[i])
        {
            return false;
        }
    }
    return true;
}

bool Vector4::operator!=(Vector4 const & rhs) const
{
    return !(*this == rhs);
}

std::ostream& operator<<(std::ostream & os, Vector4 const & rhs)
{
    os << "[";
    for (int i = 0; i < 2; i++)
    {
        os << rhs[i] << ", ";
    }
    os << rhs[2] << "]";
    return os;
}
#include "Vector3D.h"
#include <cmath>

Vector3D::Vector3D()
{
	_x = 0.0f;
	_y = 0.0f;
	_z = 0.0f;
}

Vector3D::Vector3D(float x, float y, float z)
{
	_x = x;
	_y = y;
	_z = z;
}

Vector3D::Vector3D(const Vector3D& other)
{
	Copy(other);
}

float Vector3D::GetX() const
{
	return _x;
}

float Vector3D::GetY() const
{
	return _y;
}

float Vector3D::GetZ() const
{
	return _z;
}

float Vector3D::GetLength() const
{
	return sqrt(GetX() * GetX() + GetY() * GetY() + GetZ() * GetZ());
}

Vector3D Vector3D::GetUnitVector() const
{
	return { GetX() / GetLength(), GetY() / GetLength(), GetZ() / GetLength() };
}

void Vector3D::SetX(const float x)
{
	_x = x;
}

void Vector3D::SetY(const float y)
{
	_y = y;
}

void Vector3D::SetZ(const float z)
{
	_z = z;
}

Vector3D& Vector3D::operator=(const Vector3D& rhs)
{
	if (this != &rhs)
	{
		Copy(rhs);
	}
	return *this;
}

bool Vector3D::operator==(const Vector3D& rhs) const
{
	return (_x == rhs.GetX() && _y == rhs.GetY() && _z == rhs.GetZ());
}

const Vector3D Vector3D::operator+(const Vector3D& rhs) const
{
	return Vector3D(_x + rhs.GetX(), _y + rhs.GetY(), _z + rhs.GetZ());
}

const Vector3D Vector3D::operator-(const Vector3D& rhs) const
{
	return Vector3D(_x - rhs.GetX(), _y - rhs.GetY(), _z - rhs.GetZ());
}

const Vector3D Vector3D::operator*(const float rhs) const
{
	return Vector3D(_x * rhs, _y * rhs, _z * rhs);
}

const Vector3D Vector3D::operator/(const float rhs) const
{
	return Vector3D(_x / rhs, _y / rhs, _z / rhs);
}

const Vector3D Vector3D::operator/(const int rhs) const
{
	return Vector3D(_x / rhs, _y / rhs, _z / rhs);
}

float Vector3D::DotProduct(const Vector3D u, const Vector3D v)
{
	return (u.GetX() * v.GetX() + u.GetY() * v.GetY() + u.GetZ() * v.GetZ());
}

Vector3D Vector3D::CrossProduct(const Vector3D u, const Vector3D v)
{
	return Vector3D(u.GetY() * v.GetZ() - u.GetZ() * v.GetY(), u.GetZ() * v.GetX() - u.GetX() * v.GetZ(), u.GetX() * v.GetY() - u.GetY() * v.GetX());
}

void Vector3D::Copy(const Vector3D& other)
{
	_x = other.GetX();
	_y = other.GetY();
	_z = other.GetZ();
}

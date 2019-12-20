#include "Vertex.h"

Vertex::Vertex()
{
	_x = 0.0f;
	_y = 0.0f;
	_z = 0.0f;
	_w = 1.0f;
	_normalVector = Vector3D(0, 0, 0);
	_color = RGB(0, 0, 0);
	_contributions = 0;
	_UOverZ = 0;
	_VOverZ = 0;
	_ZRecip = 0;
}

Vertex::Vertex(float x, float y, float z)
{
	_x = x;
	_y = y;
	_z = z;
	_w = 1.0f;
	_normalVector = Vector3D(0, 0, 0);
	_color = RGB(0, 0, 0);
	_contributions = 0;
	_UOverZ = 0;
	_VOverZ = 0;
	_ZRecip = 0;
}

Vertex::Vertex(const Vertex & other)
{
	Copy(other);
}

float Vertex::GetX() const
{
	return _x;
}

float Vertex::GetY() const
{
	return _y;
}

float Vertex::GetZ() const
{
	return _z;
}

float Vertex::GetW() const
{
	return _w;
}

float Vertex::GetPerspectiveZ() const
{
	return _perspectiveZ;
}

Vector3D Vertex::GetNormalVector() const
{
	return _normalVector;
}

COLORREF Vertex::GetColor() const
{
	return _color;
}

int Vertex::GetContributions() const
{
	return _contributions;
}

float Vertex::GetUOverZ() const
{
	return _UOverZ;
}

float Vertex::GetVOverZ() const
{
	return _VOverZ;
}

float Vertex::GetZRecip() const
{
	return _ZRecip;
}

void Vertex::SetX(const float x)
{
	_x = x;
}

void Vertex::SetY(const float y)
{
	_y = y;
}

void Vertex::SetZ(const float z)
{
	_z = z;
}

void Vertex::SetW(const float w)
{
	_w = w;
}

void Vertex::SetNormalVector(const Vector3D normalVector)
{
	_normalVector = normalVector;
}

void Vertex::SetColor(const int red, const int green, const int blue)
{
	if (red >= 0 && red <= 255 && green >= 0 && green <= 255 && blue >= 0 && blue <= 255)
	{
		_color = RGB(red, green, blue);
	}
}

void Vertex::SetContributions(const int contribuitons)
{
	_contributions = contribuitons;
}

void Vertex::SetUOverZ(const float UOverZ)
{
	_UOverZ = UOverZ;
}

void Vertex::SetVOverZ(const float VOverZ)
{
	_VOverZ = VOverZ;
}

void Vertex::SetZRecip(const float ZRecip)
{
	_ZRecip = ZRecip;
}

Vertex& Vertex::operator=(const Vertex& rhs)
{
	// Only do the assignment if we are not assigning
	// to ourselves
	if (this != &rhs)
	{
		Copy(rhs);
	}
	return *this;
}

// The const at the end of the declaraion for '==" indicates that this operation does not change
// any of the member variables in this class.

bool Vertex::operator==(const Vertex& rhs) const
{
	return (_x == rhs.GetX() && _y == rhs.GetY() && _z == rhs.GetZ() && _w == rhs.GetW());
}

// You can see three different uses of 'const' here:
//
// The first const indicates that the method changes the return value, but it is not moved in memory
// The second const indicates that the parameter is passed by reference, but it is not modified
// The third const indicates that the operator does not change any of the memory variables in the class.

const Vertex Vertex::operator+(const Vertex& rhs) const
{
	return Vertex(_x + rhs.GetX(), _y + rhs.GetY(), _z + rhs.GetZ());
}

void Vertex::Dehomogenize()
{
	_perspectiveZ = _w;
	_x /= _w;
	_y /= _w;
	_z /= _w;
	_w /= _w;
}

Vector3D Vertex::GenerateVector(const Vertex& other) const
{
	return Vector3D(other.GetX() - _x, other.GetY() - _y, other.GetZ() - _z);
}

void Vertex::Copy(const Vertex& other)
{
	_x = other.GetX();
	_y = other.GetY();
	_z = other.GetZ();
	_w = other.GetW();
	_perspectiveZ = other.GetPerspectiveZ();
	_normalVector = other.GetNormalVector();
	_color = other.GetColor();
	_contributions = other.GetContributions();
	_UOverZ = other.GetUOverZ();
	_VOverZ = other.GetVOverZ();
	_ZRecip = other.GetZRecip();
}

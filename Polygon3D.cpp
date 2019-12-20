#include "Polygon3D.h"

Polygon3D::Polygon3D() : _indices{ 0 }
{
}

Polygon3D::Polygon3D(int index0, int index1, int index2, int uvIndex0, int uvIndex1, int uvIndex2)
{
	_indices[0] = index0;
	_indices[1] = index1;
	_indices[2] = index2;
	_uvIndices[0] = uvIndex0;
	_uvIndices[1] = uvIndex1;
	_uvIndices[2] = uvIndex2;
	_averageZ = 0;
	_color = RGB(0, 0, 0);
}

Polygon3D::~Polygon3D()
{
}

Polygon3D::Polygon3D(const Polygon3D& p) : _indices{ 0 }
{
	Copy(p);
}

int Polygon3D::GetIndex(int index) const
{
	return _indices[index];
}

int Polygon3D::GetUVIndex(int index) const
{
	return _uvIndices[index];
}

bool Polygon3D::GetVisibility() const
{
	return _isVisible;
}

Vector3D Polygon3D::GetNormalVector() const
{
	return _normalVector;
}

float Polygon3D::GetAverageZ() const
{
	return _averageZ;
}

COLORREF Polygon3D::GetColor() const
{
	return _color;
}

void Polygon3D::SetVisibility(bool visibility)
{
	_isVisible = visibility;
}

void Polygon3D::SetNormalVector(const Vector3D normal)
{
	_normalVector = normal;
}

void Polygon3D::SetAverageZ(const float averageZ)
{
	_averageZ = averageZ;
}

void Polygon3D::SetColor(const int red, const int green, const int blue)
{
	if (red >= 0 && red <= 255 && green >= 0 && green <= 255 && blue >= 0 && blue <= 255)
	{
		_color = RGB(red, green, blue);
	}
}

Polygon3D& Polygon3D::operator= (const Polygon3D& rhs)
{
	if (this != &rhs)
	{
		Copy(rhs);
	}
	return *this;
}

void Polygon3D::Copy(const Polygon3D& other)
{
	_indices[0] = other.GetIndex(0);
	_indices[1] = other.GetIndex(1);
	_indices[2] = other.GetIndex(2);
	_uvIndices[0] = other.GetUVIndex(0);
	_uvIndices[1] = other.GetUVIndex(1);
	_uvIndices[2] = other.GetUVIndex(2);
	_isVisible = other.GetVisibility();
	_normalVector = other.GetNormalVector();
	_averageZ = other.GetAverageZ();
	_color = other.GetColor();
}

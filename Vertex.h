#pragma once
#include "Vector3D.h"
#include <Windows.h>

class Vertex
{
public:
	Vertex();
	Vertex(float x, float y, float z);
	Vertex(const Vertex& other);

	// Accessors
	float GetX() const;
	float GetY() const;
	float GetZ() const;
	float GetW() const;
	Vector3D GetNormalVector() const;
	COLORREF GetColor() const;
	int GetContributions() const;
	
	// Mutators
	void SetX(const float x);
	void SetY(const float y);
	void SetZ(const float z);
	void SetW(const float w);
	void SetNormalVector(const Vector3D normal);
	void SetColor(const int red, const int green, const int blue);
	void SetContributions(const int contributions);

	// Assignment operator
	Vertex& operator= (const Vertex& rhs);
	bool operator== (const Vertex& rhs) const;
	const Vertex operator+ (const Vertex& rhs) const;

	void Dehomogenize();

	Vector3D GenerateVector(const Vertex& other) const;

private:
	float _x;
	float _y;
	float _z;
	float _w;
	Vector3D _normalVector;
	COLORREF _color;
	int _contributions;

	void Copy(const Vertex& other);
};

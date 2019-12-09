#pragma once
#include "Vector3D.h"
#include <Windows.h>

class Polygon3D
{
public:
	Polygon3D();
	Polygon3D(int index0, int index1, int index2);
	~Polygon3D();
	Polygon3D(const Polygon3D& p);

	int GetIndex(int index) const;
	bool GetVisibility() const;
	Vector3D GetNormalVector() const;
	float GetAverageZ() const;
	COLORREF GetColor() const;

	void SetVisibility(const bool visibility);
	void SetNormalVector(const Vector3D normal);
	void SetAverageZ(const float averageZ);
	void SetColor(const int red, const int green, const int blue);

	Polygon3D& operator= (const Polygon3D& rhs);

private:
	int _indices[3];
	bool _isVisible = true;
	Vector3D _normalVector;
	float _averageZ;
	COLORREF _color;

	void Copy(const Polygon3D& other);
};


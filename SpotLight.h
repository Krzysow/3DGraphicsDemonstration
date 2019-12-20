#pragma once
#include "PointLight.h"
#include "Vertex.h"
#include "Matrix.h"
#include <cmath>

class SpotLight : public PointLight
{
public:
	SpotLight(int red, int green, int blue, Vertex position, float a, float b, float c, Vector3D direction, float innerEdge, float outerEdge) : PointLight(red, green, blue, position, a, b, c)
	{
		_direction = direction;
		_innerEdge = cos(Matrix::toRadians(innerEdge));
		_outerEdge = cos(Matrix::toRadians(outerEdge));
	}
	~SpotLight();

	Vector3D GetDirection() const;
	float GetInnerEdge() const;
	float GetOuterEdge() const;

private:
	Vector3D _direction;
	float _innerEdge;
	float _outerEdge;
};

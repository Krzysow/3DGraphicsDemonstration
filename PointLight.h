#pragma once
#include "Light.h"
#include "Vertex.h"

class PointLight : public Light
{
public:
	PointLight(int red, int green, int blue, Vertex position, float a, float b, float c) : Light(red, green, blue)
	{
		_sourcePosition = position;
		_a = a;
		_b = b;
		_c = c;
	}
	~PointLight();

	Vertex GetSourcePosition() const;
	float GetA() const;
	float GetB() const;
	float GetC() const;

protected:
	Vertex _sourcePosition;
	float _a;
	float _b;
	float _c;
};


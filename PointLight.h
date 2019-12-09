#pragma once
#include "Vertex.h"

class PointLight
{
public:
	PointLight(int red, int green, int blue, Vertex position, float a, float b, float c);
	~PointLight();

	int GetRedIntensity() const;
	int GetGreenIntensity() const;
	int GetBlueIntensity() const;
	Vertex GetSourcePosition() const;
	float GetA() const;
	float GetB() const;
	float GetC() const;

private:
	int _redIntensity;
	int _greenIntensity;
	int _blueIntensity;
	Vertex _sourcePosition;
	float _a;
	float _b;
	float _c;
};


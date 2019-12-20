#include "PointLight.h"

PointLight::~PointLight()
{
}

Vertex PointLight::GetSourcePosition() const
{
	return _sourcePosition;
}

float PointLight::GetA() const
{
	return _a;
}

float PointLight::GetB() const
{
	return _b;
}

float PointLight::GetC() const
{
	return _c;
}

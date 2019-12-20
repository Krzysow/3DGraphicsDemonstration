#include "SpotLight.h"

SpotLight::~SpotLight()
{
}

Vector3D SpotLight::GetDirection() const
{
	return _direction;
}

float SpotLight::GetInnerEdge() const
{
	return _innerEdge;
}

float SpotLight::GetOuterEdge() const
{
	return _outerEdge;
}

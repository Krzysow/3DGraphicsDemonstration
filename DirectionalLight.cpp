#include "DirectionalLight.h"

DirectionalLight::~DirectionalLight()
{
}

Vector3D DirectionalLight::GetSourceDirection() const
{
	return _sourceDirection;
}

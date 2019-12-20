#pragma once
#include "Light.h"
#include "Vector3D.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight(int red, int green, int blue, Vector3D direction) : Light(red, green, blue)
	{
		_sourceDirection = direction;
	}
	~DirectionalLight();

	Vector3D GetSourceDirection() const;

private:
	Vector3D _sourceDirection;
};

